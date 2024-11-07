NAME = minishell

SRC_DIR = sources/

SRC_FILES = syntax_checks.c parser.c array_of_structs.c split_input.c main.c

FLAGS = -Wall -Werror -Wextra -g

LDFLAGS = -lreadline

LIBFT = ./libft/libft.a
LIBFT_DIR = ./libft

OBJ_DIR = objects/

SRCS := $(addprefix $(SRC_DIR), $(SRC_FILES))
OBJS = $(patsubst $(SRC_DIR)%.c, $(OBJ_DIR)%.o, $(SRCS))

LIBFT_DIR = ./libft

MAKEFLAGS += --no-print-directory

all: $(LIBFT) $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c | $(OBJ_DIR)
	@cc $(FLAGS) -c $< -o $@

$(LIBFT):
	@make -C $(LIBFT_DIR)
	@echo "\n\033[0;32mLibft ready\033[0m\n"

$(NAME): $(OBJS)
	@cc $(OBJS) $(LIBFT) -o $(NAME) $(LDFLAGS) > /dev/null
	@echo "\n\033[0;32mLet's mini!\033[0m\n"

clean:
	@rm -f $(OBJ_DIR)/*.o
	@make -C $(LIBFT_DIR) clean

fclean: clean
	@rm -f $(NAME)
	make -C $(LIBFT_DIR) fclean
	@echo "\n\033[0;31mAll is gone\033[0m\n"

re: fclean all

.PHONY: all clean fclean re libft
