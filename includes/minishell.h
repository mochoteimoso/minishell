#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/includes/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <limits.h>
# include <errno.h>

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

/*env_ll*/
int		set_name(t_env *node, void *stuff);
int		set_value(t_env *node, void *stuff);

#endif
