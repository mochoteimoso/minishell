#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/includes/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <limits.h>
# include <stdbool.h>
# include <stdlib.h>
# include <errno.h>
# include <signal.h>

//# include </usr/include/linux/signal.h>

typedef enum e_redir_type
{
	REDIRECT_IN,
	REDIRECT_OUT,
	APPEND,
	HEREDOC
}	t_redir_type;

// linked list for redirects in each command struct

typedef struct s_redir
{
	char			*file;
	char			*delimiter;
	t_redir_type	type;
	int				node_ind;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char	*segment;
	char	*command;
	char	**args;
	int		args_count;
	char	**env_vars; //??
	int		env_var_count; //??
	char	*append;
	t_redir *redir_head;
	t_redir *redir_tail;
	int		exit_status;
}	t_cmd;

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;

}	t_env;

typedef struct s_shell
{
	t_cmd	**cmds;
	t_env	*env;
	char	**pending;
	int		exit_stat;
} t_shell;

int		validate_input_syntax(char *input);
int		is_in_quotes(char *input, int i);
int		check_consecutive_pipes(char *input);
int		check_pipes(char *input);
int		check_redirects(char *input);
int		validate_redirect(char *input, int *i, char *type);
int		count_pipes(char *input);
t_cmd	**allocate_cmd_array(int command_count);
int		split_input_by_pipes(char *input, t_shell *mini);
int		parse_input(t_shell *sh);
int		parse_and_validate_input(char *input, t_shell *mini);
int		prepare_command_structs(t_shell *sh, char *input);
void	init_sig(void);
int		split_input_by_pipes(char *input, t_shell *mini);
char	*ft_strndup(const char *src, size_t n);
bool	is_redirection(t_cmd *cmd, int i);
int		handle_redirect_in(t_cmd *cmd, int i);
int		handle_redirect_out(t_cmd *cmd, int i);
int		handle_heredoc(t_cmd *cmd, int i);
int		handle_append(t_cmd *cmd, int i);
int		handle_cmd_args(t_cmd *cmd, int i);
int		ft_isspace(char c);
t_redir	*list_redir(void);
void	redir_lstadd_back(t_redir **lst, t_redir *new);
t_redir	*redir_add_node(void);
void 	redir_update_tail(t_cmd *cmd);

/*built_in*/
	/*cd*/
int		built_cd(t_shell *mini, char **cmd);
	/*echo.c*/
int		built_echo(char **cmd);
	/*exit.c*/
int		built_exit(t_shell *mini, char **cmd);
	/*export.c*/
int		built_export(t_shell *mini, char **args);
	/*pwd.c*/
int		built_pwd(t_shell *mini);
	/*unset.c*/
int	built_unset(t_shell *mini, char **cmd);
/*built_in/env*/
	/*env_handling*/
char	**copy_env(char **envp);
int		built_env(t_shell *mini);
t_env	*list_env(char **envp);
void	to_alphabetical(char **array);
	/*env_ll*/
t_env	*add_node(char *env);
void	ft_env_lstadd_back(t_env **lst, t_env *new);
/*commands*/
	/*cmd_array.c*/
int			count_pipes(char *line);
int			prepare_command_structs(t_shell *sh, char *input);
t_cmd	**allocate_cmd_array(int command_count);
void		initialize_command_struct(t_cmd *cmd);
	/*cmd_array_utils.c*/
int	handle_cmd_args(char *cmd_string, int i, t_shell *sh, int index);
int	count_args(char *cmd_string, int i);
/*parser*/
	/*parser.c*/
char	*ft_strndup(const char *src, size_t n);
int		parse_input(t_shell *sh);
int		parse_cmd_string(t_shell *sh, int index);
int		handle_redirections(char *cmd_string, int i, t_shell *sh, int index);
int		handle_cmd_name(char *cmd_string, int i, t_shell *sh, int index);
int			parse_and_validate_input(char *input, t_shell *sh);
	/*parser_utils.c*/
char	*ft_strndup(const char *src, size_t n);
	/*split_inputs.c*/
int		split_input_by_pipes(char *input, t_shell *sh);
char	*trim_whitespace(char *segment);
int		ft_isspace(char c);
	/*syntax_checls.c*/
int	validate_input_syntax(char *input);
int	is_in_quotes(char *input, int i);
int	has_quotes(char *input, int i);
int	check_consecutive_pipes(char *input);
int	check_pipes(char *input);
int	check_redirects(char *input);
int	validate_redirect(char *input, int *i, char *type);
/*redirection*/
	/*redir_ll*/
t_redir	*list_redir(void);
	/*handle_redirections.c*/
bool	is_redirection(char *str, int index);
int		handle_redirect_in(char *str, int *i, t_shell *sh, int index);
int		handle_redirect_out(char *str, int *i, t_shell *sh, int index);
int		handle_heredoc(char *str, int *i, t_shell *sh, int index);
int		handle_append(char *str, int *i, t_shell *sh, int index);
/*utils/freeing*/
void	cleaner(t_env *node, char **temp);
void	error(char *str);

/*signals.c*/
void	init_sig(void);

#endif
