NAME	:= philo
CC		:= cc
FLAGS	:= -Wall -Wextra -Werror -g -pthread

# Folder Paths
PHILO_DIR	:= philo

# Source Files
SRCS := philo.c \
		utils.c \
		routine.c \
		death.c \

OBJS := $(SRCS:.c=.o)

# Main Target
all: $(NAME)

# Executable Creation
$(NAME): $(OBJS)
	@echo "Compiling $(NAME)..."
	$(CC) $(FLAGS) -o $@ $^

# Compilation Rule
%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

# Cleaning Rules
clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re