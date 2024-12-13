/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 13:43:57 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/13 11:50:36 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
 * sigint_handler - Signal handler for SIGINT (Ctrl-C).
 * Clears the current line, moves to a new line, and redisplays the prompt
 * to allow the user to start typing a new command after interrupting the current input.
 *
 * @sig: Signal number (expected to be SIGINT).
 */

static void	sigint_handler(int sig)
{
	if (sig == SIGINT)
	{
		rl_replace_line("", 0);
		printf("\n");
		rl_on_new_line();
		rl_redisplay();
	}
	if (sig == SIGQUIT)
		ft_putendl_fd("Quit", 2);
}

static void	sig_handler2(int sig)
{
	if (sig == SIGINT)
		printf("\n");
	if (sig == SIGQUIT)
		ft_putendl_fd("Quit", 2);
}

/*
 * init_sig - Initializes signal handling for the shell.
 * Sets the SIGINT signal (Ctrl-C) to be handled by sigint_handler
 * and ignores the SIGQUIT signal (Ctrl-\), making the shell responsive
 * to interrupts but unaffected by quit signals.
 */

void	init_sig(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	sig_handler_changer(void)
{
	signal(SIGINT, sig_handler2);
	signal(SIGQUIT, sig_handler2);
}

void	sig_reseted(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
