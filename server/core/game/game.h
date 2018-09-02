#ifndef GAME_H
#define GAME_H

#include "core/core.h"
#include "network/notification/notification.h"
#include "core/log/log.h"

int game_startable(t_thread_options *options);
void start_game(t_thread_options *options);

int launch_cycle(zsock_t *publisher, t_gameinfo *gameinfo);

int game_ended(t_thread_options *options);
void end_game(zsock_t *publisher, t_gameinfo *gameinfo);

void init_game(t_gameinfo *game, t_options_flag *flags);
void init_player(t_player *player, t_gameinfo *gameinfo, char *name);
void init_energycell(t_gameinfo *gameinfo, t_energycell *cell);

int is_pos_empty(t_gameinfo *gameinfo, int x, int y);
int is_pos_reachable(t_gameinfo *gameinfo, int x, int y);

#endif