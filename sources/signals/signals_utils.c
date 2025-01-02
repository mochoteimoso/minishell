/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 11:48:43 by nzharkev          #+#    #+#             */
/*   Updated: 2025/01/02 12:56:17 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	sigint_handler(int sig);
void	sig_handler2(int sig);
void	sig_handler_heredoc(int signum);

/**
 * sigint_handler - Handles the SIGINT signal (Ctrl+C)
 * 					during the main shell prompt.
 *
 * @sig: Signal number (expected to be SIGINT).
 *
 * Clears the current line, moves to a new line, and redisplays the prompt.
 * This allows the user to gracefully interrupt their input and begin typing
 * a new command without exiting the shell.
 */
void	sigint_handler(int sig)
{
	if (sig == SIGINT)
	{
		rl_replace_line("", 0);
		printf("\n");
		rl_on_new_line();
		rl_redisplay();
	}
}

/**
 * sig_handler2 - Handles SIGINT and SIGQUIT signals
 * 				  during child process execution.
 *
 * @sig: Signal number (SIGINT or SIGQUIT).
 *
 * Prints a newline for SIGINT or a "Quit (core dumb)" message for SIGQUIT.
 * This handler ensures that child processes react
 * appropriately to these signals.
 */
void	sig_handler2(int sig)
{
	if (sig == SIGINT)
		printf("\n");
	if (sig == SIGQUIT)
		ft_putendl_fd("Quit (core dumb)", 2);
}

/**
 * sig_handler_heredoc - Handles SIGINT signal (Ctrl+C) during heredoc input.
 *
 * @signum: Signal number (expected to be SIGINT).
 *
 * Sets a global variable `g_sig` to indicate the signal was received, clears the
 * current line, moves to a new line, and redisplays the heredoc prompt. This
 * allows the user to interrupt the heredoc input and the program to handle the
 * signal gracefully.
 */
void	sig_handler_heredoc(int signum)
{
	if (signum == SIGINT)
	{
		g_sig = signum;
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}
