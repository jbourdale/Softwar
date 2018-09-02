#include "energycell.h"

void pop_cell(t_energycell **array, t_energycell *cell, int map_size)
{
	int i;
	int index;

	i=0;
	while (array[i] != cell)
		i++;
	index = i;
	i=0;
	for(i = index; i < (map_size * map_size) - 1; i++) array[i] = array[i + 1];
}

t_energycell *findCellByPos(t_gameinfo *gameinfo, int x, int y)
{
	int i;

	i = 0;
	while (gameinfo->energy_cells[i] != NULL)
	{
		if (gameinfo->energy_cells[i]->x == x && gameinfo->energy_cells[i]->y == y)
		{
			return gameinfo->energy_cells[i];
		}
		i++;
	}
	return NULL;
}