#include "commands.h"

int check_valid_right(t_thread_options *options, t_player *player)
{
	if (options->gameinfo->game_status != 1)
		return 0;
	if (player == NULL
		|| player->stunned > 0 
		|| player->turn_over == 1 
		|| player->actionpoint < 1 
		|| player->energy <= 0)
		return 0;
	return 1;
}

int command_right(zframe_t *identity, char *data, t_thread_options *options, zsock_t *router)
{
	(void) data;
	t_player *player;
	char player_str[1024];

	pthread_mutex_lock(options->mutex);
	player = findPlayerByIdentity(options->gameinfo, identity);
	str_player_player(player, player_str);
	if (check_valid_right(options, player) == 1)
	{
		player->looking += 1;
		if (player->looking > 3)
			player->looking = 0;
		player->actionpoint -= 1;
		log_trace("[COMMAND_RIGHT] %s is now looking right.(-0.5AP)", player_str);
		send_response_empty_ok(router, player->identity);
	}
	else {
		log_warn("[COMMAND_RIGHT] %s can't look right now", player_str);
		send_response_empty_ko(router, identity);
	}
	pthread_mutex_unlock(options->mutex);
	return 1;
}