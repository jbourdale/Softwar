// Makefile for softwar_server
//
// Made by BOURDALE Jules
// Login   <bourda_j@etna-alternance.net>
//
// Started on  Fri Oct 20 18:14:57 2017 BOURDALE Jules
// Last update Fri Nov 10 23:26:05 2017 BOURDALE Jules
//
#include "core/core.h"
#include "core/game/game.h"
#include "softwar_server.h"
#include "network/notification/notification.h"
#include "network/router/router.h"
#include "utils/utils.h"

int launch_servers_threads(t_options_flag *flags, t_gameinfo *gameinfo)
{
	pthread_t *threads;
	pthread_mutex_t mutex;
	pthread_cond_t cond;

	if (pthread_mutex_init(&mutex, NULL) != 0)
	{
		log_debug("Error on mutex init");
		return 1;
	}
	if (pthread_cond_init(&cond, NULL) != 0)
	{
		log_debug("Error on mutex condition init");
		return 1;
	}
	if ((threads = malloc(sizeof(pthread_t) * 2)) == NULL)
	{
		log_fatal("Can't allocate memory, malloc failed");
		return 1;
	}

	t_thread_options threads_options = { .flags = flags, .gameinfo = gameinfo, .mutex = &mutex, .condition = &cond };
	
	if (pthread_create(&threads[0], NULL, launch_publisher, &threads_options) != 0)
	{
		log_debug("Can't create publisher thread");
		return 1;
	}
	if (pthread_create(&threads[1], NULL, launch_router, &threads_options) != 0)
	{
		log_debug("Can't create router thread");
		return 1;
	}

	pthread_join(threads[0], NULL);
	pthread_join(threads[1], NULL);
	free(threads);
	return 0;
}

int main(int argc, char **argv)
{
	t_options_flag *flags;
	t_gameinfo *gameinfo;

	flags = init_options_flags();
	if (manage_options(argc, argv, flags) == -1)
		usage();
	else
	{

		gameinfo = malloc(sizeof(t_gameinfo));
		init_game(gameinfo, flags);

		if (launch_servers_threads(flags, gameinfo) > 0)
			log_error("Couldn't launch server, stopping...");

		if (flags->log_file != NULL)
			fclose(flags->log_file);
	}
}