#include "commands.h"

int check_valid_rightfwd(t_thread_options *options, t_player *player)
{
	if (options->gameinfo->game_status != 1)
		return 0;
	if (player == NULL
		|| player->stunned > 0 
		|| player->turn_over == 1 
		|| player->actionpoint < 2 
		|| player->energy <= 0)
		return 0;
	return 1;
}

int command_right_forward(zframe_t *identity, char *data, t_thread_options *options, zsock_t *router)
{
	(void) data;
	t_player *player;
	int *pos;
	int old_orientation;
	char player_str[1024];

	pthread_mutex_lock(options->mutex);
	player = findPlayerByIdentity(options->gameinfo, identity);
	str_player_player(player, player_str);
	if (check_valid_rightfwd(options, player) == 1)
	{
		// looking right
		old_orientation = player->looking;
		player->looking += 1;
		if (player->looking > 3)
			player->looking = 0;
		player->actionpoint -= 1;

		// moving forward
		pos = malloc(sizeof(int)*2);
		findCoordForward(pos, *player);
		if (is_pos_reachable(options->gameinfo, pos[0], pos[1]))
		{
			log_trace("[COMMAND_RIGHTFWD] %s moving right forward to (%d, %d)", player_str, pos[0], pos[1]);
			player->x = pos[0];
			player->y = pos[1];
			player->actionpoint -= 1;
			send_response_empty_ok(router, player->identity);
		}
		else
		{
			log_warn("[COMMAND_RIGHTFWD] %s can't move right forward to (%d, %d)", player_str, pos[0], pos[1]);
			// cancel looking right
			player->looking = old_orientation;
			player->actionpoint += 1;
			send_response_empty_ko(router, player->identity);
		}
		free(pos);
	}
	else {
		log_warn("[COMMAND_RIGHTFWD] %s can't move right forward now", player_str);
		send_response_empty_ko(router, identity);
	}
	pthread_mutex_unlock(options->mutex);
	return 1;
}