/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 17:49:30 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/23 19:12:12 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			heredoc_expander(t_shell *mini, char **line);
int			check_expand(t_shell *mini, t_cmd *cmd, char **line, int fd);
static int	finalize_hd_expand(t_expand *arg, char *expan, char **line);
static int	init_heredoc_expander(t_expand *arg, char **expan);

int	heredoc_expander(t_shell *mini, char **line)
{
	t_expand	arg;
	char		*expan;

	if (init_heredoc_expander(&arg, &expan))
		return (1);
	while ((*line)[arg.i])
	{
		if ((*line)[arg.i] == '$' && (*line)[arg.i + 1]
			&& ft_isalnum((*line)[arg.i + 1]))
		{
			if (we_have_dollar(mini, &arg, *line) == -1)
			{
				hd_free(&arg, expan);
				return (-1);
			}
		}
		else if (add_char(*line, &arg))
		{
			hd_free(&arg, expan);
			return (-1);
		}
	}
	if (finalize_hd_expand(&arg, expan, line))
		return (1);
	return (0);
}

int	check_expand(t_shell *mini, t_cmd *cmd, char **line, int fd)
{
	if (cmd->redir_tail->expand)
	{
		if (heredoc_expander(mini, line))
		{
			free(line);
			close(fd);
			return (1);
		}
	}
	return (0);
}

static int	finalize_hd_expand(t_expand *arg, char *expan, char **line)
{
	char	*temp;

	temp = ft_strjoin(expan, arg->value);
	free(arg->value);
	free(expan);
	if (!temp)
		return (-1);
	free(*line);
	*line = temp;
	return (0);
}

static int	init_heredoc_expander(t_expand *arg, char **expan)
{
	if (the_arg(arg, 0))
		return (1);
	*expan = ft_strdup("");
	if (!(*expan))
		return (1);
	return (0);
}
