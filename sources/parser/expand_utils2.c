/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 09:18:09 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/31 17:14:03 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			tildes_home(t_shell *mini, char *str, char **expan, t_expand *arg);
int			handle_value(t_shell *mini, t_vdata *data);
char		*get_value(t_env *env, char *name);
char		*ft_strjoin_char(char *str, char c);
static int	we_have_value(char *value, char *temp, char **expan);

/**
 * tildes_home - Expands the '~' character to the user's home directory.
 *
 * @mini: Pointer to the shell structure for accessing environment variables.
 * @str: The input string being processed.
 * @expan: Pointer to the string used to accumulate the expanded result.
 * @arg: Pointer to the t_expand structure tracking expansion state.
 *
 * Replaces '~' with the value of the HOME environment variable, if it exists.
 * Handles memory allocation and concatenation for the expanded result.
 * Returns the updated index on success or -1 on failure.
 */
int	tildes_home(t_shell *mini, char *str, char **expan, t_expand *arg)
{
	char	*temp;
	char	*temp2;
	char	*value;

	temp2 = ft_strndup(&str[arg->start], arg->i - arg->start);
	temp = ft_strjoin(*expan, temp2);
	free(temp2);
	free(*expan);
	*expan = temp;
	arg->i++;
	value = get_value(mini->env, "HOME");
	if (value)
	{
		temp = ft_strjoin(*expan, value);
		if (!temp)
		{
			free(value);
			return (-1);
		}
		free(*expan);
		*expan = temp;
		free(value);
	}
	return (arg->i);
}

/**
 * handle_value - Handles expansion for variables and special characters.
 *
 * @mini: Pointer to the shell structure for accessing environment variables
 * 		  and status.
 * @data: Pointer to the t_vdata structure containing the variable name
 * 		  and result.
 *
 * Determines whether to expand a regular variable or special cases like '$?'.
 * Retrieves the variable's value and appends it to the expanded result.
 * Returns 0 on success, 1 on failure.
 */
int	handle_value(t_shell *mini, t_vdata *data)
{
	if (data->name[0] == '?')
	{
		data->value = ft_itoa(mini->exit_stat);
		if (!data->value)
			return (1);
		if (we_have_value(data->value, data->temp, data->expan) == -1)
			return (1);
		return (0);
	}
	data->value = get_value(mini->env, data->name);
	if (data->value == (char *)-1)
		return (1);
	if (we_have_value(data->value, data->temp, data->expan) == -1)
		return (1);
	return (0);
}

/**
 * get_value - Retrieves the value of an environment variable.
 *
 * @env: Pointer to the linked list of environment variables.
 * @name: Name of the variable to look up.
 *
 * Searches the environment list for a variable matching the name and returns
 * a newly allocated string containing its value. Returns an empty string
 * if the variable is not found, or (char *)-1 on allocation failure.
 */
char	*get_value(t_env *env, char *name)
{
	t_env	*temp;
	char	*value;

	temp = env;
	while (temp)
	{
		if (ft_strcmp(temp->name, name) == 0)
		{
			value = ft_strdup(temp->value);
			if (!value)
			{
				ft_putendl_fd("malloc fail", 2);
				return ((char *) -1);
			}
			return (value);
		}
		temp = temp->next;
	}
	value = ft_strdup("");
	if (!value)
	{
		ft_putendl_fd("malloc fail", 2);
		return ((char *) -1);
	}
	return (value);
}

/**
 * we_have_value - Appends a variable's value to the current expansion result.
 *
 * @value: The value to append.
 * @temp: Temporary string used for concatenation.
 * @expan: Pointer to the string accumulating the expanded result.
 *
 * Joins the current expansion result with the new value, handling memory
 * management for the concatenated string. Returns 0 on success or -1 on failure.
 */
static int	we_have_value(char *value, char *temp, char **expan)
{
	temp = ft_strjoin(*expan, value);
	free(value);
	if (!temp)
	{
		ft_putendl_fd("malloc failed", 2);
		return (-1);
	}
	free(*expan);
	*expan = temp;
	return (0);
}

/**
 * ft_strjoin_char - Appends a single character to a string.
 *
 * @str: The original string.
 * @c: The character to append.
 *
 * Allocates a new string, copies the contents of the original string, appends
 * the character, and null-terminates the result. Returns the new string or
 * NULL on allocation failure.
 */
char	*ft_strjoin_char(char *str, char c)
{
	size_t	len;
	char	*new_str;
	size_t	i;

	len = 0;
	if (str != NULL)
		len = ft_strlen(str);
	new_str = malloc(len + 2);
	if (new_str == NULL)
		return (NULL);
	i = 0;
	if (str != NULL)
	{
		while (str[i] != '\0')
		{
			new_str[i] = str[i];
			i++;
		}
	}
	new_str[i] = c;
	new_str[i + 1] = '\0';
	return (new_str);
}
