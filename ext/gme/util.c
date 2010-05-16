#include "util.h"

void handle_error(const char* string, VALUE exception)
{
    if(string) {
        rb_raise(exception, "%s", string);
    }
}

void write_samples(FILE* file, int buffer_size, short* buffer)
{
    fwrite(buffer, sizeof(short), buffer_size, file);
}
