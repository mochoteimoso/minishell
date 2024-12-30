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
static int	process_heredoc_line(t_shell *mini, t_cmd *cmd, char *line, int fd);
static int	handle_sigint_in_hd(t_shell *mini, int fd);
static int	stdin_saver(t_shell *mini);

int	open_and_write_to_heredoc(t_shell *mini, t_cmd *cmd)
{
	int		fd;
	char	*line;
	int		result;

	if (init_heredoc(mini, cmd, &fd))
		return (1);
	while (1)
	{
		line = readline("heredoc> ");
		if (g_sig == SIGINT)
			return (handle_sigint_in_hd (mini, fd));
		result = process_heredoc_line(mini, cmd, line, fd);
		if (result == 1)
			break ;
		if (result == -1)
			return (1);
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

static int	process_heredoc_line(t_shell *mini, t_cmd *cmd, char *line, int fd)
{
	if (!line || ft_strcmp(line, cmd->redir_tail->delimiter) == 0)
	{
		free(line);
		return (1);
	}
	if (check_expand(mini, cmd, &line, fd))
	{
		close(fd);
		return (-1);
	}
	write_close_hd(mini, line, fd, 0);
	return (0);
}

static int	handle_sigint_in_hd(t_shell *mini, int fd)
{
	if (stdin_saver(mini))
	{
		close(fd);
		return (1);
	}
	close(fd);
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
			close(mini->stdin_saved);
			mini->exit_stat = 1;
			mini->stdin_saved = -1;
			return (1);
		}
		mini->exit_stat = 1;
	}
	mini->stdin_saved = dup(STDIN_FILENO);
	if (mini->stdin_saved == -1)
	{
		mini->exit_stat = 1;
		perror("Failed to save STDIN");
		return (1);
	}
	return (0);
}
