#include <ruby.h>
#include "gme_funcs.h"
#include "util.h"

// classes and modules representations
VALUE mGME;
VALUE cEmulator;

VALUE eGenericException;
VALUE eInvalidFile;

void Init_gme_ext()
{
    mGME = rb_define_module("GME");
    rb_require("gme/exceptions");
    eGenericException = rb_define_class_under(mGME, "GenericException", rb_eException);
    eInvalidFile = rb_define_class_under(mGME, "InvalidFile", eGenericException);
    cEmulator = rb_define_class_under(mGME, "Emulator", rb_cObject);
    rb_funcall(cEmulator, rb_intern("attr_reader"), 1, ID2SYM(rb_intern("info")));
    rb_funcall(cEmulator, rb_intern("attr_reader"), 1, ID2SYM(rb_intern("track_count")));
    /* rb_define_alloc_func(cEmulator, gme_alloc); */
    rb_define_singleton_method(cEmulator, "open", gme_ruby_open, -1);
    rb_define_method(cEmulator, "close", gme_ruby_close, 0);
    rb_define_method(cEmulator, "start_track", gme_ruby_start_track, -1);
    rb_define_method(cEmulator, "get_samples", gme_ruby_get_samples, 1);
    rb_define_method(cEmulator, "play_to_file", gme_ruby_play_to_file, 1);
}
