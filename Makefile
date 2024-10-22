NAME = ft_traceroute
LIBFT = ./libft/libft.a
SRC =	./src/ft_traceroute.c \
		./src/parser.c \
		./src/utils.c \
		./src/socket.c \
		./src/routine.c \
		./src/probe.c
OBJS = $(SRC:.c=.o)
CFLAGS = -Wall -Wextra -Werror
CC = cc
INCL = -I ./incl
R = rm -rf

RED = \033[0;31m
GREEN = \033[0;32m
BLUE = \033[0;36m
YELLOW = \033[1;33m
NC = \033[0m

all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(BLUE)compiling libft$(NC)"
	@make all -C ./libft
	@echo "$(BLUE)linking object files$(NC)"
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)
	@echo "$(RED)$(NAME) ready$(NC)"

%.o: %.c
	@echo "$(BLUE)compiling $<$(NC)"
	$(CC) $(CFLAGS) -c $< -o $@ $(INCL)

clean:
	@$(R) $(OBJS)
	@${MAKE} clean -C ./libft
	@echo "$(YELLOW)clean done.$(NC)"

fclean: clean
	@$(R) $(NAME)
	@${MAKE} fclean -C ./libft
	@echo "$(GREEN)full clean done.$(NC)"

re: fclean all
