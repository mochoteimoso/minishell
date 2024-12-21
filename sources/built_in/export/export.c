/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 17:01:15 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/21 15:35:25 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static int	update_pending(t_shell *mini, char *str)
{
	size_t	i;
	char	**temp;

	i = 0;
	i = ft_array_len(mini->pending);
	temp = ft_realloc(mini->pending, i * sizeof(char *), (i + 2) * sizeof(char *));
	if (!temp)
		return (1);
	mini->pending = temp;
	mini->pending[i] = ft_strdup(str);
	mini->pending[i + 1] = NULL;
	return (0);
}

static void	print_pending(t_shell *mini)
{
	int	i;

	i = 0;
	while (mini->pending[i])
	{
		printf("%s\n", mini->pending[i]);
		i++;
	}
}

static int	parse_and_add(t_shell *mini, char *str)
{
	char	*sign;
	t_env	*new;

	if (!validate_variable(str))
	{
		mini->exit_stat = 1;
		return (1);
	}
	sign = ft_strchr(str, '=');
	if (!sign)
	{
		update_pending(mini, str);
		return (0);
	}
	else if (sign)
	{
		if (update_env(mini, str) == 0)
		{
			new = add_node(str);
			ft_env_lstadd_back(&mini->env, new);
		}
		update_pending(mini, str);
		to_alphabetical(mini->pending);
		return (0);
	}
	mini->exit_stat = 1;
	return (1);
}

/*Sets an environment variable. Accepts VAR=VALUE format to
define or update variables.*/

int	built_export(t_shell *mini, t_cmd *cmd)
{
	int	i;
	int	sum;

	sum = 1;
	i = 1;
	while (cmd->args[sum])
		sum++;
	if (sum == 1)
		print_pending(mini);
	if (sum >= 2)
	{
		while (i < sum)
		{
			if (cmd->args[i][0] == '-')
			{
				ft_putendl_fd("Invalid option", 2);
				return (2);
			}
			if (parse_and_add(mini, cmd->args[i]))
			{
				ft_putendl_fd("not a valid identifier", 2);
				// mini->exit_stat = 1;
				return (1);
			}
			i++;
		}
	}
	return (0);
}
