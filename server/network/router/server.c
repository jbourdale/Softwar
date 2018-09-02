// server.c for softwar server
// 
// Made by BOURDALE Jules
// Login   <bourda_j@etna-alternance.net>
// 
// Started on  Fri Avr 29 14:56:57 2018 BOURDALE Jules
//
#include "router.h"
#include "core/log/log.h"

zactor_t *init_monitor(zsock_t *router)
{
	zactor_t *router_monitor = zactor_new (zmonitor, router);
	zstr_sendx (router_monitor, "VERBOSE", NULL);
	zstr_sendx (router_monitor, "LISTEN", "LISTENING", "ACCEPTED", "DISCONNECTED", NULL);
	// zstr_sendx (router_monitor, "DISCONNECTED", NULL);
	zstr_sendx (router_monitor, "START", NULL);
	zsock_wait (router_monitor);
	return router_monitor;
}

zsock_t *init_router(char *port)
{
	zsock_t *router = zsock_new(ZMQ_ROUTER);
	if (zsock_bind(router, "tcp://*:%s", port) != 0)
	{
		return router;
	}
	return NULL;
}

void wait_players(zpoller_t *poller, t_thread_options *options, zsock_t *router)
{
	void *readable;
	log_trace("[ROUTER] Waiting for players connexion requests...");
	while (!zsys_interrupted && !game_startable(options))
	{
		if((readable = zpoller_wait(poller, 500)) != NULL)
		{
			if (readable == router)
				parse_message(readable, zmsg_recv(readable), options);
			else
				parse_monitor_event(zmsg_recv(readable), options, router);
		}
	}
	if (game_startable(options))
		start_game(options);
}

void *launch_router(void *args)
{
	zsock_t *router;
	zpoller_t *poller;
	zactor_t *router_monitor;
	void *readable;
	t_thread_options *options;

	options = (t_thread_options *)args;
	
	sleep(1); // be sure that notification server started -> TODO Using mutex condition

	log_info("[ROUTER] Launching server on port %s...", options->flags->rep_port);
	if ((router = init_router(options->flags->rep_port)) == NULL)
	{
		log_error("[ROUTER] Failed to launch router");
		return (void *)FAILED;
	}
	router_monitor = init_monitor(router);

	log_info("[ROUTER] Server launched successfuly");

	poller = zpoller_new(router, router_monitor, NULL);
	wait_players(poller, options, router);
	while (!zsys_interrupted && !game_ended(options))
	{
		if((readable = zpoller_wait(poller, 25)) != NULL)
		{
			parse_message(router, zmsg_recv(router), options);
		}
		// log_error("router montior : %s \n", router_monitor);
	}
	zactor_destroy (&router_monitor);
	zsock_destroy(&router);
	return (void *)SUCCESS;
}

