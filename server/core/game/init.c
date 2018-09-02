#include "game.h"


void init_player(t_player *player, t_gameinfo *gameinfo, char *name)
{
	int i = 0;

	while (i < 4 && gameinfo->players[i] != NULL)
		i++;

	player->x = 0;
	player->y = 0;
	if (i == 1)
	{
		player->x = 0;
		player->y = 0;
	}
	if (i == 2)
		player->x = gameinfo->map_size -1;
	else if (i == 3)
		player->y = gameinfo->map_size -1;
	else if (i == 4) 
	{
		player->x = gameinfo->map_size -1;
		player->y = gameinfo->map_size -1;
	}
	player->identity = zframe_new_empty();
	player->name = malloc(strlen(name) + 1);
	player->name = name;
	player->energy = 100;
	player->looking = 0;
	player->actionpoint = 2;
	player->stunned = 0;
	player->turn_over = 0;
}

void init_energycell(t_gameinfo *gameinfo, t_energycell *cell)
{
	int valid = 0;
	int x, y, try;

	try = 0;
	while (!valid)
	{
		try++;
		if (try >= (gameinfo->map_size * gameinfo->map_size))
			break;

		x = rand()%gameinfo->map_size;
		y = rand()%gameinfo->map_size;
		valid = is_pos_empty(gameinfo, x, y);
	}
	if (try < (gameinfo->map_size * gameinfo->map_size))
	{
		cell->x = x;
		cell->y = y;
		cell->energy = rand()%(10) + 5;
	}
	else
		log_warn("[ENERGY_CELL] NO MORE SPACE TO GENERATE A NEW ENERGY CELL");
	log_debug("[ENERGY_CELL] DONE GENERATING");
}

void init_log(t_options_flag *flags)
{
	log_set_level(SFTWR_LOG_INFO);
	if (flags->verbose == 1)
		log_set_level(SFTWR_LOG_TRACE);

	log_debug("log file path : %s", flags->log);
	if (flags->log != NULL && strlen(flags->log) > 0)
	{
		flags->log_file = NULL;
		flags->log_file = fopen(flags->log, "w+");
		if (flags->log_file != NULL)
		{
			log_debug("Log set fp");
			log_set_fp(flags->log_file);
		}
		else
			log_error("Can't open log file %s", flags->log);
	}
}

void init_game(t_gameinfo *game, t_options_flag *flags)
{
	int i;

	game->map_size = flags->size;
	game->game_status = 0;
	game->players = malloc(sizeof(t_player) * 4);
	for (i=0; i<4; i++)
	{
		game->players[i] = NULL;
	}
	game->energy_cells = malloc(sizeof(t_energycell) * (game->map_size * game->map_size));
	for (i=0; i<(game->map_size * game->map_size); i++)
	{
		game->energy_cells[i] = malloc(sizeof(t_energycell));
		game->energy_cells[i] = NULL;
	}
	init_log(flags);

	banner();
	log_game_start(game, flags);
}