#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/includes/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <limits.h>
# include <errno.h>
# include <signal.h>
//# include </usr/include/linux/signal.h>

typedef struct s_env
{
	char			*name;
	char			*value;
//	char			*both;
	struct s_env	*next;
} t_env;

typedef struct s_shell
{
	//char	**envp;
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
int	built_unset(t_env **env, char **cmd);
/*built_in/env*/
	/*env_handling*/
char	**copy_env(char **envp);
int		built_env(t_shell *mini);
t_env	*list_env(char **envp);
void	to_alphabetical(char **array);
	/*env_ll*/
t_env	*add_node(char *env);
void	ft_env_lstadd_back(t_env **lst, t_env *new);

/*utils/freeing*/
void	cleaner(t_env *node, char **temp);
void	error(char *str);

/*signals.c*/
void	init_sig(void);

#endif
