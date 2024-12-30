/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trailing_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 12:32:24 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/30 15:39:50 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char		*handle_trailing_pipe(char *input);
static char	*join_and_free(char *input, char *additional_input);

/* Handles trailing pipe by getting additional input from user
and joining that to the original input string */

char	*handle_trailing_pipe(char *input)
{
	char	*additional_input;

	additional_input = NULL;
	signal(SIGINT, sigint_handler);
	while (1)
	{
		if (!additional_input)
		{
			free(input);
			return (NULL);
		}
		if (!is_this_empty(additional_input))
			return (join_and_free(input, additional_input));
		free(additional_input);
	}
}

static char	*join_and_free(char *input, char *additional_input)
{
	char	*updated_input;

	updated_input = ft_strjoin(input, additional_input);
	free(additional_input);
	if (!updated_input)
	{
		free(input);
		return (NULL);
	}
	free(input);
	return (updated_input);
}
