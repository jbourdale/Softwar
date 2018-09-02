// message.c for softwar server
// 
// Made by BOURDALE Jules
// Login   <bourda_j@etna-alternance.net>
// 
// Started on  Fri Avr 29 14:56:57 2018 BOURDALE Jules
//

#include "router.h"
#include "core/commands/commands.h"

static const t_command commands[] = 
{
	{IDENTITY_COMMAND, command_identity},
	{FORWARD_COMMAND, command_forward},
	{BACKWARD_COMMAND, command_backward},
	{LEFT_FORWARD_COMMAND, command_left_forward},
	{RIGHT_FORWARD_COMMAND, command_right_forward},
	{LEFT_COMMAND, command_left},
	{RIGHT_COMMAND, command_right},
	{LOOKING_COMMAND, command_looking},	
	{GATHER_COMMAND, command_gather},
	{ATTACK_COMMAND, command_attack},	
	{SELF_COMMAND, command_self},
	{SELF_ENERGY_COMMAND, command_self_energy},
	{INSPECT_COMMAND, command_inspect},
	{NEXT_TURN_COMMAND, command_next_turn},
	{JUMP_COMMAND, command_jump},
	{WATCH_COMMAND, command_watch},
	{NULL, NULL},
};

void send_response_empty_ok(zsock_t *router, zframe_t *identity)
{
	log_info("[COMMAND] Command executed successfuly");
	send_response(router, identity, zframe_new("ok|null", 7));
}

void send_response_empty_ko(zsock_t *router, zframe_t *identity)
{
	log_warn("[COMMAND] Command failed, something went wrong");
	send_response(router, identity, zframe_new("ko|null", 7));
}

char **extract_from_content(zframe_t *fcontent)
{
	char **extract;
	char *content;
	char *token;
	int i;

	extract = malloc(sizeof(char*) * 2);
	i = 0;
	while (i < 2)
	{
		extract[i] = NULL;
		i++;
	}
	content = zframe_strdup(fcontent);
	i = 0;
	while ((token = strsep(&content, "|")))
	{
		extract[i] = token;
		i++;
	}

	return extract;
}

int call_command(char **msg, zframe_t *fidentity, t_thread_options *options, zsock_t *router)
{
	int i;
	char *command;
	char *data;
	char player_str[1024];

	command = msg[0];
	data = msg[1];
	i = 0;
	while(commands[i].command != NULL)
	{
		if(strcmp(command, commands[i].command) == 0)
		{
			str_player(options->gameinfo, fidentity, player_str);
			log_info("[COMMAND] Recieved command '%s' with data '%s' from %s", command, data, player_str);
			return commands[i].call(fidentity, data, options, router);
		}
		i++;
	}
	log_warn("[COMMAND] Unrecognized command '%s'", command);
	send_response_empty_ko(router, fidentity);
	return FAILED;
}

void send_response(zsock_t *router, zframe_t *identity, zframe_t *content)
{
	log_debug("[ROUTER] >> Sending %d bytes to %s", zframe_size(content), zframe_strhex(identity));

	zmsg_t *response = zmsg_new();
	zframe_t *empty = zframe_new_empty();
	zframe_t *fidentity = zframe_dup(identity);

	zmsg_prepend(response, &fidentity);
	zmsg_append(response, &empty);
	zmsg_append(response, &content);

	zmsg_send(&response, router);

	zmsg_destroy(&response);
    zframe_destroy(&fidentity);
    zframe_destroy(&empty);
    zframe_destroy(&content);
}

void parse_message(zsock_t *router, zmsg_t *message, t_thread_options *options)
{
	char **extract;

	zframe_t *fidentity = zmsg_pop(message);
	zmsg_pop(message);
	zframe_t *fcontent = zmsg_pop(message);
	zmsg_destroy(&message);

	log_debug("[ROUTER] << Recieved %d bytes from : %s", zframe_size(fcontent), zframe_strhex(fidentity));

	extract = extract_from_content(fcontent);
	call_command(extract, fidentity, options, router);
	    
    free(extract);
}