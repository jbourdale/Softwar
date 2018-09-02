#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include "softwar_server.h"
#include "core/game/game.h"
#include <time.h>
#include <jansson.h>

void *launch_publisher(void *args);
zsock_t *init_publisher(char *port);
void wait_start_game(zsock_t *publisher, t_thread_options *options);
json_t *json_encode_player(t_player *player);
json_t *json_encode_cell(t_energycell *cell);
json_t *json_encode_gameinfo(t_gameinfo *gameinfo);
json_t *json_encode_players(t_player **players);
json_t *build_notification(int notification_type, json_t *data);

// notifications
void notification_cycle_info(zsock_t *publisher, t_gameinfo *gameinfo);
void notification_game_start(zsock_t *publisher);
void notification_game_end(zsock_t *publisher);
void notification_dead(zsock_t *publisher, t_player *player);
void notification_win(zsock_t *publisher, t_player *player);
void notification_player_connected(zsock_t *publisher, t_player **players, int map_size);

#endif