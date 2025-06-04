NAME	:= philo
CC		:= cc
FLAGS	:= -Wall -Wextra -Werror -g -pthread

# Source Files
SRCS_FILES := philo.c \
		utils.c \
		routine.c \
		death.c \
		extra_utils.c

OBJS := $(SRCS_FILES:.c=.o)

# Main Target
all: $(NAME)

# Final binary outside philo/
$(NAME): $(OBJS)
	$(CC) $(FLAGS) -o $@ $^

# Object compilation
%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

# Cleaning Rules
clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re