/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henbuska <henbuska@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 13:57:16 by henbuska          #+#    #+#             */
/*   Updated: 2024/12/18 21:11:45 by henbuska         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	generate_hd_file(t_cmd *cmd);
int	open_and_write_to_heredoc(t_shell *mini, t_cmd *cmd);
int	heredoc_expander(t_shell *mini, char **line);

int	generate_hd_file(t_cmd *cmd)
{
	char	*cmd_str;
	char	*heredoc_str;
	char	*base;
	char	*mid;
	char	*full;

	cmd_str = ft_itoa(cmd->cmd_index);
	if (!cmd_str)
		return (1);
	heredoc_str = ft_itoa(cmd->redir_head->heredoc_index);
	if (!heredoc_str)
	{
		free(cmd_str);
		return (1);
	}
	base = ft_strjoin(TMP_S, cmd_str);
	if (!base)
	{
		free(cmd_str);
		free(heredoc_str);
		return (1);
	}
	free(cmd_str);
	mid = ft_strjoin(base, "_");
	if (!mid)
	{
		free(heredoc_str);
		free(base);
		return (1);
	}
	free(base);
	full = ft_strjoin(mid, heredoc_str);
	if (!full)
	{
		free(mid);
		free(heredoc_str);
		return (1);
	}
	free(mid);
	free(heredoc_str);
	cmd->redir_tail->heredoc_name = ft_strjoin(full, TMP_EXT);
	if (!cmd->redir_tail->heredoc_name)
	{
		free(full);
		return (1);
	}
	free(full);
	unlink(cmd->redir_tail->heredoc_name);
	return (0);

}

int	open_and_write_to_heredoc(t_shell *mini, t_cmd *cmd)
{
	int		fd;
	char	*line;

	fd = open(cmd->redir_tail->heredoc_name, O_RDWR | O_CREAT | O_EXCL, 0600);
	if (fd == -1)
	{
		ft_putendl_fd("Failed to open temp file for heredoc", 2);
		return (1);
	}
	mini->stdin_saved = dup(STDIN_FILENO);
	if (mini->stdin_saved == -1)
	{
		mini->exit_stat = 1;
		return (perror("Failed to save STDIN"), 1);
	}
	signal(SIGINT, sig_handler_hd);
	while (1)
	{
		line = readline("heredoc> ");
		if (g_sig == SIGINT)
		{
			g_sig = 0;
			printf("\n");
			close(fd);
			if (dup2_and_close(mini->stdin_saved, STDIN_FILENO))
			{
				perror("Failed to restore original STDIN");
				mini->exit_stat = 1;
				mini->stdin_saved = -1;
				return (1);
			}
			return (0);
		}
		if (!line || ft_strcmp(line,cmd->redir_tail->delimiter) == 0)
		{
			free(line);
			break;
		}
		if (cmd->redir_tail->expand)
		{
			if (heredoc_expander(mini, &line))
			{
				free(line);
				close(fd);
				return (1);
			}
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close(mini->stdin_saved);
	close(fd);
	return (0);
}

int	heredoc_expander(t_shell *mini, char **line)
{
	t_expand	arg;
	char		*expanded;
	char		*temp;

	if (the_arg(&arg, 0))
		return (-1);
	expanded = ft_strdup("");
	if (!expanded)
		return (-1);
	while ((*line)[arg.i])
	{
		if ((*line)[arg.i] == '$' && (*line)[arg.i + 1]
			&& ft_isalnum((*line)[arg.i + 1]))
		{
			if (we_have_dollar(mini, &arg, *line) == -1)
			{
				free(arg.value);
				free(expanded);
				return (-1);
			}
		}
		else
		{
			if (add_char(*line, &arg))
			{
				free(arg.value);
				free(expanded);
				return (-1);
			}
		}
	}
	temp = ft_strjoin(expanded, arg.value);
	free(arg.value);
	free(expanded);
	if (!temp)
		return (-1);
	free(*line);
	*line = temp;
	return (0);
}
