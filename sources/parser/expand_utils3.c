/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 18:55:47 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 12:54:25 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		init_expansion(t_expand *arg, char **expan);
int		the_arg(t_expand *arg, int i);
void	what_quote(char *str, t_expand *arg);
int		handle_question(t_shell *mini, char *str, char **expan, t_expand *arg);
int		new_result(t_expand *arg, char *temp);

/**
 * init_expansion - Initializes the expansion process.
 *
 * @arg: Pointer to the t_expand structure used for tracking the state.
 * @expan: Pointer to the string used to accumulate the expanded result.
 *
 * Allocates memory for the initial expansion result as an empty string
 * and initializes the expansion argument structure. Returns 1 on failure.
 */
int	init_expansion(t_expand *arg, char **expan)
{
	*expan = ft_strdup("");
	if (!(*expan))
		return (1);
	the_arg(arg, 0);
	if (!arg->value)
	{
		free(*expan);
		return (1);
	}
	free(arg->value);
	arg->value = NULL;
	return (0);
}

/**
 * the_arg - Initializes the t_expand structure for expansion.
 *
 * @arg: Pointer to the t_expand structure.
 * @i: Initial index in the string being processed.
 *
 * Sets the initial values for tracking quotes, indices, and the expansion
 * value as an empty string. Returns 1 on memory allocation failure.
 */
int	the_arg(t_expand *arg, int i)
{
	arg->sgl = 0;
	arg->dbl = 0;
	arg->i = i;
	arg->start = i;
	arg->name = NULL;
	arg->value = ft_strdup("");
	if (!arg->value)
	{
		ft_putendl_fd("Malloc fail", 2);
		return (1);
	}
	return (0);
}

/**
 * what_quote - Updates quote state in the t_expand structure based
 * 				on the current character.
 *
 * @str: The input string being processed.
 * @arg: Pointer to the t_expand structure.
 *
 * Toggles single or double quote tracking depending on the current character
 * in the input string. Advances the index in the structure as needed.
 */
void	what_quote(char *str, t_expand *arg)
{
	if ((arg->sgl == 1 && str[arg->i] == '\'') || (arg->dbl == 1
			&& str[arg->i] == '"'))
	{
		if (str[arg->i] == '\'')
			arg->sgl = !arg->sgl;
		else if (str[arg->i] == '\"')
			arg->dbl = !arg->dbl;
		arg->i++;
		return ;
	}
	if (str[arg->i] == '\'' && arg->sgl == 0)
	{
		arg->sgl = 1;
		arg->i++;
		return ;
	}
	if (str[arg->i] == '"' && arg->dbl == 0)
	{
		arg->dbl = 1;
		arg->i++;
		return ;
	}
}

/**
 * handle_question - Handles special cases such as '$?' during expansion.
 *
 * @mini: Pointer to the shell structure for environment information.
 * @str: The input string being processed.
 * @expan: Pointer to the string used to accumulate the expanded result.
 * @arg: Pointer to the t_expand structure used for tracking state.
 *
 * Processes the special variable '$?' by updating indices and potentially
 * calling additional handlers for dollar expansions. Returns the updated index.
 */
int	handle_question(t_shell *mini, char *str, char **expan, t_expand *arg)
{
	int	cont;

	arg->start = arg->i;
	cont = arg->start;
	if (str[arg->i] == '$')
		arg->i = oh_a_dollar(mini, str, expan, arg);
	arg->i = cont + 2;
	return (arg->i);
}

/**
 * new_result - Combines the current expansion value with a new string segment.
 *
 * @arg: Pointer to the t_expand structure used for tracking state.
 * @temp: The new string segment to be added to the current expansion result.
 *
 * Joins the current expansion result with the new string segment. Frees
 * the old values and updates the expansion result. Returns -1 on failure.
 */
int	new_result(t_expand *arg, char *temp)
{
	char	*new_res;

	new_res = ft_strjoin(arg->value, temp);
	if (!new_res)
	{
		free(temp);
		return (-1);
	}
	free(arg->value);
	arg->value = NULL;
	free(temp);
	arg->value = new_res;
	return (0);
}
