#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/includes/libft.h"
# include "../libft/includes/get_next_line.h"
# include "../libft/includes/ft_printf.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// struct for input segments, split based on pipes

typedef struct s_command
{
	char 	*command;
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


// struct to store general information

typedef struct s_mini
{
	char	**split_input;

} t_mini;

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


int		validate_input_syntax(char *line);
int		is_in_quotes(char *line, int i);
int		check_consecutive_pipes(char *line);
int		check_pipes(char *line);
int		check_redirects(char *line);
int		validate_redirect(char *line, int *i, char *type);
int		count_pipes(char *line);
t_command *initialize_cmd_array(int count);
void	add_single_command(t_command *cmd, char *command);
void	add_cmd_to_array(t_command *cmds, int *count, int size, char *command);
void	handle_single_command(char *line);
char	**split_input_by_pipes(char *line, t_mini *mini);
char	**split_input_by_pipes(char *line, t_mini *mini);
int		parse_input_segments(t_mini *mini, t_command *cmds);
int		parse_and_validate_input(char *line, t_mini *mini);

#endif
