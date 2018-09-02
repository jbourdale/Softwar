#include "router.h"
#include "core/player/player.h"

void parse_monitor_event(zmsg_t *msg, t_thread_options *options, zsock_t *router)
{
	(void) options;
	(void) router;
	// zmsg_t *response;
	// zframe_t *empty;
	// zframe_t *content;
	// zframe_t *identity;
	// int send_ret, i;
	// char player_str[1024];

	log_info("[NETWORK] Event recieved size : %d", zmsg_size(msg));
	zframe_t *f1 = zmsg_pop(msg);
	zframe_t *f2 = zmsg_pop(msg);
	zframe_t *f3 = zmsg_pop(msg);
	log_info("[NETWORK] Event str recieved :(%s,%s,%s)", zframe_strdup(f1), zframe_strdup(f2), zframe_strdup(f3));
	log_info("[NETWORK] Event hex recieved :(%s,%s,%s)", zframe_strhex(f1), zframe_strhex(f2), zframe_strhex(f3));

	// pthread_mutex_lock(options->mutex);
	// if (strcmp(event, "DISCONNECTED") == 0)
	// {
	// 	log_info("before wait");
	// 	fflush(stdout);
	// 	usleep(1000000);
	// 	log_info("after wait");
	// 	i = 0;
	// 	while (options->gameinfo->players[i] != NULL)
	// 	{
	// 		send_ret = -123;
	// 		response = zmsg_new();

	// 		content = zframe_new("test", 4);
	// 		empty = zframe_new_empty();
	// 		identity = zframe_dup(options->gameinfo->players[i]->identity);
	// 		identity = zframe_new("test", 4);

	// 		zmsg_prepend(response, &identity);
	// 		zmsg_append(response, &empty);
	// 		zmsg_append(response, &content);

	// 		send_ret = zmsg_send(&response, router);

	// 		str_player_player(options->gameinfo->players[i], player_str);
	// 		log_debug("Send ret for %s : %d", player_str, send_ret);

	// 		zmsg_destroy(&response);
	// 	    zframe_destroy(&identity);
	// 	    zframe_destroy(&empty);
	// 	    zframe_destroy(&content);
	// 	    i++;
	// 	}
	// }
	// pthread_mutex_unlock(options->mutex);
	// freen (event);
	zmsg_destroy (&msg);
}