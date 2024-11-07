#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/includes/libft.h"
# include "../libft/includes/get_next_line.h"
# include "../libft/includes/ft_printf.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <limits.h>
# include <stdbool.h>
# include <stdlib.h>
# include <errno.h>
# include <signal.h>

// struct for input segments, split based on pipes

typedef struct s_command
{
	char	*segment;
	char	*command;
	char	**args;
	int		args_count;
	char	*redirect_in;
	char	*redirect_out;
	int		redirect_type; //??
	char	**env_vars; //??
	int		env_var_count; //??
	char	*append;
	bool	heredoc;
	char	*heredoc_delim;
	char	*heredoc_content;
	int		exit_status;
}	t_command;


typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
} t_env;

// struct to store general information

typedef struct s_shell
{
	t_env		*env;
	char		**pending;
	t_command	**cmds;
	int			exit_stat;
} t_shell;

/*typedef struct s_mini
{
	char	**split_input;

} t_mini; */

// enum of token types

typedef enum e_tok_type
{
	WORD,
	PIPE,
	APPEND,
	REDIRECT_IN,    // redirection '<'
	REDIRECT_OUT,   // redirection '>'
	ENV_VAR,
	HEREDOC,
	DELIMITER
} t_tok_type;


int		validate_input_syntax(char *input);
int		is_in_quotes(char *input, int i);
int		check_consecutive_pipes(char *input);
int		check_pipes(char *input);
int		check_redirects(char *input);
int		validate_redirect(char *input, int *i, char *type);
int		count_pipes(char *input);
t_command **allocate_cmd_array(int command_count);
int		split_input_by_pipes(char *input, t_shell *sh);
int		parse_input_segments(t_shell *sh);
int		parse_and_validate_input(char *input, t_shell *sh);
int		prepare_command_structs(t_shell *sh, char *input);
void	init_sig(void);
int		split_input_by_pipes(char *input, t_shell *sh);
char	*ft_strndup(const char *src, size_t n);

#endif
