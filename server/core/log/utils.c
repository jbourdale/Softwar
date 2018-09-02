#include "log.h"

void log_game_start(t_gameinfo *game, t_options_flag *flags)
{
	log_trace("Environment : ");
	log_trace("\t Router server port :  %s", flags->rep_port);
	log_trace("\t Publisher server port :  %s", flags->pub_port);
	log_trace("\t Map size :  %d", game->map_size);
	log_trace("\t Cycle duration : %d(µs)", flags->cycle);
	if (flags->verbose == 1)
		log_trace("\t Verbose :  ON");
	else
		log_trace("\t Verbose :  OFF");
	if (flags->log != NULL && strlen(flags->log) > 0)
		log_trace("\t Logs will be write in file %s", flags->log);
}