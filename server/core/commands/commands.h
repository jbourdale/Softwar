#ifndef COMMANDS_H
#define COMMANDS_H

#include "softwar_server.h"
#include "network/router/router.h"
#include "core/player/player.h"
#include "core/energycell/energycell.h"

#define IDENTITY_COMMAND "identify"
#define FORWARD_COMMAND "forward"
#define BACKWARD_COMMAND "backward"
#define LEFT_FORWARD_COMMAND "leftfwd"
#define RIGHT_FORWARD_COMMAND "rightfwd"
#define LEFT_COMMAND "left"
#define RIGHT_COMMAND "right"
#define LOOKING_COMMAND "looking"
#define GATHER_COMMAND "gather"
#define ATTACK_COMMAND "attack"
#define SELF_COMMAND "selfid"
#define SELF_ENERGY_COMMAND "selfstats"
#define INSPECT_COMMAND "inspect"
#define NEXT_TURN_COMMAND "next"
#define JUMP_COMMAND "jump"
#define WATCH_COMMAND "watch"


int command_identity(zframe_t *msg, char *data, t_thread_options *options, zsock_t *router);
int command_forward(zframe_t *msg, char *data, t_thread_options *options, zsock_t *router);
int command_backward(zframe_t *msg, char *data, t_thread_options *options, zsock_t *router);
int command_left_forward(zframe_t *msg, char *data, t_thread_options *options, zsock_t *router);
int command_right_forward(zframe_t *msg, char *data, t_thread_options *options, zsock_t *router);
int command_left(zframe_t *msg, char *data, t_thread_options *options, zsock_t *router);
int command_right(zframe_t *msg, char *data, t_thread_options *options, zsock_t *router);
int command_looking(zframe_t *msg, char *data, t_thread_options *options, zsock_t *router);
int command_gather(zframe_t *msg, char *data, t_thread_options *options, zsock_t *router);
int command_attack(zframe_t *msg, char *data, t_thread_options *options, zsock_t *router);
int command_self(zframe_t *msg, char *data, t_thread_options *options, zsock_t *router);
int command_self_energy(zframe_t *msg, char *data, t_thread_options *options, zsock_t *router);
int command_inspect(zframe_t *msg, char *data, t_thread_options *options, zsock_t *router);
int command_next_turn(zframe_t *msg, char *data, t_thread_options *options, zsock_t *router);
int command_jump(zframe_t *msg, char *data, t_thread_options *options, zsock_t *router);
int command_watch(zframe_t *msg, char *data, t_thread_options *options, zsock_t *router);
void findCoordForward(int *pos, t_player player);

#endif