/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handlers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 13:43:57 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 11:43:24 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	init_sig(void);
void	sig_handler_changer(void);
void	sig_reseted(void);
void	sig_heredoc(void);
void	sig_handler_hd(int signal);

/**
 * init_sig - Initializes default signal handling for the shell's main prompt.
 *
 * Sets the SIGINT signal (Ctrl+C) to be handled by `sigint_handler`,
 * which allows graceful interruption of user input without
 * terminating the shell.
 * Ignores the SIGQUIT signal (Ctrl+\).
 */
void	init_sig(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

/**
 * sig_handler_changer - Updates signal handling for child processes.
 *
 * Sets both SIGINT (Ctrl+C) and SIGQUIT (Ctrl+\)
 * to be handled by `sig_handler2`.
 * This ensures proper behavior for processes executed by the shell,
 * such as displaying "Quit (core dumb)" for SIGQUIT
 * or a newline for SIGINT.
 */
void	sig_handler_changer(void)
{
	signal(SIGINT, sig_handler2);
	signal(SIGQUIT, sig_handler2);
}

/**
 * sig_reseted - Resets signal handling to the default behavior.
 *
 * Sets both SIGINT and SIGQUIT signals to
 * their default actions (SIG_DFL).
 * This is used for child processes to ensure they behave
 * as standard system commands.
 */
void	sig_reseted(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

/**
 * sig_heredoc - Configures signal handling specifically for heredoc input.
 *
 * Sets SIGINT (Ctrl+C) to be handled by `sig_handler_heredoc`, allowing the user
 * to gracefully interrupt heredoc input. SIGQUIT (Ctrl+\) remains ignored.
 */
void	sig_heredoc(void)
{
	signal(SIGINT, sig_handler_heredoc);
	signal(SIGQUIT, SIG_IGN);
}

/**
 * sig_handler_hd - Handles SIGINT during heredoc input.
 *
 * @signal: Signal number (expected to be SIGINT).
 *
 * Closes the standard input file descriptor
 * and updates the global `g_sig` variable to indicate that SIGINT was received.
 * This allows the heredoc processing to handle the interruption appropriately.
 */
void	sig_handler_hd(int signal)
{
	close(STDIN_FILENO);
	g_sig = signal;
}
