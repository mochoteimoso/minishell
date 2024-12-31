/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handlers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 13:43:57 by nzharkev          #+#    #+#             */
/*   Updated: 2024/12/31 11:23:19 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	init_sig(void);
void	sig_handler_changer(void);
void	sig_reseted(void);
void	sig_heredoc(void);
void	sig_handler_hd(int signal);

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

void	sig_heredoc(void)
{
	signal(SIGINT, sig_handler_heredoc);
	signal(SIGQUIT, SIG_IGN);
}

void	sig_handler_hd(int signal)
{
	close(STDIN_FILENO);
	g_sig = signal;
}
