#ifndef ROUTER_H
#define ROUTER_H

#include "softwar_server.h"
#include "utils/flags.h"
#include "core/core.h"
#include "core/game/game.h"
#include "core/options/options.h"

void 	*launch_router(void *args);
void	parse_message(zsock_t *router, zmsg_t *message, t_thread_options *options);
int 	call_command(char **msg, zframe_t *fidentity, t_thread_options *options, zsock_t *router);
void	create_response(zmsg_t *response, zframe_t *identity, zframe_t *empty, zframe_t *content);
char 	**extract_from_content(zframe_t *fcontent);
void 	send_response(zsock_t *router, zframe_t *identity, zframe_t *content);

void 	send_response_empty_ok(zsock_t *router, zframe_t *identity);
void 	send_response_empty_ko(zsock_t *router, zframe_t *identity);

void parse_monitor_event(zmsg_t *msg, t_thread_options *options, zsock_t *router);

#endif