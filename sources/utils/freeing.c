/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 10:13:29 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/27 15:29:46 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_free_int_arr(int **array);
void	ft_free_int_arr_with_size(int **array, int size);
void	error(t_shell *mini, char *str);
void	clean_env(t_env *ll, char **array);
void	clean_redir(t_redir *head);

void	ft_free_int_arr(int **array)
{
	int	a;

	a = 0;
	if (!array)
		return ;
	while (array[a] != NULL)
	{
		free(array[a]);
		a++;
	}
	free(array);
}

void	ft_free_int_arr_with_size(int **array, int size)
{
	int	a;

	a = 0;
	if (!array)
		return ;
	while (a < size)
	{
		free(array[a]);
		a++;
	}
	free(array);
}

void	error(t_shell *mini, char *str)
{
	ft_putendl_fd(str, 2);
	mini->exit_stat = 1;
}

void	clean_env(t_env *ll, char **array)
{
	t_env	*temp;

	if (array)
		ft_free_array(array);
	while (ll != NULL)
	{
		temp = ll;
		free(ll->name);
		free(ll->value);
		ll = ll->next;
		free(temp);
	}
}

void	clean_redir(t_redir *head)
{
	t_redir	*temp;

	if (head)
	{
		while (head != NULL)
		{
			temp = head;
			free(head->file);
			free(head->delimiter);
			free(head->heredoc_name);
			head = head->next;
			free(temp);
		}
	}
}
