##
## Makefile for softwar_server
##
## Made by BOURDALE Jules
## Login   <bourda_j@etna-alternance.net>
##
## Started on  Fri Oct 20 18:14:57 2017 BOURDALE Jules
## Last update Fri Nov 10 23:26:05 2017 BOURDALE Jules
##

CC			= 	gcc
INCFLAGS	= 	-I .
CFLAGS		= 	-Wall -Werror -W
LDFLAGS		= 	-lzmq -lczmq -lpthread -ljansson -DLOG_USE_COLOR
NAME		= 	softwar_server
SRC			= 	softwar_server.c						\
				network/router/server.c 				\
				network/router/message.c 				\
				network/router/event.c 					\
				network/notification/server.c			\
				network/notification/notifications.c 	\
				network/notification/json.c 			\
				core/options/options.c 					\
				core/commands/attack.c 					\
				core/commands/backward.c 				\
				core/commands/forward.c 				\
				core/commands/gather.c 					\
				core/commands/identity.c 				\
				core/commands/inspect.c 				\
				core/commands/jump.c 					\
				core/commands/left.c 					\
				core/commands/left_forward.c 			\
				core/commands/looking.c 				\
				core/commands/next_turn.c 				\
				core/commands/right.c 					\
				core/commands/right_forward.c 			\
				core/commands/self.c 					\
				core/commands/watch.c 					\
				core/commands/self_energy.c 			\
				core/game/game.c 						\
				core/game/init.c 						\
				core/game/utils.c 						\
				core/player/utils.c						\
				core/energycell/utils.c					\
				core/log/log.c 							\
				core/log/utils.c						\
				utils/flags.c 							\
				utils/utils.c 							\

all:	fclean $(NAME)

$(NAME):
	$(CC) -g3 $(CFLAGS) $(INCFLAGS) $(SRC) -o $(NAME) $(LDFLAGS)

clean:
	rm -f *.o

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
