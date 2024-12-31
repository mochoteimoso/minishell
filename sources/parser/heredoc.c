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

/**
 * open_and_write_to_heredoc - Handles heredoc logic,
 * 							   including file creation and input processing.
 *
 * @mini: Pointer to the shell structure.
 * @cmd: Pointer to the current command structure.
 *
 * Creates a temporary file for the heredoc and processes input lines until the
 * delimiter is matched or an interruption occurs. Writes valid input lines to
 * the file. Returns 0 on success or 1 on failure.
 */
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

/**
 * init_heredoc - Initializes the heredoc by
 * 				  opening a temporary file and saving STDIN.
 *
 * @mini: Pointer to the shell structure.
 * @cmd: Pointer to the current command structure.
 * @fd: Pointer to store the file descriptor of the heredoc file.
 *
 * Opens a temporary file for the heredoc and saves the current STDIN state for
 * restoration. Sets up signal handling for SIGINT during heredoc input.
 * Returns 0 on success or 1 on failure.
 */
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

/**
 * process_heredoc_line - Processes each line input during heredoc.
 *
 * @mini: Pointer to the shell structure.
 * @cmd: Pointer to the current command structure.
 * @line: The input line to process.
 * @fd: File descriptor of the heredoc file.
 *
 * Handles each heredoc line by checking if it matches the delimiter, and writes
 * the expanded line (if applicable) to the file.
 * Frees the line memory as needed.
 * Returns 1 when the delimiter is matched, -1 on error,
 * or 0 to continue processing.
 */
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

/**
 * handle_sigint_in_hd - Handles SIGINT during heredoc processing.
 *
 * @mini: Pointer to the shell structure.
 * @fd: File descriptor of the heredoc file.
 *
 * Restores STDIN and closes the heredoc file when interrupted by SIGINT.
 * Returns 0 on success or 1 if restoration fails.
 */
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

/**
 * stdin_saver - Saves or restores the original STDIN during heredoc processing.
 *
 * @mini: Pointer to the shell structure.
 *
 * Saves the current STDIN to allow restoration after heredoc processing. If a
 * SIGINT occurs, it restores the original STDIN and updates the exit status.
 * Returns 0 on success or 1 on failure.
 */
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
