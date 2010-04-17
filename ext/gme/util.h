#ifndef _UTIL_H
#define _UTIL_H

#include <ruby.h>

void handle_error(const char* string, VALUE exception);
void write_samples(FILE* file, int buffer_size, short* buffer);

#endif
