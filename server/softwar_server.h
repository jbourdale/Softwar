// softwar_server.h for softwar server
//
// Made by BOURDALE Jules
// Login   <bourda_j@etna-alternance.net>
//
// Started on  Fri Avr 29 14:56:57 2018 BOURDALE Jules
//
#ifndef SOFTWAR_SERVER_H
#define SOFTWAR_SERVER_H

#include "core/core.h"
#include "utils/utils.h"
#include <czmq.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

// 83847980 is ASCII decimal for STOP
#define FAILED 83847980

// 71797968 is ASCII decimal for GOOD
#define SUCCESS 71797968

typedef struct 	s_options_flag
{
	char 					*log;
	char					*rep_port;
	char 					*pub_port;
	int 					cycle;
	int 					verbose;
	int 					size;
	FILE 					*log_file;
} 							t_options_flag;

typedef struct 	s_flag_setter
{
	char 					option;
	int						(*call)(t_options_flag *options_flags, char *data);
}							t_flag_setter;

typedef struct 		s_thread_options
{
	t_gameinfo 		*gameinfo;
	t_options_flag	*flags;
	pthread_mutex_t *mutex;
	pthread_cond_t 	*condition;
} 					t_thread_options;

typedef struct 	command_s 
{
	char 		*command;
	int			(*call)(zframe_t *msg, char *data, t_thread_options *options, zsock_t *router);
} 				t_command;

#endif
