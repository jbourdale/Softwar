#include "commands.h"

int check_valid_looking(t_thread_options *options, int orientation, t_player *player, char *data)
{
	if (options->gameinfo->game_status != 1)
		return 0;
	if (player == NULL
		|| player->stunned > 0 
		|| player->turn_over == 1 
		|| player->actionpoint < 1 
		|| player->energy <= 0)
		return 0;
	if (data == NULL 
		|| data[0] != '0'
		|| orientation == 0
		|| orientation < 0
		|| orientation > 0)
		return 0;
	return 1;
}

int command_looking(zframe_t *identity, char *data, t_thread_options *options, zsock_t *router)
{
	(void) data;
	t_player *player;
	int orientation;
	char player_str[1024];

	pthread_mutex_lock(options->mutex);
	player = findPlayerByIdentity(options->gameinfo, identity);
	str_player_player(player, player_str);
	orientation = atoi(data);
	if (check_valid_looking(options, orientation, player, data) == 1)
	{
		player->looking = orientation;
		log_trace("[COMMAND_LOOKING] %s is now looking %d", player_str, orientation);
		send_response_empty_ok(router, player->identity);
	}
	else {
		log_warn("[COMMAND_LOOKING] %s can't update his orientation now", player_str);
		send_response_empty_ko(router, identity);	
	}

	pthread_mutex_unlock(options->mutex);

	return 0;
}