#ifndef PLAYER_H
#define PLAYER_H

#include "softwar_server.h"
#include "core/log/log.h"

t_player *findPlayerByIdentity(t_gameinfo *gameinfo, zframe_t *identity);
t_player *findPlayerByPos(t_gameinfo *gameinfo, int x, int y);
t_player *findPlayerByName(t_gameinfo *gameinfo, char *name);

void str_player(t_gameinfo *gameinfo, zframe_t *identity, char *str);
void str_player_player(t_player *player, char *str);

#endif