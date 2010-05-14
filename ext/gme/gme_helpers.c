#include "gme_helpers.h"

/*
 * free function to the GME::Emulator wrapper for Music_Emu
 */
void gme_ruby_emu_free(void* pointer)
{
    if(pointer != NULL) gme_delete(pointer);
}

/*
 * Raises an exception if a track has not been started in the emulator.
 */
void raise_if_track_has_not_started(VALUE emulator)
{
    VALUE track_started = rb_iv_get(emulator, "@track_started");
    if(!RTEST(track_started)) rb_raise(eTrackNotStarted, "you must start a track first");
}

/*
 * Raises an exception the specified file is invalid.
 * i.e. doesn't satisfy the required conditions.
 */
void raise_if_invalid_file(VALUE file)
{
    // FIXME: currently it *requires* an object of class File
    if(NIL_P(file) || TYPE(file) != T_FILE) {
        rb_raise(eGenericException, "the file is not valid.");
    }

    FILE* stdio_file = get_stdio_pointer(file);

    // raises an exception if the stdio pointer couldn't be accesed
    if(stdio_file == NULL) {
        rb_raise(eGenericException, "couldn't access stdio FILE pointer of the specified file");
    }
}

/*
 * Recovers a pointer to the internal buffer of the emulator.
 */
void get_internal_buffer(VALUE emulator, short** buffer, int* buffer_len)
{
    *buffer = (short*) NUM2LONG(rb_iv_get(emulator, "@internal_buffer"));
    *buffer_len = NUM2INT(rb_iv_get(emulator, "@internal_buffer_length"));
}

/*
 * Gets the stdio pointer of the specified file.
 */
FILE* get_stdio_pointer(VALUE file)
{
    FILE* stdio_pointer = NULL;

    // TODO: fix for ruby-1.9 (fptr->stdio_file)
    stdio_pointer = RFILE(file)->fptr->f;

    return stdio_pointer;
}
