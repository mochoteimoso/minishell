/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 17:01:15 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/04 14:35:42 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

static int	validate_variable(char *name)
{
	int	i;

	i = 0;
	if (!name)
		return (0);
	if (!ft_isalpha(name[0]) && name[0] != '_')
		return (0);
	while (name[i] && name[i] != '=')
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static int	parse_and_add(t_shell *mini, char *str, int len)
{
	char	*sign;
	t_env	*new;

	if (!validate_variable(str))
	{
		mini->exit_stat = 1;
		return (1);
	}
	sign = ft_strchr(str, '=');
	if (sign && str[len - 1] == '=')
	{
		update_pending(mini, str);
		return (0);
	}
	else if (sign)
	{
		new = add_node(str);
		ft_env_lstadd_back(&mini->env, new);
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
	int	len;

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
			len = ft_strlen(cmd->args[i]);
			if (parse_and_add(mini, cmd->args[i], len))
				ft_putendl_fd("Invalid identifier", 2);
			len = 0;
			i++;
		}
	}
	return (0);
}

// static int	parse_and_add(t_shell *mini, char *str)
// {
// 	char	*sign;
// 	t_env	*new;

// 	sign = ft_strchr(str, '=');
// 	if (sign)
// 	{
// 		new = add_node(str);
// 		ft_env_lstadd_back(&mini->env, new);
// 		update_pending(mini, str);
// 		to_alphabetical(mini->pending);
// 		return (0);
// 	}
// 	else
// 	{
// 		update_pending(mini, str);
// 		return (0);
// 	}
// 	mini->exit_stat = 1;
// 	return (1);
// }
