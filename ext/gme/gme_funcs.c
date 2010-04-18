#include "gme_funcs.h"
#include "util.h"

#include <rubyio.h>

extern VALUE cEmulator;
extern VALUE eGenericException;
extern VALUE eInvalidFile;

void gme_ruby_emu_free(void* pointer);

VALUE gme_ruby_open(int argc, VALUE* argv, VALUE self)
{
    Music_Emu* emulator;
    int c_sample_rate;
    char* c_path;
    int track = 0;
    gme_info_t* info;

    // use the first (mandatory) argument
    VALUE string = StringValue(argv[0]);
    c_path = RSTRING_PTR(string);

    // use the second argument, if present
    if(argc >= 2) {
        c_sample_rate = FIX2INT(argv[1]);
    }
    else {
        c_sample_rate = 44100;
    }

    handle_error(gme_open_file(c_path, &emulator, c_sample_rate), eInvalidFile);
    handle_error(gme_track_info(emulator, &info, track), eGenericException);

    VALUE new_instance = Data_Wrap_Struct(cEmulator, 0, gme_ruby_emu_free, emulator);

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

    int track_count = gme_track_count(emulator);
    rb_iv_set(new_instance, "@track_count", INT2FIX(track_count));

    gme_free_info(info);

    return new_instance;
}

VALUE gme_ruby_close(VALUE self)
{
    // TODO: Es necesario esto?

    /* Music_Emu* emulator; */
    /* gme_info_t* info; */

    /* Data_Get_Struct(self, Music_Emu, emulator); */
    /* Data_Get_Struct(self, gme_info_t, info); */

    /* gme_ruby_emu_free(emulator); */
    /* gme_ruby_info_free(info); */

    return Qnil;    
}

VALUE gme_ruby_start_track(int argc, VALUE* argv, VALUE self)
{
    Music_Emu* emulator;
    int c_track;

    Data_Get_Struct(self, Music_Emu, emulator);

    if(argc >= 1) {
        c_track = FIX2INT(argv[0]);
    }
    else {
        // default value
        c_track = 0;
    }

    handle_error(gme_start_track(emulator, c_track), eGenericException);

    return Qnil;
}

VALUE gme_ruby_get_samples(VALUE self, VALUE samples)
{
    int buffer_size = FIX2INT(samples);
    Music_Emu* emulator;
    int c_samples;
    short* c_buffer;

    Data_Get_Struct(self, Music_Emu, emulator);

    c_buffer = (short*) malloc(buffer_size * sizeof(short));

    handle_error(gme_play(emulator, buffer_size, c_buffer), eGenericException);

    VALUE ruby_string = rb_str_new((const char*)c_buffer, buffer_size * sizeof(short));

    free(c_buffer);

    return ruby_string;
}

VALUE gme_ruby_play_to_file(VALUE self, VALUE file)
{
    int buffer_size = 1024;
    short* buffer;
    FILE* stdio_file;
    Music_Emu* emulator;
    int track = 0;

    buffer = (short*) malloc(buffer_size * sizeof(short));

    Data_Get_Struct(self, Music_Emu, emulator);

    // TODO: fix for ruby-1.9 (fptr->stdio_file)
    stdio_file = RFILE(file)->fptr->f;

    if(stdio_file == NULL) {
        rb_fatal("Couldn't access stdio FILE pointer");
    }

    gme_ignore_silence(emulator, 1);

    handle_error(gme_start_track(emulator, track), eGenericException);

    VALUE info_hash = rb_iv_get(self, "@info");
    int play_length = FIX2INT(rb_hash_aref(info_hash, ID2SYM(rb_intern("play_length"))));

    while(gme_tell(emulator) < play_length) {
        handle_error(gme_play(emulator, buffer_size, buffer), eGenericException);
        write_samples(stdio_file, buffer_size, buffer);
        fflush(stdio_file);
    }

    free(buffer);

    return Qnil;
}

void gme_ruby_emu_free(void* pointer)
{
    if(pointer != NULL) gme_delete(pointer);
}
