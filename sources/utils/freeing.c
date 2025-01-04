/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 10:13:29 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/03 12:52:18 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_free_int_arr_with_size(int **array, int size);
void	error(t_shell *mini, char *str);
void	clean_env(t_env *ll, char **array);
void	clean_redir(t_redir *head);

/**
 * ft_free_int_arr_with_size - Frees an array of integer pointers
 * with a known size.
 *
 * @array: Pointer to the array of integer pointers to be freed.
 * @size: The number of elements in the array.
 */
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

/**
 * error - Prints an error message and updates the shell's exit status.
 *
 * @mini: Pointer to the shell structure to update the exit status.
 * @str: Error message to be displayed.
 */
void	error(t_shell *mini, char *str)
{
	ft_putendl_fd(str, 2);
	mini->exit_stat = 1;
}

/**
 * clean_env - Frees a linked list representing
 * the shell's environment variables and an optional array of strings.
 *
 * @ll: Pointer to the head of the linked list of environment variables.
 * @array: Array of strings representing environment variables, can be NULL.
 */
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

/**
 * clean_redir - Frees a linked list of redirection nodes.
 *
 * @head: Pointer to the head of the linked list of redirection nodes.
 */
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
