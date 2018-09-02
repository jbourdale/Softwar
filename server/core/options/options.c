#include "options.h"
#include "core/log/log.h"
#include "../../utils/flags.h"

static struct option long_options[] =
{
    {"rep-port", required_argument, NULL, 'r'},
		{"pub-port", required_argument, NULL, 'p'},
		{"cycle", required_argument, NULL, 'c'},
		{"verbose", no_argument, NULL, 'v'},
    {"log", required_argument, NULL, 'l'},
		{"size", required_argument, NULL, 's'},
    {NULL, 0, NULL, 0}
};

static const t_flag_setter flag_setter_options[] =
{
    {'l', flag_log},
		{'v', flag_verbose},
		{'s', flag_size},
		{'r', flag_rep_port},
    {'p', flag_pub_port},
		{'c', flag_cycle},
    {0, 0}
};

void usage()
{
  log_error("Usage : ");
  log_error("\t--rep-port [port utilisé pour recevoir les commandes des clients et répondre]");
  log_error("\t--pub-port [port utilisé pour envoyer les notifications aux clients]");
  log_error("\t--cycle [nombre en microsecondes correspondant à un cycle (> 0)]");
  log_error("\t--v (active le mode verbose (loglevel INFO))");
  log_error("\t--log [fichier de log]");
  log_error("\t--size [taille de la map]");
}

t_options_flag *init_options_flags() {
  t_options_flag *flags = malloc(sizeof *flags);

  if (!flags) {
    log_fatal("System error, please launch binary again");
    return NULL;
  }

  return flags;
}

void set_default_options(t_options_flag *flags) {
  flags->log = NULL;
  flags->rep_port = "4242";
  flags->pub_port = "4243";
  flags->cycle = 500000;
  flags->verbose = 0;
  flags->size = 5;
}

int call_flag_setter(t_options_flag *options_flags, char option, char *data)
{
  int i;

	i = 0;
	while (flag_setter_options[i].option != 0)
	{
    if (option == flag_setter_options[i].option)
    {
      return flag_setter_options[i].call(options_flags, data);
    }
		i++;
	}
	return FAILED;
}

int manage_options(int argc, char **argv, t_options_flag *flags)
{
  int         c;
  extern char *optarg;
  extern int  optind, opterr;

  set_default_options(flags);
  opterr = 0;
  while ((c = getopt_long_only(argc , argv, "r:p:c:vl:s:", long_options, NULL)) != -1)
	{
    switch (c) {
      case '?':
        log_fatal("Bad arguments");
        return -1;
      default:
        if (call_flag_setter(flags, c, optarg) == FAILED) {
          log_fatal("Bad arguments");
          return -1;
        }
        break;
    }
	}

  return (0);
}
