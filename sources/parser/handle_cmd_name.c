#include "../../includes/minishell.h"

int	handle_cmd_name(t_shell *mini, t_cmd *cmd, int i);
int	skip_to_next_segment(t_shell *mini, t_cmd *cmd, int i);
int	process_quoted_segment(t_shell *mini, char *segment, int i, t_expand *result);

int	handle_cmd_name(t_shell *mini, t_cmd *cmd, int i)
{
	t_expand cmd_name;

	i = skip_whitespace(cmd->segment, i);
	the_arg(&cmd_name, i);
	cmd_name.i = i;
	while (cmd->segment[cmd_name.i])
	{	if ((cmd->segment[cmd_name.i] == ' ' || cmd->segment[cmd_name.i] == '<'
			|| cmd->segment[cmd_name.i] == '>' || cmd->segment[cmd_name.i] == '|') && (!cmd_name.sgl && !cmd_name.dbl))
			break;
		if (cmd->segment[cmd_name.i] == '\'' || cmd->segment[cmd_name.i] == '"')
		{
			cmd_name.i = process_quoted_segment(mini, cmd->segment, cmd_name.i, &cmd_name);
			cmd->command = ft_strdup(cmd_name.value);
			return (cmd_name.i);
		}
		else if (add_char(cmd->segment, &cmd_name))
			return (free(cmd_name.value), -1);
	}
	if (cmd_name.i == -1)
		return (-1);
	cmd->command = ft_strdup(cmd_name.value);
	free(cmd_name.value);
	if (!cmd->command)
	{
		ft_putendl_fd("Failed to allocate memory for command name", 2);
		return (-1);
	}
	return (cmd_name.i);
}

int	process_quoted_segment(t_shell *mini, char *segment, int i, t_expand *result)
{
	(void)mini;
	if (the_arg(result, i))
		return (-1);
	what_quote(segment, result);
	while (segment[result->i])
	{
		if ((segment[result->i] == ' ' || segment[result->i] == '<'
			|| segment[result->i] == '>') && (!result->sgl && !result->dbl))
			break ;
		else if (!result->sgl && !result->dbl && (segment[result->i] == '\''
			|| segment[result->i] == '"'))
			what_quote(segment, result);
		else if ((result->sgl && segment[result->i] == '\'')
			|| (result->dbl && segment[result->i] == '"'))
			what_quote(segment, result);
		else if (add_char(segment, result))
			return (free(result->value), -1);
	}
	result->len = ft_strlen(result->value);
	return (result->i);
}

int	skip_to_next_segment(t_shell *mini, t_cmd *cmd, int i)
{
	while (cmd->segment[i])
	{
		i = skip_whitespace(cmd->segment, i);
		if (is_redirection(cmd, i))
		{
			i = handle_redirections(mini, cmd, i);
			if (i == -1)
				return (-1);
			continue;
		}
		if (!ft_isspace(cmd->segment[i]))
			break;
	}
	return (i);
}

// Retrieves command name from string and copies it to struct

/*int	handle_cmd_name(t_cmd *cmd, int i)
{
	char	*cmd_start;
	int		cmd_length;

	cmd_length = 0;
	while (cmd->segment[i] && ft_isspace(cmd->segment[i]))
		i++;
	if (cmd->segment[i] == '\'' || cmd->segment[i] == '"')
		i++;
	cmd_start = &cmd->segment[i];
 	while (cmd->segment[i] && !ft_isspace(cmd->segment[i]) &&
	!is_redirection(cmd, i))
	{
		cmd_length++;
		i++;
	}
	cmd->command = ft_strndup(cmd_start, cmd_length);
	if (!cmd->command)
	{
		ft_putendl_fd("Failed to allocate memory for command name", 2);
		return (-1);
	}
	return (i);
} */
