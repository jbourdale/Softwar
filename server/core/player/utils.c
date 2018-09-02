#include "player.h"

t_player *findPlayerByIdentity(t_gameinfo *gameinfo, zframe_t *identity)
{
	int i;
	char *playerIdentity;

	playerIdentity = zframe_strhex(identity);
	i=0;
	while (i<4 
		&& gameinfo->players[i] != NULL
		&& strcmp(zframe_strhex(gameinfo->players[i]->identity), playerIdentity) != 0
	)
		i++;

	if (i == 4)
		return NULL;
	return gameinfo->players[i];
}

t_player *findPlayerByPos(t_gameinfo *gameinfo, int x, int y)
{
	int i;

	for (i=0; i<4; i++)
	{
		if (gameinfo->players[i]->x == x && gameinfo->players[i]->y == y)
		{
			return gameinfo->players[i];
		}
	}
	return NULL;
}

t_player *findPlayerByName(t_gameinfo *gameinfo, char *name)
{
	int i;

	for (i=0; i<4; i++)
	{
		if (strcmp(gameinfo->players[i]->name, name) == 0)
			return gameinfo->players[i];
	}
	return NULL;
}

void str_player(t_gameinfo *gameinfo, zframe_t *identity, char *str)
{
	t_player *player;


	if((player = findPlayerByIdentity(gameinfo, identity)) != NULL)
		sprintf(str, "Player{ identity: %s, name : %s }", zframe_strhex(identity), player->name);
	else
		sprintf(str, "Player{ identity: %s, name : %s }", zframe_strhex(identity), "NULL");
}

void str_player_player(t_player *player, char *str)
{
	if(player != NULL)
		sprintf(str, "Player{ identity: %s, name : %s }", zframe_strhex(player->identity), player->name);
	else
		sprintf(str, "Player{ identity: %s, name : %s }", "NULL", "NULL");
}