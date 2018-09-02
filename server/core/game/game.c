#include "game.h"

int game_ended(t_thread_options *options)
{
	int i;
	int nbAlive;

	pthread_mutex_lock(options->mutex);
	nbAlive = 0;
	for (i=0; i<4; i++)
	{
		if (options->gameinfo->players[i]->energy > 0)
			nbAlive++;
	}
	pthread_mutex_unlock(options->mutex);

	if(nbAlive >= 2)
		return 0;
	log_info("[GAME] GAME IS OVER, ONLY %d SURVIVORS LEFT", nbAlive);
	return 1;
}

int launch_cycle(zsock_t *publisher, t_gameinfo *gameinfo)
{
	int i;
	int nbDead;

	nbDead = 0;
	// update players
	for (i=0; i<4; i++)
	{
		log_debug("[NOTIFICATION] RESETING ALL PLAYERS");
		gameinfo->players[i]->energy -= 2;
		gameinfo->players[i]->actionpoint = 2;
		if (gameinfo->players[i]->stunned > 0)
			gameinfo->players[i]->stunned -= 1;
		
		if (gameinfo->players[i]->energy <= 0)
		{
			nbDead++;
			notification_dead(publisher, gameinfo->players[i]);
		}
	}
	log_debug("[NOTIFICATION] GENERATE ENERGY CELL");
	// generate energy cell
	i = 0;
	while (gameinfo->energy_cells[i] != NULL)
		i++;
	log_debug("[ENERGY_CELL] GENERATING THE %d ENERGY CELL", i);
	if (i < gameinfo->map_size * gameinfo->map_size)
	{
		gameinfo->energy_cells[i] = malloc(sizeof(t_energycell));	
		init_energycell(gameinfo, gameinfo->energy_cells[i]);	
	}
	
	// notify
	log_debug("[NOTIFICATION] SENDING CYCLE INFO NOTIFICATION");
	notification_cycle_info(publisher, gameinfo);
	
	if (nbDead >= 3)
		return 1;
	return 0;
}

void end_game(zsock_t *publisher, t_gameinfo *gameinfo)
{
	int i;

	notification_game_end(publisher);
	for (i=0; i<4; i++)
	{
		if (gameinfo->players[i]->energy > 0)
			notification_win(publisher, gameinfo->players[i]);
	}
}

void start_game(t_thread_options *options)
{
	pthread_mutex_lock(options->mutex);
	options->gameinfo->game_status = 1;
	log_info("[GAME] 4 players connected, game is about to start");
	log_info("[GAME] Launching game...");
	// pthread_cond_signal(options->condition);
	pthread_mutex_unlock(options->mutex);
}