/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 14:44:51 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 12:44:15 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			we_have_dollar(t_shell *mini, t_expand *arg, char *str);
int			oh_a_dollar(t_shell *mini, char *str, char **expan, t_expand *arg);
static int	finalize_expand(t_shell *mini, t_vdata *data, t_expand *arg);
static int	handle_new_expand(char *temp, char **expan);
static void	init_vdata(t_vdata *data, char **expan, char *temp, char *name);

/**
 * we_have_dollar - Expands a dollar sign ('$') sequence in the input string.
 *
 * @mini: Pointer to the shell structure for accessing environment variables.
 * @arg: Pointer to the t_expand structure tracking expansion state.
 * @str: Input string containing the dollar sequence.
 *
 * Handles variable expansion for sequences starting with '$'. Joins the expanded
 * result with the current expansion value and updates the t_expand structure.
 * Returns 0 on success or -1 on failure.
 */
int	we_have_dollar(t_shell *mini, t_expand *arg, char *str)
{
	int		s_exp;
	char	*temp;
	char	*new_res;

	s_exp = arg->i;
	temp = ft_strdup("");
	if (!temp)
		return (-1);
	arg->start = arg->i;
	arg->i = oh_a_dollar(mini, str, &temp, arg);
	if (arg->i == -1)
	{
		free(temp);
		return (-1);
	}
	new_res = ft_strjoin(arg->value, temp);
	if (!new_res)
	{
		free(temp);
		return (-1);
	}
	free(arg->value);
	free(temp);
	arg->value = new_res;
	return (0);
}

/**
 * oh_a_dollar - Processes the content following a dollar sign ('$').
 *
 * @mini: Pointer to the shell structure for accessing environment variables.
 * @str: Input string being processed.
 * @expan: Pointer to the string accumulating the expanded result.
 * @arg: Pointer to the t_expand structure tracking expansion state.
 *
 * Extracts the variable name following the '$', resolves its value, and appends
 * it to the expanded result. Supports special cases like '$?'.
 * Returns the updated index or -1 on failure.
 */
int	oh_a_dollar(t_shell *mini, char *str, char **expan, t_expand *arg)
{
	char	*temp;
	char	name[100];
	int		indx;
	t_vdata	data;

	indx = 0;
	temp = ft_strndup(&str[arg->start], arg->i - arg->start);
	if (!temp || handle_new_expand(temp, expan))
	{
		if (temp)
			free(temp);
		return (-1);
	}
	arg->i++;
	if (str[arg->i] == '?')
		just_a_quest(str, name, &indx, arg);
	else
		we_need_name(arg, str, name, &indx);
	init_vdata(&data, expan, temp, name);
	if (finalize_expand(mini, &data, arg))
		return (-1);
	return (arg->start);
}

/**
 * finalize_expand - Finalizes the expansion process for a variable.
 *
 * @mini: Pointer to the shell structure for accessing environment variables.
 * @data: Pointer to the t_vdata structure containing expansion details.
 * @arg: Pointer to the t_expand structure tracking expansion state.
 *
 * Handles the resolution and appending of
 * a variable's value to the expanded result.
 * Updates the starting index and frees temporary memory.
 * Returns 0 on success or 1 on failure.
 */
static int	finalize_expand(t_shell *mini, t_vdata *data, t_expand *arg)
{
	if (handle_value(mini, data))
		return (1);
	arg->name = ft_strdup(data->name);
	if (!arg->name)
		return (1);
	arg->start += ft_strlen(arg->name) + 1;
	free(arg->name);
	return (0);
}

/**
 * handle_new_expand - Appends a newly expanded variable value to the result.
 *
 * @temp: Temporary string containing the value to append.
 * @expan: Pointer to the string accumulating the expanded result.
 *
 * Concatenates the temporary value to the current expanded result,
 * handling memory
 * management for both strings. Returns 0 on success or -1 on failure.
 */
static int	handle_new_expand(char *temp, char **expan)
{
	char	*new_expanded;

	new_expanded = ft_strjoin(*expan, temp);
	if (!new_expanded)
	{
		ft_putendl_fd("malloc failed", 2);
		return (-1);
	}
	free(temp);
	free(*expan);
	*expan = new_expanded;
	return (0);
}

/**
 * init_vdata - Initializes a t_vdata structure for expansion processing.
 *
 * @data: Pointer to the t_vdata structure to initialize.
 * @expan: Pointer to the string accumulating the expanded result.
 * @temp: Temporary string used during expansion.
 * @name: Name of the variable being expanded.
 *
 * Prepares the t_vdata structure with pointers to relevant data
 * for the expansion process.
 */
static void	init_vdata(t_vdata *data, char **expan, char *temp, char *name)
{
	data->value = NULL;
	data->expan = expan;
	data->temp = temp;
	data->name = name;
}
