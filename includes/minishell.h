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
# include <fcntl.h>

//# include </usr/include/linux/signal.h>

typedef enum e_redir_type
{
	REDIRECT_IN,
	REDIRECT_OUT,
	APPEND,
	HEREDOC
}	t_redir_type;

typedef struct s_vdata
{
	char	*value;
	char	**expanded;
	char	*temp;
	char	*name;
}	t_vdata;

typedef struct s_expand
{
	int		sgl;
	int		dbl;
	int		i;
	char	*value;
	int		start;
	int		len;
}	t_expand;

// linked list for redirects in each command struct

typedef struct s_redir
{
	char			*file;
	char			*delimiter;
	t_redir_type	type;
	int				node_ind;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char	*segment;
	char	*command;
	char	*cmd_path;
	char	**args;
	int		args_count;
	t_redir	*redir_head;
	t_redir	*redir_tail;
	int		fd_in;
	int		fd_out;
	int		cmd_exit;
}	t_cmd;

typedef struct s_env
{
	char			*name;
	struct s_env	*next;
	char			*value;

}	t_env;

typedef struct s_shell
{
	t_cmd	**cmds;
	t_env	*env;
	int		cmd_count;
	char	**pending;
	int		*pids;
	int		**pipes;
	int		prev_pipe;
	int		stdin_saved;
	int		stdout_saved;
	int		exit_stat;
} t_shell;

void printer(t_shell *mini);

/*built_in*/
	/*cd*/
int		built_cd(t_shell *mini, t_cmd *cmd);
	/*echo.c*/
int		built_echo(t_cmd *cmd);
	/*exit.c*/
int		built_exit(t_shell *mini, t_cmd *cmd);
	/*export.c*/
int		built_export(t_shell *mini, t_cmd *cmd);
	/*pwd.c*/
int		built_pwd(t_shell *mini);
	/*unset.c*/
int		built_unset(t_shell *mini, t_cmd *cmd);

/*built_in/env*/
	/*env.c*/
char	**env_to_array(t_env *env);
int		built_env(t_shell *mini);

	/*env_handling*/
char	**copy_env(char **envp);
int		built_env(t_shell *mini);
t_env	*list_env(char **envp);
void	to_alphabetical(char **array);

	/*env_ll*/
t_env	*add_node(char *env);
void	ft_env_lstadd_back(t_env **lst, t_env *new);

/*commands*/
	/*cmd_array.c*/
int		count_pipes(char *line);
int		prepare_command_structs(t_shell *mini, char *input);
void	initialize_command_struct(t_cmd *cmd);

/*parser*/
	/*parser.c*/
int		parse_input(t_shell *mini);
int		parse_cmd_string(t_shell *mini, t_cmd *cmd);
int		handle_redirections(t_cmd *cmd, int i);
int		handle_cmd_name(t_cmd *cmd, int i);
int		parse_and_validate_input(char *input, t_shell *mini);

	/*expand.c*/
int		oh_its_a_dollar(t_shell *mini, char *str, char **expanded, t_expand *arg);
int		expand_variable(t_shell *mini, char *str, char **expanded, t_expand *arg);

	/*expand_utils.c*/
char	*get_value(t_env *env, char *name);
int		handle_value(t_shell *mini, t_vdata *data);
void	init_vdata(t_vdata *data, char **expanded, char *temp, char *name);

/*handle_cmd_array.c*/
int		handle_cmd_args(t_shell *mini, t_cmd *cmd, int i);
int		count_args(t_cmd *cmd, int i);

	/*handle_cmd_array_utils.c*/
int		skip_whitespace(char *str, int i);
int		arg_in_quotes(t_shell *mini, char *str, int i, t_expand *arg);
int		arg_no_quotes(t_shell *mini, t_cmd *cmd, int i, t_expand *arg);
int		append_to_array(t_cmd *cmd, char *start, int len, int *index);

	/*handle_cmd_array_utils.c*/
int		add_char(char *str, t_expand *arg);
int		the_arg(t_expand *arg, int i);
void	what_quote(char *str, t_expand *arg);
int		we_have_dollar(t_shell *mini, t_expand *arg, char *str);

	/*split_inputs.c*/

int		split_input_by_pipes(char *input, t_shell *mini);
char	*trim_whitespace(char *segment);

	/*find_cmd_path.c*/
int		get_cmd_path(t_shell *mini, t_cmd *cmd);

/*syntax*/

	/*syntax_checker.c*/
int		validate_input_syntax(char **input);
int		check_quotes(char *input, int limit);
int		check_non_whitespace(char *str);

	/*redirection_syntax.c*/
int		check_redirects(char *input);

	/*pipe_syntax.c*/
int		check_pipes(char **input);

/*redirection*/
	/*redir_ll*/
t_redir	*list_redir(void);
t_redir	*redir_add_node(void);
void	redir_lstadd_back(t_redir **lst, t_redir *new);
void	redir_update_tail(t_cmd *cmd);
int		redirll_head_tail(t_cmd *cmd);

	/*handle_redirections.c*/
bool	is_redirection(t_cmd *cmd, int i);
int		handle_redirect_in(t_cmd *cmd, int i);
int		handle_redirect_out(t_cmd *cmd, int i);
int		handle_heredoc(t_cmd *cmd, int i);
int		handle_append(t_cmd *cmd, int i);

	/*redirector.c*/
int		resolve_fd(t_cmd *cmd);

	/*open_files.c*/
int		open_input_file(t_cmd *cmd, char *input_file);
int		open_output_file(t_cmd *cmd, char *output_file);
int		open_append_file(t_cmd *cmd, char *output_file);
int		open_heredoc(t_cmd *cmd, char *delimiter);

/*executor*/

	/*pipeline.c*/
int		execute_pipeline(t_shell *mini);

	/*parent_process_utils.c*/
int		dup2_and_close_in_main(t_shell *mini, int old_fd, int new_fd);
void	close_fds_and_pipes(t_shell *mini, int i);
void	wait_children(t_shell *mini);

	/*child_process.c*/
int		fork_and_execute(t_shell *mini, t_cmd *cmd, int i);

	/*child_process_utils.c*/
int		dup_input(t_shell *mini, t_cmd *cmd, int i);
int		dup_output(t_shell *mini, t_cmd *cmd, int count, int i);
int		dup2_and_close(int old_fd, int new_fd);

	/*handle_builtins.c*/
int		built_in_exe(t_shell *mini, t_cmd *cmd);
int		is_this_builtin_cmd(t_cmd *cmd);
int		is_this_built(char *str);

	/*fd_handler.c*/
int		save_fds(t_shell *mini);
int		reset_fds(t_shell *mini);

/*utils*/
	/*freeing*/
void	clean_env(t_env *ll, char **array);
void	cleaner(t_shell *mini);
void	error(char *str);
void	ft_free_int_arr_with_size(int **array, int size);
void	clean_cmds(t_cmd **cmds);

	/*exit_handler.c*/
//void	exit_for_pipes(t_shell *mini, int pipe_fd[2], int i, int exit_status);
//void	exit_for_pipes(t_shell *mini, int i, int exit_status);
void	exit_handler(t_shell *mini, int i, int exit_status);
void	exit_for_success(t_shell *mini, int i, int exit_status);
void	cleaner_for_main(t_shell *mini);

/*signals.c*/
void	init_sig(void);
void	sig_handler_changer(void);
void	sig_reseted(void);

#endif
