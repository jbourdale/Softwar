#include "commands.h"

void find_targets(t_gameinfo *gameinfo, t_player *player, t_player **targets)
{
	int nbTarget;
	int i, x, y;
	t_player *playerFound;

	const int relative_coords[4][4][2] = {
		{{-1, 0},{-2,1},{-2,0},{-2,-1}},
		{{0,-1},{-1,-2},{0,-2},{1,-2}},
		{{1,0},{2,-1},{2,0},{2,1}},
		{{0,1},{1,2},{0,2},{-1,2}}
	};

	nbTarget = 0;
	for (i = 0; i<4; i++)
	{
		targets[i] = NULL;
		x = player->x + relative_coords[player->looking][i][0];
		y = player->y + relative_coords[player->looking][i][1];
		if ((playerFound = findPlayerByPos(gameinfo, x, y)) != NULL)
		{
			targets[nbTarget] = playerFound;
			nbTarget++;
		}
	}
}

int check_valid_attack(t_thread_options *options, t_player *player)
{
	if (options->gameinfo->game_status != 1)
		return 0;
	if (player == NULL
		|| player->stunned > 0 
		|| player->turn_over == 1 
		|| player->actionpoint < 1 
		|| player->energy < 0)
		return 0;
	return 1;
}

int command_attack(zframe_t *identity, char *data, t_thread_options *options, zsock_t *router)
{
	(void) data;
	t_player *player;
	t_player **targets;
	int i;
	char player_str[1024];

	pthread_mutex_lock(options->mutex);
	player = findPlayerByIdentity(options->gameinfo, identity);
	str_player_player(player, player_str);
	if (check_valid_attack(options, player) == 1)
	{
		log_trace("[COMMAND_ATTACK] %s is attacking", player_str);
		log_trace("[COMMAND_ATTACK] %s spend 0.5 actionpoint and 1 energy point", player_str);

		targets = malloc(sizeof(t_player *) * 4);
		find_targets(options->gameinfo, player, targets);
		i = 0;
		while (i < 4 && targets[i] != NULL)
		{
			str_player_player(targets[i], player_str);
			log_trace("[COMMAND_ATTACK] %s have been hit, he is now stunned for 2 cycles", player_str);
			
			targets[i]->actionpoint = 0;
			targets[i]->stunned = 2;
			i++;
		}

		player->actionpoint -= 1;
		player->energy -= 1;
		send_response_empty_ok(router, player->identity);
		free(targets);
	}
	// not enought AP
	else 
	{
		log_warn("[COMMAND_ATTACK] %s don't have enought AP to attack", player_str);
		send_response_empty_ko(router, identity);	
	}
	pthread_mutex_unlock(options->mutex);
	return 1;
}