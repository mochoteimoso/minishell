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
int			restore_and_cleanup(t_shell *mini, int fd, int exit_code);
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

/**
 * restore_and_cleanup - Closes file descriptors and restores the saved stdin.
 *
 * @mini: Pointer to the shell structure.
 * @fd: File descriptor to close.
 * @exit_code: Exit code to return.
 *
 * Returns:
 * The provided exit code after performing cleanup.
 *
 * Details:
 * - Closes the provided file descriptor if it's valid.
 * - Restores the original stdin using `mini->stdin_saved`.
 * - Cleans up resources and resets the saved stdin to -1.
 */
int	restore_and_cleanup(t_shell *mini, int fd, int exit_code)
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
 * init_heredoc - Initializes a file for heredoc input and sets up signals.
 *
 * @cmd: Pointer to the command structure containing heredoc information.
 * @fd: Pointer to an integer to store the opened file descriptor.
 *
 * Returns:
 * 0 on success, 1 on failure.
 *
 * Details:
 * - Opens a temporary file for the heredoc with specific permissions.
 * - Prints an error message if the file cannot be opened.
 * - Configures the signal handler for SIGINT to handle heredoc interruptions.
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
 * write_close_hd - Writes a line to a file descriptor and handles cleanup.
 *
 * @line: Line to write to the file descriptor.
 * @fd: File descriptor to write to.
 * @end: Flag to indicate whether to close the file descriptor.
 *
 * Details:
 * - If `end` is true, closes the file descriptor.
 * - Otherwise, writes the line to the file,
 * 	appends a newline, and frees the line.
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
