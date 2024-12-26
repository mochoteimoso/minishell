/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 11:48:43 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/26 11:54:03 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	sigint_handler(int sig);
void	sig_handler2(int sig);
void	sig_handler_heredoc(int signum);
/*
 * sigint_handler - Signal handler for SIGINT (Ctrl-C).
 * Clears the current line, moves to a new line, and redisplays the prompt
 * to allow the user to start typing a
 * new command after interrupting the current input.
 *
 * @sig: Signal number (expected to be SIGINT).
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

void	sig_handler2(int sig)
{
	if (sig == SIGINT)
		printf("\n");
	if (sig == SIGQUIT)
		ft_putendl_fd("Quit (core dumb)", 2);
}

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
