/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzharkev <nzharkev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 19:03:42 by henbuska          #+#    #+#             */
/*   Updated: 2025/01/03 10:20:05 by nzharkev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/includes/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <limits.h>
# include <stdbool.h>
# include <stdlib.h>
# include <errno.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>

# define TMP_S "/tmp/heredoc"
# define TMP_EXT ".tmp"

extern int	g_sig;

/**
 * e_redir_type - Enum representing the types of redirections.
 *
 * Members:
 * - REDIRECT_IN: Input redirection using '<'.
 * - REDIRECT_OUT: Output redirection using '>'.
 * - APPEND: Output redirection in append mode using '>>'.
 * - HEREDOC: Heredoc redirection using '<<'.
 */
typedef enum e_redir_type
{
	REDIRECT_IN,
	REDIRECT_OUT,
	APPEND,
	HEREDOC
}	t_redir_type;

/**
 * s_hd - Structure for heredoc-related metadata.
 *
 * Members:
 * - cmd_str: Command identifier string.
 * - heredoc_str: Heredoc-specific identifier string.
 * - base: Base string for heredoc file naming.
 * - mid: Intermediate string for heredoc file naming.
 * - full: Full string for the heredoc file path.
 */
typedef struct s_hd
{
	char	*cmd_str;
	char	*heredoc_str;
	char	*base;
	char	*mid;
	char	*full;
}	t_hd;

/**
 * s_vdata - Structure for variable expansion data.
 *
 * Members:
 * - value: The expanded value of the variable.
 * - expan: Pointer to the current expansion result.
 * - temp: Temporary buffer for intermediate operations.
 * - name: Name of the variable being expanded.
 */
typedef struct s_vdata
{
	char	*value;
	char	**expan;
	char	*temp;
	char	*name;
}	t_vdata;

/**
 * s_expand - Structure for tracking the state of variable expansion.
 *
 * Members:
 * - sgl: Single quote state (1 if inside single quotes, 0 otherwise).
 * - dbl: Double quote state (1 if inside double quotes, 0 otherwise).
 * - i: Current position in the string being processed.
 * - name: Name of the variable being expanded.
 * - value: Value of the expanded variable.
 * - start: Starting index of the current expansion.
 * - len: Length of the expanded result.
 */
typedef struct s_expand
{
	int		sgl;
	int		dbl;
	int		i;
	char	*name;
	char	*value;
	int		start;
	int		len;
}	t_expand;

/**
 * s_redir - Linked list node representing redirections for a command.
 *
 * Members:
 * - file: The file name for input/output redirection.
 * - delimiter: The delimiter string for heredocs.
 * - type: Type of redirection (from `t_redir_type` enum).
 * - expand: Boolean indicating whether variable expansion is enabled.
 * - node_ind: Index of the redirection node.
 * - heredoc_name: Name of the heredoc temporary file.
 * - heredoc_index: Index of the heredoc in the command.
 * - next: Pointer to the next redirection node in the list.
 */
typedef struct s_redir
{
	char			*file;
	char			*delimiter;
	t_redir_type	type;
	bool			expand;
	int				node_ind;
	char			*heredoc_name;
	int				heredoc_index;
	struct s_redir	*next;
}	t_redir;

/**
 * s_cmd - Structure representing a parsed command.
 *
 * Members:
 * - seg: The original command segment string.
 * - command: The name of the command.
 * - cmd_path: The full path to the command executable.
 * - cmd_index: Index of the command in the pipeline.
 * - args: Array of arguments for the command.
 * - a_num: Number of arguments in the `args` array.
 * - redir_head: Pointer to the head of the redirection list.
 * - redir_tail: Pointer to the tail of the redirection list.
 * - fd_in: File descriptor for input redirection.
 * - fd_out: File descriptor for output redirection.
 * - cmd_exit: Exit status of the command after execution.
 */
typedef struct s_cmd
{
	char	*seg;
	char	*command;
	char	*cmd_path;
	int		cmd_index;
	char	**args;
	int		a_num;
	t_redir	*redir_head;
	t_redir	*redir_tail;
	int		fd_in;
	int		fd_out;
	int		cmd_exit;
}	t_cmd;

/**
 * s_env - Linked list node representing an environment variable.
 *
 * Members:
 * - name: The name of the environment variable.
 * - value: The value of the environment variable.
 * - next: Pointer to the next environment variable node.
 */
typedef struct s_env
{
	char			*name;
	struct s_env	*next;
	char			*value;

}	t_env;

/**
 * s_shell - Main structure for the shell state.
 *
 * Members:
 * - cmds: Array of command structures for the pipeline.
 * - env: Linked list of environment variables.
 * - cmd_count: Number of commands in the pipeline.
 * - pending: Array of pending environment variable declarations.
 * - pids: Array of process IDs for forked commands.
 * - pipes: 2D array of pipes for inter-process communication.
 * - stdin_saved: File descriptor for the saved standard input.
 * - stdout_saved: File descriptor for the saved standard output.
 * - exit_stat: Exit status of the last executed command.
 */
