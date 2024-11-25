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
	$(SRC_DIR)commands/cmd_array.c\
	$(SRC_DIR)parser/parser.c\
	$(SRC_DIR)parser/expand.c\
	$(SRC_DIR)parser/handle_cmd_args.c\
	$(SRC_DIR)parser/handle_cmd_args_utils.c\
	$(SRC_DIR)parser/split_input.c\
	$(SRC_DIR)parser/syntax_checks.c\
	$(SRC_DIR)redirection/redir_ll.c\
	$(SRC_DIR)redirection/handle_redirections.c\
	$(SRC_DIR)redirection/redirector.c\
	$(SRC_DIR)redirection/open_files.c\
	$(SRC_DIR)utils/freeing.c\
	$(SRC_DIR)executor/find_cmd_path.c\
	$(SRC_DIR)executor/pipeline.c\
	$(SRC_DIR)executor/pipeline_utils.c\
	$(SRC_DIR)executor/child_process.c\
	$(SRC_DIR)executor/child_process_utils.c\
	$(SRC_DIR)executor/handle_builtins.c\
	$(SRC_DIR)executor/fd_handler.c

FLAGS = -Wall -Werror -Wextra -g

LDFLAGS = -lreadline

LIBFT = ./libft/libft.a

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
		@cc $(OBJS) $(LIBFT) -o $(NAME) $(LDFLAGS) > /dev/null
		@echo "\n\033[0;32mLet's mini!\033[0m\n"

clean:
		@rm -rf $(OBJ_DIR)
		@make -C $(LIBFT_DIR) clean

fclean: clean
		@rm -f $(NAME)
		@make -C $(LIBFT_DIR) fclean
		@echo "\n\033[0;31mAll is gone\033[0m\n"

re: fclean all

.PHONY: all clean fclean re libft
