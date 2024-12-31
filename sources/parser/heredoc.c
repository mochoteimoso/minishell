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
static int	restore_and_cleanup(t_shell *mini, int fd, int exit_code);
static int	init_heredoc(t_cmd *cmd, int *fd);
static int	process_heredoc_line(t_shell *mini, t_cmd *cmd, char *line, int fd);
static void	write_close_hd(char *line, int fd, int end);

int	open_and_write_to_heredoc(t_shell *mini, t_cmd *cmd)
{
	int		fd;
	char	*line;
	int		result;

	mini->stdin_saved = dup(STDIN_FILENO);
	if (mini->stdin_saved == -1)
	{
		perror("Failed to save STDIN");
		return (1);
	}
	if (init_heredoc(cmd, &fd))
		return (restore_and_cleanup(mini, fd, 1));
	while (1)
	{
		line = readline("heredoc> ");
		if (g_sig == SIGINT)
			return (restore_and_cleanup(mini, fd, 1));
		result = process_heredoc_line(mini, cmd, line, fd);
		if (result == 1)
			break ;
		if (result == -1)
			return (restore_and_cleanup(mini, fd, 1));
	}
	write_close_hd(line, fd, 1);
	return (restore_and_cleanup(mini, fd, 0));
}

static int	restore_and_cleanup(t_shell *mini, int fd, int exit_code)
{
	if (fd != -1)
		close(fd);
	if (mini->stdin_saved != -1)
	{
		if (dup2(mini->stdin_saved, STDIN_FILENO) == -1)
			perror("Failed to restore original STDIN");
		close(mini->stdin_saved);
		mini->stdin_saved = -1;
	}
	return (exit_code);
}

static int	init_heredoc(t_cmd *cmd, int *fd)
{
	*fd = open(cmd->redir_tail->heredoc_name, O_RDWR | O_CREAT | O_EXCL, 0600);
	if (*fd == -1)
	{
		ft_putendl_fd("Failed to open temp file for heredoc", 2);
		return (1);
	}
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
	write_close_hd(line, fd, 0);
	return (0);
}

static void	write_close_hd(char *line, int fd, int end)
{
	if (end)
	{
		close(fd);
		return ;
	}
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
	free(line);
}
