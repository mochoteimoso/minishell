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
	write_close_hd(line, fd, 0);
	return (0);
}

/**
 * write_close_hd - Writes a line to the heredoc file
 * 					or closes the file descriptor.
 *
 * @mini: Pointer to the shell structure.
 * @line: Line to write to the heredoc file.
 * @fd: File descriptor of the heredoc file.
 * @end: Indicates whether to write or close the file.
 *
 * If `end` is true, closes the heredoc file and restores STDIN. If false, writes
 * the line to the file, appending a newline character.
 * Frees the line after writing.
 */
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
