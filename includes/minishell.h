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

typedef struct s_redir
{
	char				*file;
	char				*delimiter;
	t_redir_type		*type;
	struct s_redir		*next;
}	t_redir;

typedef struct s_cmd
{
	char	*segment;
	char	*command;
	char	**args;
	int		args_count;
	// char	*redirect_in;
	// char	*redirect_out;
	// int		redirect_type; //??
	char	**env_vars; //??
	int		env_var_count; //??
	char	*append;
	t_redir *redir;
	//bool	heredoc;
	// char	*heredoc_delim;
	// char	*heredoc_content;
	int		exit_status;
}	t_cmd;

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
} t_env;

typedef struct s_shell
{
	t_cmd	**cmds;
	t_env	*env;
	char	**pending;
	int		exit_stat;
} t_shell;

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
/*redirection/redir_ll*/
bool	is_redirection(char *str, int i);
t_redir	*list_redir(void);
/*utils/freeing*/
void	cleaner(t_env *node, char **temp);
void	error(char *str);

/*signals.c*/
void	init_sig(void);

#endif
