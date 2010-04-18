#ifndef GME_FUNCS_H
#define GME_FUNCS_H

#include <ruby.h>
#include <gme/gme.h>

VALUE gme_ruby_open(int argc, VALUE* argv, VALUE self);
VALUE gme_ruby_close(VALUE self);
VALUE gme_ruby_start_track(int argc, VALUE* argv, VALUE self);
VALUE gme_ruby_get_samples(VALUE self, VALUE samples);
VALUE gme_ruby_play_to_file(VALUE self, VALUE file);

#endif
