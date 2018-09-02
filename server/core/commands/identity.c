#include "commands.h"

int name_already_taken(char *name, t_thread_options *options)
{
	int i;

	i = 0;
	while (options->gameinfo->players[i] != NULL && strcmp(options->gameinfo->players[i]->name, name) != 0)
		i++;
	if (options->gameinfo->players[i] == NULL)
		return 0;
	return 1;
}

int command_identity(zframe_t *identity, char *name, t_thread_options *options, zsock_t *router)
{
	t_player *player;
	int i;

	pthread_mutex_lock(options->mutex);
	if (options->gameinfo->game_status != 0) {
		log_error("[COMMAND_IDENTIFY] Can't connect a new player, game already started");
		send_response(router, identity, zframe_new("ko|game full", 12));
	}
	else if (name == NULL) {
		log_warn("[COMMAND_IDENTIFY] [BAD_CALL] Connexion request came without name");
		send_response_empty_ko(router, identity);
	}
	else if (strlen(name) <= 1) {
		log_warn("[COMMAND_IDENTIFY] [BAD_CALL] Connexion request came with an empty name");
		send_response_empty_ko(router, identity);
	}
	else if (name_already_taken(name, options) > 0) {
		log_error("[COMMAND_IDENTIFY] Name %s is already used by someone else", name);
		send_response(router, identity, zframe_new("ko|identity already exists", 26));
	}
	else
	{
		player = findPlayerByIdentity(options->gameinfo, identity);
		if (player != NULL)
		{
			log_info("[COMMAND_IDENTIFY] Renamed player %s to %s \n", player->name, name);
			player->name = name;
			send_response_empty_ok(router, player->identity);
		}
		else
		{
			i = 0;
			while (options->gameinfo->players[i] != NULL)
				i++;
			options->gameinfo->players[i] = malloc(sizeof(t_player));
			init_player(options->gameinfo->players[i], options->gameinfo, name);
			options->gameinfo->players[i]->identity = zframe_dup(identity);
			send_response_empty_ok(router, options->gameinfo->players[i]->identity);
			
			char player_str[1024];
			str_player(options->gameinfo, identity, player_str);
			log_info("[COMMAND_IDENTIFY] New player connected! %s", player_str);
			sleep(1);
			log_error("[COMMAND_IDENTIFY] SENDING SIGNAL");
			pthread_cond_signal(options->condition);
		}
	}
	pthread_mutex_unlock(options->mutex);
	return 1;
}