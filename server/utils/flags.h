#ifndef FLAGS_H
#define FLAGS_H

#include "softwar_server.h"

int flag_log(t_options_flag *options_flags, char *data);
int flag_verbose(t_options_flag *options_flags, char *data);
int flag_size(t_options_flag *options_flags, char *data);
int flag_rep_port(t_options_flag *options_flags, char *data);
int flag_pub_port(t_options_flag *options_flags, char *data);
int flag_cycle(t_options_flag *options_flags, char *data);

#endif