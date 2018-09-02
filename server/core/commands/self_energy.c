#include "commands.h"

int check_valid_self_energy(t_thread_options *options)
{
	if (options->gameinfo->game_status != 1)
		return 0;
	return 1;
}

int command_self_energy(zframe_t *identity, char *data, t_thread_options *options, zsock_t *router)
{
	(void) data;
	t_player *player;
	char *str;
	char player_str[1024];

	pthread_mutex_lock(options->mutex);
	player = findPlayerByIdentity(options->gameinfo, identity);
	str_player_player(player, player_str);
	if (check_valid_self_energy(options) == 1)
	{
		str = malloc(sizeof(char) * 6);
		sprintf(str, "ok|%d", player->energy);
		log_trace("[COMMAND_SELF_ENERGY] Sending his energy count (%d) to %s", player->energy, player_str);

		send_response(router, player->identity, zframe_new(str, strlen(str)));
		free(str);
	}
	else {
		log_warn("[COMMAND_SELF_ENERGY] Can't find energy for %s", player_str);
		send_response_empty_ko(router, identity);
	}

	pthread_mutex_unlock(options->mutex);
	return 1;
}