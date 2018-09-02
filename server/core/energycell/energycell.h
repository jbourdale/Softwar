#ifndef ENERGY_CELL_H
#define ENERGY_CELL_H

#include "softwar_server.h"

t_energycell *findCellByPos(t_gameinfo *gameinfo, int x, int y);
void pop_cell(t_energycell **array, t_energycell *cell, int map_size);

#endif