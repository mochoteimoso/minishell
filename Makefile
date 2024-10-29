NAME = minishell

SRC_DIR = sources/

SRC = $(SRC_DIR)main.c

FLAGS = -Wall -Werror -Wextra -g

LDFLAGS = -lreadline

LIBFT = ./libft/libft.a

LIBFT_DIR = ./libft

OBJ_DIR = objects/

OBJS = $(SRC:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)

MAKEFLAGS += --no-print-directory

all: $(LIBFT) $(NAME)

$(OBJ_DIR):
		@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)%.o:$(SRC_DIR)%.c | $(OBJ_DIR)
		@cc $(FLAGS) -c $< -o $@

$(LIBFT):
		@make -C $(LIBFT_DIR)
		@echo "\n\033[0;32mLibft ready\033[0m\n"

$(NAME): $(OBJS)
		@cc $(OBJS) $(LIBFT) -o $(NAME) $(LDFLAGS) > /dev/null
		@echo "\n\033[0;32mLet's mini!\033[0m\n"

clean:
		@rm -f $(OBJ_DIR)
		@make -C $(LIBFT_DIR) clean

fclean:
		@rm -f $(NAME)
		@make -C $(LIBFT_DIR) fclean
		@echo "\n\033[0;31mAll is gone\033[0m\n"

re: fclean all

.PHONY: all clean fclean re libft
