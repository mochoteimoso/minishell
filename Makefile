NAME = minishell

SRC_DIR = sources/


SRC = $(SRC_DIR)main.c\
	$(SRC_DIR)signals.c\
	$(SRC_DIR)built_in/cd.c\
	$(SRC_DIR)built_in/echo.c\
	$(SRC_DIR)built_in/exit.c\
	$(SRC_DIR)built_in/export.c\
	$(SRC_DIR)built_in/pwd.c\
	$(SRC_DIR)built_in/unset.c\
	$(SRC_DIR)built_in/env/env.c\
	$(SRC_DIR)built_in/env/env_handling.c\
	$(SRC_DIR)built_in/env/env_ll.c\
	$(SRC_DIR)utils/freeing.c

FLAGS = -Wall -Werror -Wextra -g

LDFLAGS = -lreadline

LIBFT = ./libft/libft.a
LIBFT_DIR = ./libft

LIBFT_DIR = ./libft

OBJ_DIR = objects/

OBJS = $(patsubst $(SRC_DIR)%.c, $(OBJ_DIR)%.o, $(SRC))

OBJ_DIRS = $(sort $(dir $(OBJS)))

MAKEFLAGS += --no-print-directory

all: $(LIBFT) $(OBJ_DIRS) $(NAME)

$(OBJ_DIRS):
		@mkdir -p $(OBJ_DIRS)

$(OBJ_DIR)%.o:$(SRC_DIR)%.c
		@cc $(FLAGS) -c $< -o $@

$(LIBFT):
	@make -C $(LIBFT_DIR)
	@echo "\n\033[0;32mLibft ready\033[0m\n"

$(NAME): $(OBJS)

re: fclean all

.PHONY: all clean fclean re libft
