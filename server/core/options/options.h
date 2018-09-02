#ifndef OPTIONS_H
#define OPTIONS_H

#include "softwar_server.h"

t_options_flag *init_options_flags();
void set_default_options(t_options_flag *flags);
int call_flag_setter(t_options_flag *options_flags, char option, char *data);
int manage_options(int argc, char **argv, t_options_flag *flags);
void usage();

#endif