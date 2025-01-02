NAME	= libft.a

INC = -I ./includes/

SRC_DIR = sources/

SRC		= \
		$(addprefix checks/, ft_array_len.c ft_countchar.c ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c ft_isprint.c ft_isspace.c)\
		$(addprefix converters/, ft_atoi.c ft_atoi_base.c ft_itoa.c ft_tolower.c ft_toupper.c)\
		$(addprefix ft_printf/, ft_printf.c check_f.c is_char.c is_hex.c is_nbr.c is_pointer.c is_str.c is_unsignbr.c)\
		$(addprefix list/, ft_lstadd_back.c ft_lstadd_front.c ft_lstclear.c ft_lstdelone.c ft_lstiter.c\
		ft_lstlast.c ft_lstmap.c ft_lstnew.c ft_lstsize.c)\
		$(addprefix  memory/, ft_bzero.c ft_calloc.c ft_free_array.c ft_memchr.c ft_memcmp.c ft_memcpy.c ft_memmove.c ft_memset.c ft_realloc.c)\
		$(addprefix string/, ft_putchar_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_putstr_fd.c ft_strdup.c ft_strndup.c ft_strchr.c ft_strcmp.c ft_split.c\
		ft_strdup.c ft_striteri.c ft_strjoin.c ft_strlcat.c ft_strlcpy.c ft_strlen.c ft_strmapi.c ft_strncmp.c ft_strnstr.c ft_strrchr.c ft_strtrim.c ft_substr.c)\
		$(addprefix gnl/, get_next_line.c get_next_line_utils.c)

src := $(addprefix $(SRC_DIR), $(SRC))

OBJ_DIR    = objects/

OBJS        = $(addprefix $(OBJ_DIR), $(SRC:.c=.o))

OBJ_DIRS    = $(sort $(dir $(OBJS)))

HEAD	= ./libft

AR		= @ar rcs

CC		= cc

CFLAGS	= -Wall -Wextra -Werror -g

all: $(OBJ_DIRS) $(NAME)

$(OBJ_DIRS):
		@mkdir -p $@

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
		@$(CC) $(CFLAGS) -c -I $(HEAD) $< -o $@

$(NAME):	$(OBJS)
				$(AR) $(NAME) $(OBJS)

clean:
		@rm -rf $(OBJ_DIR)

fclean:	clean
		@rm -f $(NAME)

re:		fclean all

.PHONY: all clean fclean re
