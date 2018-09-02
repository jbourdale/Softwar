#include "commands.h"

int check_valid_watch(t_thread_options *options, t_player *player)
{
	if (options->gameinfo->game_status != 1)
		return 0;
	if (player == NULL)
		return 0;

	if (player->stunned > 0 
		|| player->turn_over == 1 
		|| player->actionpoint < 2 
		|| player->energy <= 0)
		return 0;
	
	return 1;
}

int command_watch(zframe_t *identity, char *data, t_thread_options *options, zsock_t *router)
{
	const int relative_coords[4][4][2] = {
		{{-1, 0},{-2,1},{-2,0},{-2,-1}},
		{{0,-1},{-1,-2},{0,-2},{1,-2}},
		{{1,0},{2,-1},{2,0},{2,1}},
		{{0,1},{1,2},{0,2},{-1,2}}
	};

	(void) data;
	t_player *player, *playerFound;
	t_energycell *cell;
	char **result;
	char *response;
	int i, x, y;
	char player_str[1024];

	pthread_mutex_lock(options->mutex);
	player = findPlayerByIdentity(options->gameinfo, identity);
	str_player_player(player, player_str);
	log_debug("[WATCH] 1");
	if (check_valid_watch(options, player) == 1)
	{
		log_debug("[WATCH] 2");
		result = malloc(sizeof(char *) * 4);
		for (i = 0; i<4; i++)
		{
			log_debug("[WATCH] 3.%d", i);
			x = player->x + relative_coords[player->looking][i][0];
			y = player->y + relative_coords[player->looking][i][1];
			if ((x < 0 || x >= options->gameinfo->map_size) || (y < 0 || y >= options->gameinfo->map_size))
				result[i] = strdup("out");
			else if (is_pos_empty(options->gameinfo, x, y))
				result[i] = strdup("empty");
			else if ((playerFound = findPlayerByPos(options->gameinfo, x, y)) != NULL)
				result[i] = strdup(playerFound->name);
			else if ((cell = findCellByPos(options->gameinfo, x, y)) != NULL)
				result[i] = strdup("energy");
			log_debug("[WATCH] 4.%d", i);
		}
		response = malloc(1024);
		log_debug("[WATCH] 5");
		sprintf(response,"ok|[\"%s\",\"%s\",\"%s\",\"%s\"]", result[0],result[1],result[2],result[3]);
		log_debug("[WATCH] 6");

		log_trace("[COMMAND_WATCH] Sending his vision (%s) to %s", response, player_str);
		send_response(router, player->identity, zframe_new(response, strlen(response)));
		free(response);
	}
	else {
		log_warn("[COMMAND_WATCH] Can't get vision for %s now", player_str);
		send_response_empty_ko(router, identity);
	}

	pthread_mutex_unlock(options->mutex);
	return 1;
}