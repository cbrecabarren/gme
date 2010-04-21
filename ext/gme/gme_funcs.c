#include "gme_funcs.h"
#include "util.h"

#include <rubyio.h>

extern VALUE cEmulator;
extern VALUE eGenericException;
extern VALUE eInvalidFile;

void gme_ruby_emu_free(void* pointer);

/*
 * Opens a new input file
 * Returns a new instance of GME::Emulator
 */
VALUE gme_ruby_open(int argc, VALUE* argv, VALUE self)
{
    Music_Emu*  emulator;
    int         c_sample_rate;
    char*       c_path;
    int         track = 0; // uses track 0 for getting info (FIXME)
    gme_info_t* info;

    // internal buffer
    short* buffer;
    int    buffer_length;

    // options hash
    VALUE options;

    // use the first (mandatory) argument, as path to file
    VALUE string = StringValue(argv[0]);
    c_path = RSTRING_PTR(string);

    // use the second argument, if present, as the options hash
    VALUE temp;
    if(argc >= 2){
        temp = rb_check_convert_type(argv[1], T_HASH, "Hash", "to_hash");
        if(!NIL_P(temp)) options = temp;
        else options = rb_hash_new();
    }
    else {
        options = rb_hash_new();
    }

    // set sample rate
    VALUE sample_rate = rb_hash_aref(options, ID2SYM(rb_intern("sample_rate")));
    if(!NIL_P(sample_rate)) {
        c_sample_rate = FIX2INT(sample_rate);
    }
    else {
        // default value
        c_sample_rate = 44100;
    }

    // set buffer length
    VALUE buffer_len = rb_hash_aref(options, ID2SYM(rb_intern("buffer_length")));
    if(!NIL_P(buffer_len)) {
        buffer_length = FIX2INT(buffer_len);
    }
    else {
        // default value
        buffer_length = 2048;
    }

    // opens the specified file
    handle_error(gme_open_file(c_path, &emulator, c_sample_rate), eInvalidFile);
    // and gets the info on the track
    handle_error(gme_track_info(emulator, &info, track), eGenericException);

    // creates a new instance of GME::Emulator, as a wrapper around Music_Emu
    VALUE new_instance = Data_Wrap_Struct(cEmulator, 0, gme_ruby_emu_free, emulator);

    // allocates memory for the internal buffer
    buffer = ALLOC_N(short, buffer_length);
    // and saves a reference for later use (hack?)
    rb_iv_set(new_instance, "@internal_buffer", LONG2NUM((long)buffer));
    rb_iv_set(new_instance, "@internal_buffer_length", INT2NUM(buffer_length));

    // Fills the info hash
    VALUE info_hash = rb_hash_new();
    rb_hash_aset(info_hash, ID2SYM(rb_intern("play_length")), INT2FIX(info->play_length));
    rb_hash_aset(info_hash, ID2SYM(rb_intern("length")), INT2FIX(info->length));
    rb_hash_aset(info_hash, ID2SYM(rb_intern("intro_length")), INT2FIX(info->intro_length));
    rb_hash_aset(info_hash, ID2SYM(rb_intern("loop_length")), INT2FIX(info->loop_length));
    rb_hash_aset(info_hash, ID2SYM(rb_intern("system")), rb_str_new2(info->system));
    rb_hash_aset(info_hash, ID2SYM(rb_intern("game")), rb_str_new2(info->game));
    rb_hash_aset(info_hash, ID2SYM(rb_intern("song")), rb_str_new2(info->song));
    rb_hash_aset(info_hash, ID2SYM(rb_intern("author")), rb_str_new2(info->author));
    rb_hash_aset(info_hash, ID2SYM(rb_intern("copyright")), rb_str_new2(info->copyright));
    rb_hash_aset(info_hash, ID2SYM(rb_intern("comment")), rb_str_new2(info->comment));
    rb_hash_aset(info_hash, ID2SYM(rb_intern("dumper")), rb_str_new2(info->dumper));
    rb_iv_set(new_instance, "@info", info_hash);
    gme_free_info(info);

    // sets the track count
    int track_count = gme_track_count(emulator);
    rb_iv_set(new_instance, "@track_count", INT2FIX(track_count));

    // returns the new instance of GME::Emulator
    return new_instance;
}

/*
 * is this function really needed?
 */
VALUE gme_ruby_close(VALUE self)
{
    return Qnil;    
}

/*
 * starts a track
 * if not specified, uses track number 0
 */
VALUE gme_ruby_start_track(int argc, VALUE* argv, VALUE self)
{
    Music_Emu* emulator;
    int        c_track;

    Data_Get_Struct(self, Music_Emu, emulator);

    // uses the first argument, if present, as track number
    if(argc >= 1) {
        c_track = FIX2INT(argv[0]);
    }
    else {
        // default value
        c_track = 0;
    }

    // starts the track
    handle_error(gme_start_track(emulator, c_track), eGenericException);

    // returns the track number started
    return INT2FIX(c_track);
}

/*
 * Plays the specified number of samples
 * FIXME: This function allocates a buffer each time it is called.
 *        Maybe we should use a one-time allocated buffer.  
 */
VALUE gme_ruby_get_samples(VALUE self)
{
    Music_Emu* emulator;
    int        c_samples;
    short*     c_buffer;
    int        c_buffer_len;

    Data_Get_Struct(self, Music_Emu, emulator);

    // recovers a pointer to the internal buffer
    c_buffer = (short*) NUM2LONG(rb_iv_get(self, "@internal_buffer"));
    c_buffer_len = NUM2INT(rb_iv_get(self, "@internal_buffer_length"));

    // plays the file, returning the specified number of samples
    handle_error(gme_play(emulator, c_buffer_len, c_buffer), eGenericException);

    // creates a ruby string, containing the buffer content (generated samples)
    VALUE ruby_string = rb_str_new((const char*)c_buffer, c_buffer_len * sizeof(short));

    // returns the played samples
    return ruby_string;
}

/*
 * Plays the track 0 of the input file
 * and writes in the specified (ruby) output file
 */
VALUE gme_ruby_play_to_file(VALUE self, VALUE file)
{
    int        buffer_size = 1024; // hardcoded buffer size (TODO?)
    short*     buffer;
    FILE*      stdio_file;
    Music_Emu* emulator;
    int        track       = 0;    // plays track 0 (TODO?)

    // allocates memory for the buffer
    buffer = (short*) malloc(buffer_size * sizeof(short));

    Data_Get_Struct(self, Music_Emu, emulator);

    // TODO: fix for ruby-1.9 (fptr->stdio_file)
    stdio_file = RFILE(file)->fptr->f;

    // if the stdio pointer couldn't be accesed, exit the program
    if(stdio_file == NULL) {
        rb_fatal("Couldn't access stdio FILE pointer");
    }

    // starts track 0
    handle_error(gme_start_track(emulator, track), eGenericException);

    // gets the play length of the track from the info hash
    VALUE info_hash = rb_iv_get(self, "@info");
    int play_length = FIX2INT(rb_hash_aref(info_hash, ID2SYM(rb_intern("play_length"))));

    // plays the track completely
    while(gme_tell(emulator) < play_length) {
        handle_error(gme_play(emulator, buffer_size, buffer), eGenericException);
        write_samples(stdio_file, buffer_size, buffer);
        fflush(stdio_file);
    }

    // releases the memory of the buffer
    free(buffer);

    return Qnil;
}

/*
 * free function to the GME::Emulator wrapper for Music_Emu
 */
void gme_ruby_emu_free(void* pointer)
{
    if(pointer != NULL) gme_delete(pointer);
}
