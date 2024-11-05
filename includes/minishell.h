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
	struct s_env	*next;
} t_env;

typedef struct s_shell
{
	char	**envp;
	t_env	*env;
} t_shell;

/*env/env_handling*/
char	**copy_env(char **envp);
void	*list_env(t_env *ll, char **envp);

/*env/env_ll*/
t_env	*add_node(void *stuff);
void	ft_env_lstadd_back(t_env *lst, t_env *new);

/*utils/freeing*/
void	cleaner(t_env *node, char **temp);

/*signals.c*/
void	init_sig(void);

#endif