typedef struct s_shell
{
	t_cmd	**cmds;
	t_env	*env;
	int		cmd_count;
	char	**pending;
	int		*pids;
	int		**pipes;
	int		stdin_saved;
	int		stdout_saved;
	int		exit_stat;
}	t_shell;

/*built_in*/
	/*cd/cd.c*/

int		built_cd(t_shell *mini, t_cmd *cmd);

	/*cd/cd_utils.c*/

int		get_oldpwd(t_env *env, char **pwd);
int		update_env_value(t_env *env, char *new_value);
int		update_pwd(t_env *env, char *wd, char **oldpwd);
int		handle_update_pwd(t_shell *mini, char *pwd, char *oldpwd);

	/*cd_utils2.c*/

int		no_cwd(t_shell *mini, t_env *pwd, char **cwd);
int		handle_path(t_shell *mini, char *oldpwd, char *path);

	/*echo.c*/

int		built_echo(t_cmd *cmd);

	/*env/env.c*/

char	**env_to_array(t_env *env);
int		built_env(t_shell *mini, t_cmd *cmd);

	/*env/env_handling*/

char	**copy_env(char **envp);
t_env	*list_env(char **envp);
void	to_alphabetical(char **array);

	/*env/env_ll*/

t_env	*add_node(char *env);
t_env	*create_node(void);
void	ft_env_lstadd_back(t_env **lst, t_env *new);

	/*env/env_utils.c*/

int		set_value(t_env *node, char *value);
int		fill_node(t_env *node, char *name, char *value);

	/*exit.c*/

int		built_exit(t_shell *mini, t_cmd *cmd);

	/*export/export.c*/

int		built_export(t_shell *mini, t_cmd *cmd);

	/*export/export_update.c*/

int		handle_sign(t_shell *mini, char *str);
int		update_env(t_shell *mini, char *str);
int		update_pending(t_shell *mini, char *name, char *str);

	/*pwd.c*/

int		built_pwd(t_shell *mini);
t_env	*find_pwd(t_env *env, char *name);

	/*unset.c*/

int		built_unset(t_shell *mini, t_cmd *cmd);

/*commands*/
	/*cmd_array.c*/

int		count_pipes(char *line);
int		prepare_command_structs(t_shell *mini, char *input);
void	initialize_command_struct(t_cmd *cmd);

/*executor*/
	/*child_process.c*/

int		fork_and_execute(t_shell *mini, t_cmd *cmd, int i);
void	close_unused_fds(t_shell *mini, int i);

	/*child_process_utils.c*/

int		dup_input(t_shell *mini, t_cmd *cmd, int i);
int		dup_output(t_shell *mini, t_cmd *cmd, int count, int i);
int		dup2_and_close(int old_fd, int new_fd);

	/*cmd_path.c*/

int		get_cmd_path(t_shell *mini, t_cmd *cmd);

	/*cmd_path_utils.c*/

int		check_special_cases(t_cmd *cmd);
int		check_for_directory(t_cmd *cmd);
void	cmd_error_and_exit_stat(t_cmd *cmd, int exit_status);
int		check_access(t_cmd *cmd);

	/*fd_handlers.c*/

int		save_fds(t_shell *mini);
int		reset_fds(t_shell *mini);

	/*handle_builtins.c*/

int		built_in_exe(t_shell *mini, t_cmd *cmd);
int		is_this_built(char *str);

	/*pipeline.c*/

int		execute_pipeline(t_shell *mini);

	/*pipeline_utils.c*/

int		create_pipes(t_shell *mini);
int		dup2_and_close_in_main(t_shell *mini, int old_fd, int new_fd);
void	close_fds_and_pipes(t_shell *mini, int i);
void	wait_children(t_shell *mini);
void	unlink_all_heredocs(t_shell *mini);

	/*pipe;ine_utils2.c*/

int		redirect_fd(int src_fd, int target_fd);

/*parser*/
	/*expand.c*/

int		handle_expand(t_shell *mini, t_cmd **cmd);

	/*expand_no_expand.c*/

int		no_expanding(t_shell *mini, char *str, t_expand *arg);

	/*expand_quoted.c*/

int		in_quotes(t_shell *mini, char *str, int i, t_expand *arg);
int		we_have_heredoc(t_expand *arg, char *str, int n);

	/*expand_unquoted.c*/

int		s_unquoted(t_shell *mini, t_cmd **cmd, t_expand *arg, char **expan);
int		no_quotes(t_shell *mini, t_cmd *cmd, int i, t_expand *arg);

	/*expand_utils.c*/

int		we_have_dollar(t_shell *mini, t_expand *arg, char *str);
int		oh_a_dollar(t_shell *mini, char *str, char **expan, t_expand *arg);

	/*expand_utils2.c*/

int		tildes_home(t_shell *mini, char *str, char **expan, t_expand *arg);
int		handle_value(t_shell *mini, t_vdata *data);
char	*get_value(t_env *env, char *name);
char	*ft_strjoin_char(char *str, char c);

	/*expand_utils3.c*/

