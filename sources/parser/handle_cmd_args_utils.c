/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd_args_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 18:12:21 by henbuska          #+#    #+#             */
/*   Updated: 2024/11/26 19:47:49 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	skip_whitespace(char *str, int i)
{
	while (str[i] && ft_isspace(str[i]))
		i++;
	return (i);
}

// int	arg_in_quotes(t_shell *mini, char *str, int i, char **start, int *len)
// {
// 	char	quote;
// 	char	*res;
// 	char	*temp;
// 	char	*new_res;
// 	int		start_exp;

// 	printf("at the beginning str: {%s}\n", str);
// 	quote = str[i];
// 	res = ft_strdup("");
// 	i++;
// 	while (str[i] && str[i] != quote)
// 	{
// 		if (quote == '"' && str[i] == '$')
// 		{
// 			start_exp = i;
// 			temp = ft_strdup("");
// 			i = expand_variable(mini, str, i, &temp, &start_exp);
// 			new_res = ft_strjoin(res, temp);
// 			free(res);
// 			free(temp);
// 			res = new_res;
// 		}
// 		else
// 		{
// 			temp = ft_strjoin(res, ft_strndup(&str[i], 1));
// 			free(res);
// 			res = temp;
// 		}
// 		if (str[i + 1] != )
// 	}
// 	printf("at the end str: {%s}\n", res);
// 	if (str[i] == quote)
// 		i++;
// 	*start = res;
// 	*len = ft_strlen(res);
// 	return (i);
// }

// int	arg_in_quotes(t_shell *mini, char *str, int i, char **start, int *len)
// {
// 	char	quote;
// 	char	*res;
// 	char	*temp;
// 	char	*new_res;
// 	char	inner;
// 	int		inner_s;
// 	int		start_exp;

// 	printf("at the beginning str: {%s}\ni: %d\n", &str[i], i);
// 	quote = str[i];
// 	res = ft_strdup("");
// 	i++;
// 	while (str[i])
// 	{
// 		if (quote == '"' && str[i] == '$')
// 		{
// 			start_exp = i;
// 			temp = ft_strdup("");
// 			i = expand_variable(mini, str, i, &temp, &start_exp);
// 			new_res = ft_strjoin(res, temp);
// 			free(res);
// 			free(temp);
// 			res = new_res;
// 		}
// 		else if (str[i] == '\'' || str[i] == '"')
// 		{
// 			inner = str[i];
// 			inner_s = i;
// 			if (str[i + 1] != '\0' && str[i + 1] != ' ')
// 			{
// 				temp = ft_strjoin(res, ft_strndup(&str[i], 1));
// 				free(res);
// 				res = temp;
// 				i++;
// 			}
// 			else
// 			{
// 				i++;
// 				break;
// 			}
// 		}
// 		else
// 		{
// 			temp = ft_strjoin(res, ft_strndup(&str[i], 1));
// 			free(res);
// 			res = temp;
// 			i++;
// 		}
// 	}
// 	printf("at the end str: {%s}\n", res);
// 	*start = res;
// 	*len = ft_strlen(res);
// 	return (i);
// }

int	arg_in_quotes(t_shell *mini, char *str, int i, char **start, int *len)
{
	t_expand	arg;
	char		*temp;
	char		*new_res;
	int			s_exp;

	//printf("start: {%s}\nlen: %d\n", &str[i], *len);
	arg.sgl = 0;
	arg.dbl = 0;
	arg.i = i;
	arg.value = ft_strdup("");
	if (str[arg.i] == '\'')
		arg.sgl = 1;
	else if (str[arg.i] == '"')
		arg.dbl = 1;
	arg.i++;
	while (str[arg.i])
	{
		if (str[arg.i] == ' ' && !arg.sgl &&  !arg.dbl)
		{
			*start = arg.value;
			*len = ft_strlen(arg.value);
			return (arg.i);
		}
		if ((arg.dbl && str[arg.i] == '$') || (!arg.sgl && str[arg.i] == '$'))
		{
			s_exp = arg.i;
			temp = ft_strdup("");
			arg.i = expand_variable(mini, str, arg.i, &temp, &s_exp);
			new_res = ft_strjoin(arg.value, temp);
			free(arg.value);
			free(temp);
			arg.value = new_res;
		}
		else if ((arg.sgl && str[arg.i] == '\'') || (arg.dbl && str[arg.i] == '"'))
		{
			if ((arg.sgl && str[arg.i] == '\'') || (arg.dbl && str[arg.i] == '"'))
			{
				if (str[arg.i + 1] != '\0' || str[arg.i] != ' ')
				{
					arg.sgl = 0;
					arg.dbl = 0;
					arg.i++;
				}
				else
				{
					arg.i++;
					break;
				}
			}
		}
		else if (!arg.sgl && !arg.dbl && (str[arg.i] == '\'' || str[arg.i] == '"'))
		{
			if (str[arg.i] == '\'')
				arg.sgl = 1;
			else if (str[arg.i] == '"')
				arg.dbl = 1;
			arg.i++;
		}
		else
		{
			temp = ft_strjoin(arg.value, ft_strndup(&str[arg.i], 1));
			free(arg.value);
			arg.value = temp;
			arg.i++;
		}
	}
	*start = arg.value;
	*len = ft_strlen(arg.value);
	return (arg.i);
}

int	arg_no_quotes(t_cmd *cmd, int i, char **start, int *len)
{
	*start = &cmd->segment[i];
	*len = 0;

	while (cmd->segment[i] && (!ft_isspace(cmd->segment[i]) ||
		check_quotes(cmd->segment, i)) && !is_redirection(cmd, i))
	{
		(*len)++;
		i++;
	}
	return (i);
}

int	append_to_array(t_cmd *cmd, char *start, int len, int *index)
{
	cmd->args[*index] = ft_strndup(start, len);
	if (!cmd->args[*index])
	{
		ft_putendl_fd("Failed to allocate memory for argument", 2);
		return (-1);
	}
	(*index)++;
	return (0);
}
