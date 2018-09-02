#include "commands.h"

int command_self(zframe_t *identity, char *data, t_thread_options *options, zsock_t *router)
{
	(void) data;
	t_player *player;
	char *str;
	char player_str[1024];

	pthread_mutex_lock(options->mutex);
	player = findPlayerByIdentity(options->gameinfo, identity);
	str_player_player(player, player_str);
	if (player != NULL)
	{
		str = malloc(sizeof(char) * strlen(player->name) + 3);
		sprintf(str, "ok|%s", player->name);
		log_trace("[COMMAND_SELFID] Sending id %s to %s", player->name, player_str);
		send_response(router, player->identity, zframe_new(str, strlen(str)));
	}
	else {
		log_warn("[COMMAND_SELFID] Can't find id for %s", player_str);
		send_response_empty_ko(router, identity);
	}
	pthread_mutex_unlock(options->mutex);
	return 1;
}