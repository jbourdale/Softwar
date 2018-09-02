#include "flags.h"

int flag_log(t_options_flag *options_flags, char *data)
{
  options_flags->log = data;
  return 0;
}

int flag_verbose(t_options_flag *options_flags, char *data)
{
  if (data) {
      options_flags->verbose = 0;
  }
  options_flags->verbose = 1;
  return 0;
}

int flag_size(t_options_flag *options_flags, char *data)
{
  int size;

  size = atoi(data);
  if (size > 5)
    options_flags->size = atoi(data);
  return 0;
}

int flag_rep_port(t_options_flag *options_flags, char *data)
{
  options_flags->rep_port = data;
  return 0;
}

int flag_pub_port(t_options_flag *options_flags, char *data)
{
  options_flags->pub_port = data;
  return 0;
}

int flag_cycle(t_options_flag *options_flags, char *data)
{
  int duration;

  duration = atoi(data);
  if (duration > 500000) {
    options_flags->cycle = atoi(data);
  }
  return 0;
}
