#ifndef _GME_HELPERS_H
#define _GME_HELPERS_H

#include <ruby.h>
#include <rubyio.h>

extern VALUE eTrackNotStarted;
extern VALUE eGenericException;

void gme_ruby_emu_free(void* pointer);

void  raise_if_track_has_not_started(VALUE emulator);
void  raise_if_invalid_file(VALUE file);
void  get_internal_buffer(VALUE emulator, short** buffer, int* buffer_len);
FILE* get_stdio_pointer(VALUE file);

#endif
