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
	t_redir *redir_head;
	t_redir *redir_tail;
	int		fd_in;
	int		fd_out;
	int		exit_status;
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
	int		prev_pipe[2];
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
int	built_unset(t_shell *mini, t_cmd *cmd);

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
t_cmd	**allocate_cmd_array(int command_count);
void	initialize_command_struct(t_cmd *cmd);

/*parser*/
	/*parser.c*/
int		parse_input(t_shell *mini);
int		parse_cmd_string(t_cmd *cmd);
int		handle_redirections(t_cmd *cmd, int i);
int		handle_cmd_name(t_cmd *cmd, int i);
int		parse_and_validate_input(char *input, t_shell *mini);

	/*expand.c*/
char	*expand_var(t_shell *mini, char *str);
int		expand_or_not(t_shell *mini, t_cmd *cmd);

/*handle_cmd_array.c*/
int		handle_cmd_args(t_cmd *cmd, int i);
int		count_args(t_cmd *cmd, int i);

	/*handle_cmd_array_utils.c*/
int		skip_whitespace(char *str, int i);
int		arg_in_quotes(char *str, int i, char **start, int *len);
int		arg_no_quotes(t_cmd *cmd, int i, char **start, int *len);
int		append_to_array(t_cmd *cmd, char *start, int len, int *index);

	/*split_inputs.c*/

int		split_input_by_pipes(char *input, t_shell *mini);
char	*trim_whitespace(char *segment);


	/*syntax_checks.c*/
int		validate_input_syntax(char *input);
int 	check_quotes(char *input, int limit);
int		check_consecutive_pipes(char *input);
int		check_pipes(char *input);
int		check_redirects(char *input);
int		validate_redirect(char *input, int *i, char *type);

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
int		open_input_file(char *input_file);
int		open_output_file(char *output_file);
int		open_append_file(char *output_file);
int		open_heredoc(char *delimiter);

/*executor*/
	/*find_cmd_path.c*/
int		get_cmd_path(t_shell *mini, t_cmd *cmd);

	/*pipeline.c*/
int		execute_pipeline(t_shell *mini);

	/*pipeline_utils.c*/
int		dup_input(t_shell *mini, t_cmd *cmd, int i);
int		dup_output(t_cmd *cmd, int pipe_fd[2], int count, int i);
int		dup2_and_close(int old_fd, int new_fd);
void	close_pipe_fds(int pipe_fd[2]);
void	close_pipes(t_shell *mini, int pipe_fd[2]);

	/*handle_builtins.c*/
int		built_in_exe(t_shell *mini, t_cmd *cmd);
int		is_this_builtin_cmd(t_cmd *cmd);
int		is_this_built(char *str);

	/*fd_handler.c*/
int		save_fds(t_shell *mini);
int		reset_fds(t_shell *mini);

/*utils/freeing*/
void	clean_env(t_env *ll, char **array);
void	cleaner(t_shell *mini);
void	error(char *str);
void	clean_cmds(t_cmd **cmds);

/*signals.c*/
void	init_sig(void);
void	sig_handler_changer(void);
void	sig_reseted(void);

#endif
