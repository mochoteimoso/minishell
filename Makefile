NAME = minishell

SRC_DIR = sources/

SRC = $(SRC_DIR)server.c

FLAGS = -Wall -Werror -Wextra -g

LIBFT = ./libft/libft.a

OBJ_DIR = objects/

OBJS = $(OBJ_DIR)

LIBFT_DIR = ./libft

MAKEFLAGS += --no-print-directory

all: $(LIBFT) $(NAME)

$(OBJ_DIR):
		@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)client.o: $(SRC_C) | $(OBJ_DIR)
		@cc $(FLAGS) -c $< -o $@

$(LIBFT):
		@make -C $(LIBFT_DIR)
		@echo "\n\033[0;32mLibft ready\033[0m\n"

$(NAME_C): $(OBJS_C)
		@cc $(OBJS_C) $(LIBFT) -o $(NAME_C) > /dev/null
		@echo "\n\033[0;32mLet's mini!\033[0m\n"

clean:
		@rm -f $(OBJS_C) $(OBJS_S)
		@make -C $(LIBFT_DIR) clean

fclean:
		@rm -f $(NAME)
		@rm -f $(OBJS)
		@make -C $(LIBFT_DIR) fclean
		@echo "\n\033[0;31mAll is gone\033[0m\n"

re: fclean all

.PHONY: all clean fclean re libft
