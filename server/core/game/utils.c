#include "game.h"

int game_startable(t_thread_options *options)
{
	int i;

	pthread_mutex_lock(options->mutex);
	i = 0;
	while (i < 4 && options->gameinfo->players[i] != NULL)
		i++;
	pthread_mutex_unlock(options->mutex);
	if (i == 4)
		return 1;
	return 0;
}

int is_pos_empty(t_gameinfo *gameinfo, int x, int y)
{
	int i;

	if (gameinfo->energy_cells != NULL)
	{
		i = 0;
		while (gameinfo->energy_cells[i] != NULL)
		{
			if (gameinfo->energy_cells[i]->x == x && gameinfo->energy_cells[i]->y == y)
				return 0;
			i++;
		}
	}
	
	if (gameinfo->players != NULL)
	{
		i = 0;
		while (gameinfo->players[i] != NULL)
		{
			if (gameinfo->players[i]->x == x && gameinfo->players[i]->y == y && gameinfo->players[i]->energy > 0)
				return 0;
			i++;
		}
	} 

	return 1;
}

int is_pos_reachable(t_gameinfo *gameinfo, int x, int y)
{
	int i;

	if (x >= 0 && x < gameinfo->map_size && y >= 0 && y < gameinfo->map_size)
	{
		if (gameinfo->players != NULL)
		{
			i = 0;
			while (gameinfo->players[i] != NULL)
			{
				if (gameinfo->players[i]->x == x && gameinfo->players[i]->y == y && gameinfo->players[i]->energy > 0) {
					log_error("[IS_POS_REACHABLE] CAN'T REACH THIS POS (%d, %d), PLAYER OVER HERE", x, y);
					return 0;
				}
				i++;
			}
		} 
		return 1;
	}
	log_error("[IS_POS_REACHABLE] CAN'T REACH THIS POS (%d, %d), OVER MAP", x, y);
	return 0;
}