int		init_expansion(t_expand *arg, char **expan);
int		the_arg(t_expand *arg, int i);
void	what_quote(char *str, t_expand *arg);
int		handle_question(t_shell *mini, char *str, char **expan, t_expand *arg);
int		new_result(t_expand *arg, char *temp);

	/*expand_utils4.c*/

void	just_a_quest(char *str, char *name, int *indx, t_expand *arg);
void	we_need_name(t_expand *arg, char *str, char *name, int *indx);

	/*handle_cmd_args.c*/

int		handle_cmd_args(t_shell *mini, t_cmd *cmd, int i);

	/*handle_cmd_args_utils.c*/

int		skip_whitespace(char *str, int i);
int		arg_no_quotes(t_cmd *cmd, t_expand *arg, int i);
int		arg_in_quotes(char *str, int i, t_expand *arg);
int		append_to_array(t_cmd *cmd, char *arg, int *index);

	/*handle_cmd_args_utils2.c*/

int		count_args(t_cmd *cmd, int i);
int		count_if_redirection(t_cmd *cmd, int i);
int		only_redirect(char *str, int i);
int		init_args_array(t_cmd *cmd, int i);

	/*handle_cmd_name.c*/

int		handle_cmd_name(t_cmd *cmd, int i);

	/*handle_redirections.c*/

int		handle_redirections(t_shell *mini, t_cmd *cmd, int i);

	/*handle_redirs_utils.c*/

bool	is_redirection(t_cmd *cmd, int i);
int		handle_redirect_in(t_cmd *cmd, int i);
int		handle_redirect_out(t_cmd *cmd, int i);
int		handle_heredoc(t_shell *mini, t_cmd *cmd, int i);
int		handle_append(t_cmd *cmd, int i);

	/*heredoc.c*/

int		open_and_write_to_heredoc(t_shell *mini, t_cmd *cmd);
int	restore_and_cleanup(t_shell *mini, int fd, int exit_code);

	/*heredoc_expand*/

int		heredoc_expander(t_shell *mini, char **line);
int		check_expand(t_shell *mini, t_cmd *cmd, char **line, int fd);

	/*heredoc_file.c*/

int		generate_hd_file(t_cmd *cmd);

	/*parser.c*/

int		parse_and_validate_input(char **input, t_shell *mini);
int		parse_input(t_shell *mini);
int		parse_cmd_string(t_shell *mini, t_cmd *cmd);
int		cmd_string_while(t_shell *mini, t_cmd *cmd, int i, int *cmd_found);

	/*parser_utils.c*/

int		no_args(t_cmd *cmd, int i);
bool	is_empty_command(t_cmd *cmd, int i);
int		add_char(char *str, t_expand *arg);

	/*split_inputs.c*/

int		split_input_by_pipes(char *input, t_shell *mini);
char	*trim_whitespace(char *seg);

	/*find_cmd_path.c*/

int		get_cmd_path(t_shell *mini, t_cmd *cmd);

/*redirection*/
	/*get_filename.c*/

int		parse_filename(t_cmd *cmd, int i, char **filename);
int		filename_in_quotes(t_cmd *cmd, char *str, int i, t_expand *arg);

	/*open_files.c*/

int		open_input_file(t_cmd *cmd, char *input_file);
int		open_output_file(t_cmd *cmd, char *output_file);
int		open_append_file(t_cmd *cmd, char *output_file);
int		open_heredoc(t_cmd *cmd, char *delimiter);

	/*redir_ll*/

int		redirll_head_tail(t_cmd *cmd);

	/*redirector.c*/

int		resolve_fd(t_cmd *cmd);

/*signals.c*/
	/*signals.c*/

void	init_sig(void);
void	sig_reseted(void);
void	sig_handler_changer(void);
void	sig_heredoc(void);
void	sig_handler_hd(int signal);

	/*signals_utils.c*/

void	sigint_handler(int sig);
void	sig_handler2(int sig);
void	sig_handler_heredoc(int signum);

/*syntax*/
	/*pipe_syntax*/

int		check_pipes(char **input, t_shell *mini);

	/*redirection_syntax.c*/

int		check_redirects(char *input, t_shell *mini);

	/*syntax_checker.c*/

int		validate_input_syntax(char **input, t_shell *mini);
int		check_quotes(char *input, int limit);
int		is_this_empty(char *input);

	/*trailing_pipe.c*/

char	*handle_trailing_pipe(t_shell *mini, char *input);

/*utils*/
	/*cleaners.c*/

void	mini_cleaner(t_shell *mini);
void	clean_cmds(t_cmd **cmds);
void	cleaner_for_success(t_shell *mini);
void	cleaner_for_failure(t_shell *mini);

	/*exit_handler.c*/

void	exit_for_failure(t_shell *mini, int i, int exit_status);
void	exit_for_success(t_shell *mini, int i, int exit_status);
void	exit_for_single_cmd(t_shell *mini, int exit_status);
void	hd_free(t_expand *arg, char *expan);

	/*freeing*/

void	ft_free_int_arr(int **array);
void	ft_free_int_arr_with_size(int **array, int size);
void	error(t_shell *mini, char *str);
void	clean_env(t_env *ll, char **array);
void	clean_redir(t_redir *head);

#endif
