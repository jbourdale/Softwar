#ifndef CORE_H
#define CORE_H

#include <czmq.h>

typedef struct 	s_energycell {
	int 		x;
	int 		y;
	int 		energy;
} t_energycell;

typedef struct 	s_player {
	zframe_t 	*identity;
	char 		*name;
	int 		x;
	int 		y;
	int 		energy;
	int 		stunned;
	int 		turn_over;
	int 		looking; //(left = 0, up = 1, right = 2, down = 3)
	int 		actionpoint; // Action point
} t_player;

typedef struct 		s_gameinfo {
	int 			map_size;
	int 			game_status; // 0 => waiting, 1 => started, 2 => finished
	t_player		**players;
	t_energycell	**energy_cells;
} t_gameinfo;

#endif