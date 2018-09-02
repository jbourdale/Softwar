#include "notification.h"

json_t *json_encode_players(t_player **players)
{
	int i;

	json_t *player_arr = json_array();
	for (i=0; i<4; i++)
	{
		if (players[i] != NULL)
			json_array_append_new(player_arr, json_encode_player(players[i]));
	}
	return player_arr;
}

json_t *json_encode_player(t_player *player)
{
    json_t *json_player = json_object();

    if (player != NULL)
    {
	    json_object_set_new(json_player, "name", json_string(player->name));
    	json_object_set_new(json_player, "x", json_integer(player->x));
    	json_object_set_new(json_player, "y", json_integer(player->y));
    	json_object_set_new(json_player, "energy", json_integer(player->energy));
    	json_object_set_new(json_player, "looking", json_integer(player->looking));
    	json_object_set_new(json_player, "actionpoint", json_integer(player->actionpoint));
    	json_object_set_new(json_player, "stunned", json_integer(player->stunned));
    }
    return json_player;
}

json_t *json_encode_cell(t_energycell *cell)
{
    if (cell != NULL)
    {
    	json_t *json_cell = json_object();
	    json_object_set_new(json_cell, "x", json_integer(cell->x));
	    json_object_set_new(json_cell, "y", json_integer(cell->y));
	    json_object_set_new(json_cell, "energy", json_integer(cell->energy));
	    return json_cell;
    }
    return NULL;
}

json_t *json_encode_gameinfo(t_gameinfo *gameinfo)
{
	int i;
	json_t *json_gameinfo = json_object();
	json_t *player_arr = json_array();
	json_t *map_arr = json_array();

	json_object_set_new(json_gameinfo, "map_size", json_integer(gameinfo->map_size));
	json_object_set_new(json_gameinfo, "game_status", json_integer(gameinfo->game_status));

	for (i=0; i<4; i++)
	{
		json_array_append_new(player_arr, json_encode_player(gameinfo->players[i]));
	}
	json_object_set_new(json_gameinfo, "players", player_arr);
	for (i=0; i<(gameinfo->map_size * gameinfo->map_size); i++)
	{
		json_array_append_new(map_arr, json_encode_cell(gameinfo->energy_cells[i]));
	}
	json_object_set_new(json_gameinfo, "energy_cells", map_arr);
	return json_gameinfo;
}

json_t *build_notification(int notification_type, json_t *data)
{
	json_t *json = json_object();

	json_object_set_new(json, "notification_type", json_integer(notification_type));
	json_object_set_new(json, "data", data);

	return json;
}