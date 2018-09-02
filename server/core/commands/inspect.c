#include "commands.h"

int check_valid_inspect(t_thread_options *options, t_player *player, t_player *target, char *name)
{
	if (options->gameinfo->game_status != 1)
		return 0;
	if (name == NULL
		|| strlen(name) < 1)
		return 0;
	if (player == NULL
		|| player->stunned > 0 
		|| player->turn_over == 1 
		|| player->actionpoint < 1 
		|| player->energy <= 0)
		return 0;
	if (target == NULL)
		return 0;
	return 1;
}

int command_inspect(zframe_t *identity, char *name, t_thread_options *options, zsock_t *router)
{
	t_player *player;
	t_player *target;
	char *str;
	char player_str[1024];
	char target_str[1024];

	pthread_mutex_lock(options->mutex);
	player = findPlayerByIdentity(options->gameinfo, identity);
	target = findPlayerByName(options->gameinfo, name);
	str_player_player(player, player_str);
	if (check_valid_inspect(options, player, target, name) == 1)
	{
		str_player_player(target, target_str);
		log_trace("[COMMAND_INSPECT] %s inspecting %s", player_str, target_str);

		player->actionpoint -= 1;
		str = malloc(sizeof(char) * 5);
		sprintf(str, "ok|%d", target->energy);
		send_response(router, player->identity, zframe_new(str, strlen(str)));
		free(str);
	}
	else {
		log_warn("[COMMAND_INSPECT] %s can't inspect %s now", player_str, name);
		send_response_empty_ko(router, identity);
	}
	pthread_mutex_unlock(options->mutex);
	return 1;
}