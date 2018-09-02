#include "notification.h"
#include "core/player/player.h"

void notification_cycle_info(zsock_t *publisher, t_gameinfo *gameinfo)
{
	char *chanel;

	chanel = strdup("#global");
	json_t *data = json_encode_gameinfo(gameinfo);
	json_t *notification = build_notification(0, data);
	log_trace("[PUBLISHER] Publishing cycle info notification");
    zstr_sendf(publisher, "%s|%s", chanel, json_dumps(notification, JSON_INDENT(4)));
}

void notification_game_start(zsock_t *publisher)
{
	char *chanel;

	chanel = strdup("#global");
	json_t *notification = build_notification(1, NULL);
	log_trace("[PUBLISHER] Publishing game start notification");
    zstr_sendf(publisher, "%s|%s", chanel, json_dumps(notification, JSON_INDENT(4)));
}

void notification_game_end(zsock_t *publisher)
{
	char *chanel;

	chanel = strdup("#global");
	json_t *notification = build_notification(2, NULL);
	log_trace("[PUBLISHER] Publishing game's over notification");
    zstr_sendf(publisher, "%s|%s", chanel, json_dumps(notification, JSON_INDENT(4)));
}

void notification_dead(zsock_t *publisher, t_player *player)
{
	char player_str[1024];
	str_player_player(player, player_str);

	json_t *notification = build_notification(3, NULL);
	log_trace("[PUBLISHER] Publishing that %s is dead notification", player_str);
    zstr_sendf(publisher, "%s|%s", player->name, json_dumps(notification, JSON_INDENT(4)));
}

void notification_win(zsock_t *publisher, t_player *player)
{
	char player_str[1024];
	str_player_player(player, player_str);

	json_t *notification = build_notification(4, NULL);
	log_trace("[PUBLISHER] Publishing that %s won the game notification", player_str);
    zstr_sendf(publisher, "%s|%s", player->name, json_dumps(notification, JSON_INDENT(4)));
}

void notification_player_connected(zsock_t *publisher, t_player **players, int map_size)
{
	char *chanel;
	char player_str[1024];
	int i;

	chanel = strdup("#global");
	i = 0;
	while (players[i] != NULL)
		i++;
	str_player_player(players[i-1], player_str);

	json_t *json_notification = json_object();
	json_object_set_new(json_notification, "map_size", json_integer(map_size));
	json_t *json_players = json_encode_players(players);
	json_object_set_new(json_notification, "players", json_players);
	json_t *notification = build_notification(5, json_notification);
	log_trace("[PUBLISHER] Publishing that %s has just connect", player_str);
	zstr_sendf(publisher, "%s|%s", chanel, json_dumps(notification, JSON_INDENT(4)));
}



