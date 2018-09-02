#include "commands.h"

void findCoordJump(int *pos, t_player player)
{
	switch(player.looking)
	{
		case 0:
			pos[0] = player.x + 2;
			pos[1] = player.y;
			break;
		case 1:
			pos[0] = player.x;
			pos[1] = player.y - 2;
			break;
		case 2:
			pos[0] = player.x - 2;
			pos[1] = player.y;
			break;
		case 3:
			pos[0] = player.x;
			pos[1] = player.y + 2;
			break;
	}
}

int check_valid_jump(t_thread_options *options, t_player *player)
{
	if (options->gameinfo->game_status != 1)
		return 0;
	if (player == NULL
		|| player->stunned > 0 
		|| player->turn_over == 1 
		|| player->energy < 2)
		return 0;
	return 1;
}

int command_jump(zframe_t *identity, char *data, t_thread_options *options, zsock_t *router)
{
	(void) data;
	t_player *player;
	int *pos;
	char player_str[1024];

	pthread_mutex_lock(options->mutex);
	player = findPlayerByIdentity(options->gameinfo, identity);
	str_player_player(player, player_str);
	if (check_valid_jump(options, player) == 1)
	{
		pos = malloc(sizeof(int) * 2);
		findCoordJump(pos, *player);

		if (is_pos_reachable(options->gameinfo, pos[0], pos[1]))
		{
			log_trace("[COMMAND_INSPECT] %s jumping to (%d, %d)", player_str, pos[0], pos[1]);
			player->x = pos[0];
			player->y = pos[1];
			player->energy -= 2;
			send_response_empty_ok(router, player->identity);
		}
		else {
			log_warn("[COMMAND_INSPECT] %s can't jump to (%d,%d) (unreachable)", player_str, pos[0], pos[1]);
			send_response_empty_ko(router, player->identity);
		}
		free(pos);
	}
	else {
		log_warn("[COMMAND_INSPECT] %s can't jump now", player_str);	
		send_response_empty_ko(router, identity);
	}
	pthread_mutex_unlock(options->mutex);
	return 1;
}