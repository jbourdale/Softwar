#include "commands.h"

int check_valid_next(t_thread_options *options)
{
	if (options->gameinfo->game_status != 1)
		return 0;
	return 1;
}

int command_next_turn(zframe_t *identity, char *data, t_thread_options *options, zsock_t *router)
{
	(void) data;
	t_player *player;
	char player_str[1024];

	pthread_mutex_lock(options->mutex);
	player = findPlayerByIdentity(options->gameinfo, identity);
	str_player_player(player, player_str);
	if (check_valid_next(options) == 1)
	{
		player->turn_over = 1;
		log_trace("[COMMAND_NEXT_TURN] %s turn is now over", player_str);
		send_response_empty_ok(router, player->identity);
	}
	else {
		log_warn("[COMMAND_NEXT_TURN] %s can't end his turn now", player_str);
		send_response_empty_ko(router, identity);
	}

	pthread_mutex_unlock(options->mutex);
	return 1;
}