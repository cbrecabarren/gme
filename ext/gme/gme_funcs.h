#ifndef GME_FUNCS_H
#define GME_FUNCS_H

#include <ruby.h>
#include <gme/gme.h>

VALUE gme_ruby_open(int argc, VALUE* argv, VALUE self);
VALUE gme_ruby_close(VALUE self);
VALUE gme_ruby_start_track(int argc, VALUE* argv, VALUE self);
VALUE gme_ruby_get_samples(VALUE self);
VALUE gme_ruby_play_to_file(VALUE self, VALUE file);
VALUE gme_ruby_track_started(VALUE self);
VALUE gme_ruby_tell(VALUE self);
VALUE gme_ruby_track_ended(VALUE self);
VALUE gme_ruby_ignore_silence(VALUE self, VALUE ignore);
VALUE gme_ruby_set_fade(VALUE self, VALUE milliseconds);
VALUE gme_ruby_play(int argc, VALUE* argv, VALUE self);
VALUE gme_ruby_play_silence(VALUE self, VALUE file, VALUE milliseconds);

#endif
