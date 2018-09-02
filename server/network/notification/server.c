// server.c for softwar server
// 
// Made by BOURDALE Jules
// Login   <bourda_j@etna-alternance.net>
// 
// Started on  Fri Avr 29 14:56:57 2018 BOURDALE Jules
//
#include "notification.h"

zsock_t *init_publisher(char *port)
{
	zsock_t *publisher = zsock_new(ZMQ_PUB);
	if (zsock_bind(publisher, "tcp://*:%s", port) != 0)
		return publisher;
	return NULL;
}

void wait_start_game(zsock_t *publisher, t_thread_options *options)
{
	int i;
	int j;

	pthread_mutex_lock(options->mutex);
	for (i=0; i<4; i++)
	{	
		pthread_cond_wait(options->condition, options->mutex);
		log_error("[NOTIFICATION] GOT PLAYER CONNECTED SIGNAL");
		j = 0;
		while(options->gameinfo->players[j] != NULL)
			j++;
		log_debug("FOUNDED THAT PLAYER N°%d JUST CONNECT FOR i = %d", j-1, i);
		notification_player_connected(publisher, options->gameinfo->players, options->gameinfo->map_size);
	}
	notification_game_start(publisher);
	pthread_mutex_unlock(options->mutex);
}

void notify_players(zsock_t *publisher, t_gameinfo *gameinfo)
{
	int i;
	int nbDead;

	nbDead = 0;
	for (i=0; i<4; i++)
	{
		gameinfo->players[i]->energy -= 2;
		if (gameinfo->players[i]->energy <= 0)
		{
			nbDead++;
			notification_dead(publisher, gameinfo->players[i]);
		}
	}
	if (nbDead >= 3)
		end_game(publisher, gameinfo);
}

void *launch_publisher(void *args)
{
	zsock_t *publisher;
	int finished;
	t_thread_options *options;

	options = (t_thread_options *)args;
	log_info("[PUBLISHER] Launching server on port %s...", options->flags->pub_port);
	if ((publisher = init_publisher(options->flags->pub_port)) == NULL) {
		log_error("[PUBLISHER] Failed to launch server");
		return (void *) FAILED;
	}
	log_info("[PUBLISHER] Server launched successfuly");

	wait_start_game(publisher, options);
	notification_cycle_info(publisher, options->gameinfo);

	finished = 0;
	while (!zsys_interrupted && finished == 0)
	{
		usleep(options->flags->cycle);
		log_info("[PUBLISHER] Starting new cycle");

		log_debug("[NOTIFICATION] LOCKING MUTEX");
		pthread_mutex_lock(options->mutex);
		log_debug("[NOTIFICATION] ON VA LANCER LE CYCLE LA");
		finished = launch_cycle(publisher, options->gameinfo);
		pthread_mutex_unlock(options->mutex);
	}
	pthread_mutex_lock(options->mutex);
	end_game(publisher, options->gameinfo);
	pthread_mutex_unlock(options->mutex);
	
	zsock_destroy(&publisher);
	return (void *) SUCCESS;
}