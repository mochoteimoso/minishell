/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 17:01:15 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/31 11:07:41 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int			built_export(t_shell *mini, t_cmd *cmd);
static void	print_pending(t_shell *mini);
static int	parse_and_add(t_shell *mini, char *str);
static int	validate_variable(char *name);

/*Sets an environment variable. Accepts VAR=VALUE format to
define or update variables.*/
int	built_export(t_shell *mini, t_cmd *cmd)
{
	int	i;

	i = 1;
	if (cmd->a_num == 1)
		print_pending(mini);
	if (cmd->a_num >= 2)
	{
		while (i < cmd->a_num)
		{
			if (cmd->args[i][0] && cmd->args[i][0] == '-')
			{
				ft_putendl_fd("Invalid option", 2);
				return (2);
			}
			if (parse_and_add(mini, cmd->args[i]))
			{
				ft_putendl_fd("not a valid identifier", 2);
				return (1);
			}
			i++;
		}
	}
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

	if (!validate_variable(str))
	{
		mini->exit_stat = 1;
		return (1);
	}
	sign = ft_strchr(str, '=');
	if (!sign)
	{
		update_pending(mini, str, str);
		return (0);
	}
	else if (sign)
	{
		handle_sign(mini, str);
		return (0);
	}
	mini->exit_stat = 1;
	return (1);
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
