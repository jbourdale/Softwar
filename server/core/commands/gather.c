#include "commands.h"

int check_valid_gather(t_thread_options *options, t_player *player, t_energycell *cell)
{
	if (options->gameinfo->game_status != 1)
		return 0;
	if (player == NULL
		|| player->stunned > 0
		|| player->turn_over == 1 
		|| player->actionpoint < 2 
		|| player->energy < 0)
		return 0;
	if (cell == NULL)
		return 0;
	return 1;
}

int command_gather(zframe_t *identity, char *data, t_thread_options *options, zsock_t *router)
{
	(void) data;
	t_player *player;
	t_energycell *cell;
	char player_str[1024];

	pthread_mutex_lock(options->mutex);
	player = findPlayerByIdentity(options->gameinfo, identity);
	cell = findCellByPos(options->gameinfo, player->x, player->y);
	str_player_player(player, player_str);
	log_trace("[COMMAND_GATHER] %s is trying to gather energy", player_str);
	if (check_valid_gather(options, player, cell) == 1)
	{
		log_trace("[COMMAND_GATHER] %s gather energy (+%d energy)", player_str, cell->energy);
		player->energy += cell->energy;
		pop_cell(options->gameinfo->energy_cells, cell, options->gameinfo->map_size);
		player->actionpoint -= 2;
		send_response_empty_ok(router, player->identity);
	}
	else {
		log_warn("[COMMAND_GATHER] %s can't gather energy here", player_str);
		send_response_empty_ko(router, identity);
	}
	pthread_mutex_unlock(options->mutex);
	return 1;
}