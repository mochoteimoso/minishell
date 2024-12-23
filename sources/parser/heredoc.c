/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 17:56:15 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/21 17:57:49 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			open_and_write_to_heredoc(t_shell *mini, t_cmd *cmd);
static int	init_heredoc(t_shell *mini, t_cmd *cmd, int *fd);
static int	stdin_saver(t_shell *mini);
static void	write_close_hd(t_shell *mini, char *line, int fd, int end);

int	open_and_write_to_heredoc(t_shell *mini, t_cmd *cmd)
{
	int		fd;
	char	*line;

	if (init_heredoc(mini, cmd, &fd))
		return (1);
	while (1)
	{
		line = readline("heredoc> ");
		if (g_sig == SIGINT)
		{
			if (stdin_saver(mini))
				return (1);
			return (0);
		}
		if (!line || ft_strcmp(line, cmd->redir_tail->delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (check_expand(mini, cmd, &line, fd))
			return (1);
		write_close_hd(mini, line, fd, 0);
	}
	write_close_hd(mini, line, fd, 1);
	return (0);
}

static int	init_heredoc(t_shell *mini, t_cmd *cmd, int *fd)
{
	*fd = open(cmd->redir_tail->heredoc_name, O_RDWR | O_CREAT | O_EXCL, 0600);
	if (*fd == -1)
	{
		ft_putendl_fd("Failed to open temp file for heredoc", 2);
		return (1);
	}
	if (stdin_saver(mini))
		return (1);
	signal(SIGINT, sig_handler_hd);
	return (0);
}

static int	stdin_saver(t_shell *mini)
{
	if (g_sig == SIGINT)
	{
		g_sig = 0;
		printf("\n");
		if (dup2_and_close(mini->stdin_saved, STDIN_FILENO))
		{
			perror("Failed to restore original STDIN");
			mini->exit_stat = 1;
			mini->stdin_saved = -1;
			return (1);
		}
	}
	mini->stdin_saved = dup(STDIN_FILENO);
	if (mini->stdin_saved == -1)
	{
		mini->exit_stat = 1;
		return (perror("Failed to save STDIN"), 1);
	}
	return (0);
}

static void	write_close_hd(t_shell *mini, char *line, int fd, int end)
{
	if (end)
	{
		close(mini->stdin_saved);
		close(fd);
		return ;
	}
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
	free(line);
}
