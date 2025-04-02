/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthuilli <pthuilli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 17:17:30 by abidaux           #+#    #+#             */
/*   Updated: 2025/04/02 08:30:32 by pthuilli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/* type of token
INPUT = 1     // <
HEREDOC = 2   // <<
TRUNC = 3     // >
APPEND = 4    // >>
PIPE = 5      // |
CMD = 6       // cmd (ex: "cat")
ARG = 7       // arg or file after redirection
*/

#include "libft/libft.h"
#include "readline/history.h"
#include "readline/readline.h"
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>

#define PATH_MAX 4096

typedef struct s_heredoc
{
	char	*tmp_filename;
	char	*limiter;
	int		tmp_fd;
	char	**out_tmp_filename;
}	t_heredoc;

typedef struct s_Shell_State
{
	char		current_path[1024];
	char		**envp;
	int			last_exit_status;
	int			num_pipes;
	int			index;
	int			o_stdout;
	int			o_stdin;
	int			*pipes;
	pid_t		*pids;
	t_heredoc	*hd;
}	t_state;

typedef enum e_redir_type
{
	R_INPUT,
	R_OUTPUT,
	R_APPEND,
	R_HEREDOC
}	t_redir_type;

typedef struct s_redir
{
	t_redir_type	type;
	char			*target;
}	t_redir;

typedef struct s_command
{
	char				**args;

	t_redir				**in;
	int					nb_in;
	t_redir				**out;
	int					nb_out;
	char				*tmp_filename;
	int					tmp_fd;
	char				*path;

	int					is_pipe;
	struct s_command	*next;
	struct s_command	*prev;
}	t_command;

typedef struct s_context
{
	t_command	*cmd;
	t_state		*state;
	int			exit;
}	t_context;

// Structure pour un token en liste chaînée
typedef struct s_token
{
	char			*content;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

typedef struct s_fork_info
{
	int		(*pipes)[2];
	pid_t	*pids;
	int		num_cmds;
}	t_fork_info;

/*
** Exemple de structure pour stocker infos de heredoc :
** tmp_filename : "/tmp/heredoc_1234"
** tmp_fd       : FD si besoin
** limiter      : la chaîne "EOF" par exemple
*/

/*
** Exit Status Codes
** 0   - Success
** 1   - General errors
** 2   - Syntax errors
** 126 - Command found but not executable
** 127 - Command not found
** 130 - Script terminated by Control-C
*/

/* ---------------- origin folder ----------------  */
/* ----------------    exec     ----------------  */
	/* -------- external_utils.c ---- */
t_context	*get_signal(void);
void		child_cleanup_and_exit(int signal);
void		handle_no_cmd(t_command *cmd, t_state *state);

	/* -------- external_utils.c ---- */
int			handle_all_redirections(t_command *cmd, t_state *state);

	/* -------- external_utils2.c ---- */
void		cleanup_command(t_command *cmd);
void		cleanup_heredoc(t_heredoc *hd);
void		cleanup_state(t_state *st);
	/* -------- state.c ------------- */
int			exec_builtins(t_command *cmd, t_state *state);
void		handle_source_command(char **args, t_state *state);
void		reset_state(t_state *state);
int			check_synthax(t_command *start_cmd, t_state *state);

	/* -------- exec_shell.c -------- */
void		exec_shell(t_command *cmd, t_state *state);

	/* -------- exec_shell2.c -------- */
int			check_if_empty_command(t_command *cmd, t_state *st);
int			exec_if_builtin(t_command *cmd, t_state *st);

	/* -------- exec_utils.c -------- */
void		setup_pipes(int pipefds[][2], t_state *state);
void		close_pipes(int	pipefds[][2], int numpipes);
void		print_error_and_exit(char *cmd, char *path, char *msg, int exit_code);
int			print_error(char *cmd, char *path, char *msg, int exit_code);
char		*valide_path(char *cmd_path);

	/* -------- exec_utils3.c -------- */
void		execute_external(t_command *cmd, t_state *state);
int			is_builtin(char *cmd);
int			check_output_target(t_redir *redir, struct stat *st, t_state *state);

	/* -------- exec_utils4.c -------- */
int			handle_output_redirection(t_redir *redir, t_state *state);
void		handle_command_error(t_state *state, char *cmd, int error_code);
int			count_commands(t_command *start_cmd);
	/* -------- external_command.c -------- */
void		handle_external_cmd(t_command *cmd, char **argv, t_state *state);

	/* -------- external_command2.c -------- */
void		close_child_fds(t_state *st);
void		init_child_context(t_command *cmd, t_state *st);
void		process_heredocs_or_fail(t_command *cmd, t_state *st, char *path);
void		process_redirections_or_fail(t_command *cmd, t_state *st);
void		execute_binary_or_fail(t_command *cmd, t_state *st, char *path);
	/* -------- forking.c -------- */
int			handle_single_command(t_command **cmd,
				t_state *st, t_fork_info *fi, int i);
void		child_process(t_command *cmd, t_state *st,
				t_fork_info *fi, int i);
void		handle_empty_cmd(t_command *cmd, t_state *st);
	/* -------- multiples_cmd.c ----------- */
void		handle_multiple_pipes(t_command *start_cmd, t_state *state);
void		execute_command(t_command *cmd, t_state *state);
void		execute_builtin(t_command *cmd, t_state *temp_state);

	/* -------- multiples_cmd2.c ----------- */
void		handle_redirections_or_exit(t_command *cmd, t_state *st);
void		copy_environment_or_exit(t_state *st,
				t_state *temp_st, char ***temp_env);
void		handle_no_args_or_exit(t_command *cmd,
				t_state *st, char **temp_env);
	/* -------- multiples_utils.c ----------- */
int			allocate_pipes_pids(int num_cmds, int **pipes, pid_t **pids, t_state *state);
int			create_pipes(int num_pipes, int pipes[][2]);
int			fork_and_execute(t_command *cmd, t_state *state,
				int pipes[][2], pid_t *pids);
void		close_all_pipes(int num_pipes, int pipes[][2]);
void		wait_children(int num_cmds, pid_t *pids, t_state *state);
	/* -------- path.c -------------------- */
char		*get_command_path(char *cmd, t_state *state);
int			handle_access_error(char *target, t_state *state);
int			is_path_absolute_or_relative(char *cmd);
int			validate_command_path(char *cmd, char **path, t_state *st, bool *check);
	/* -------- path.c -------------------- */
int			allocate_and_create_pipes(t_command *start_cmd,
				int **pipes, pid_t **pids, t_state *state);
void		execute_and_cleanup(t_command *start_cmd,
				t_state *state, int (*pipes)[2], pid_t *pids);
	/* -------- path2.c -------------------- */
int			check_file_existence(char *cmd, t_state *st, bool *check);
int			check_dir_and_exec(char *cmd, t_state *st, bool *check);
	/* -------- builtins --------- */
		/* ----- cd_utils.c ------ */
int			check_cd_path_permission(char *path);
int			check_cd_path(char *path, t_state *state);
void		handle_cd_command(char **argv, t_state *state);
		/* ----- cd.c ------ */
char		*get_cd_path(char **argv, t_state *state);
void		update_env_vars(t_state *state);
int			check_cd_args(char **argv);
int			check_cd_path_exists(char *path);
int			check_cd_path_dir(char *path);
		/* ----- echo.c ------ */
void		handle_echo_command(t_command *cmd, t_state *state);

		/* ----- env.c ------ */
int			has_equals_as(char *str, char *ui);
void		handle_env_command(char **envp, t_state *state);
char		*join_key_value(const char *key, const char *value);
void		replace_env_entry(char **envp, int index, const char *key,
				const char *value);
		/* ----- exit.c ------ */
void		handle_exit(t_command *cmd, t_state *st);
		/* ----- export_utils.c ------ */
void		print_sorted_env(char **envp);
void		handle_export_command(char **argv, t_state *state);
		/* ----- export_utils2.c ------ */
char		*normalize_spaces(const char *str);
		/* ----- export.c ------ */
int			is_valid_identifier(const char *str);
char		**get_sorted_env(char **envp);
int			has_leading_space(const char *str);
		/* ----- pwd.c ------ */
void		handle_pwd_command(t_state *state);
		/* ----- unset.c ------ */
void		handle_unset_command(char **argv, char **envp);

/* ----------------    parsing    ----------------  */
	/* -------- command_free.c -------- */
// void		free_args(char **args);
// void		free_command_list(t_command *cmd);
	/* -------- command_utils.c -------- */
t_command	*init_command(void);
int			fill_command_args(t_command *cmd, t_token *current, int *index);
int			handle_pipe(t_command **current_cmd, int *arg_index, t_state *state);
int			handle_arguments(t_command *current_cmd, t_token *current, int *arg_index);
	/* -------- ft_arraydup.c -------- il faut quoi ici lui mdr */
char		**ft_arraydup(char **array);
	/* -------- heredoc_utils.c -------- */
int			fork_one_heredoc(const char *lim, char **out_tmp,
			t_command *cmd, t_state *st);
int			handle_all_heredocs(t_command *cmd, t_state *state);
void		free_child_and_exit(t_heredoc *hd, t_command *cmd, t_state *st);
void		child_read_heredoc(t_heredoc *hd, t_command *cmd, t_state *st);
	/* -------- heredoc_utils2.c -------- */
int			fork_parent(pid_t p, t_heredoc *hd, t_state *st,
				struct sigaction *o);
int			fork_child(t_heredoc *hd, t_command *cmd,
				t_state *st, struct sigaction *o);
	/* -------- heredoc.c -------- */
char		*generate_tmp_file(void);
int			write_to_tmp_file(int fd, const char *limiter);
void		init_signal_context(t_heredoc *hd, t_command *cmd, t_state *st);
int			open_heredoc_file(t_heredoc *hd);
void		read_and_write_heredoc(t_heredoc *hd);
	/* -------- parse_input.c -------- */
t_command	*initialize_command(t_state *state);
int			handle_commands(t_command **cmd, t_token *tokens,
				t_state *state);
void		cleanup_on_error(t_token *tokens, t_command *cmd,
				int error_code, t_state *state);
t_command	*parse_input(char *input, t_state *state);

	/* -------- parse_input2.c -------- */
int			convert_tokens_to_commands(t_command **cmd, t_token *tokens, t_state *state);
int			validate_input(char *input);
t_token		*tokenize_and_validate(char *input, t_state *state);

	/* -------- parsing.c -------- */
char		*handle_special_var(char c, t_state *state);
char		*expand_variable(char *input, int *i, t_state *state);
void		debug_tokens(t_token *tokens);
void		append_tokens(t_token **current, t_token *new_tokens);

	/* -------- quotes.c ------ */
int			check_unclosed_quotes(const char *input);
char		*handle_double_quotes(const char *input, int *i, t_state *state);

	/* -------- redir.c -------- */
int			add_in_redir(t_command *cmd, t_redir_type type, const char *content);
int			add_out_redir(t_command *cmd, t_redir_type type, const char *content);

/* ----------------    utils     ----------------  */
	/* -------- parse_utils.c -------- */
void		unlink_all_heredocs(t_command *cmd);
void		restore_and_close_fds(t_state *state);

	/* -------- parse_utils2.c -------- */
int			convert_tokens_to_command(t_command *cmd,
				t_token *tokens, t_state *state);
int			is_redirection(const char *content);
int			is_file_tok(t_token *file_tok, t_state *state);
int			check_redirections_conflicts(t_token *current, t_state *state);
int			check_syntax_errors(t_token *tokens, t_state *state);

	/* -------- parse_utils3.c -------- */
int			is_command_incomplete(t_token *tokens, t_state *state);
int			process_tokenizer(t_command **curr,
				t_token **cur, int *arg_idx, t_state *state);

	/* -------- quote_utils.c -------- */
int			should_parse_as_special(const char *input, int i);
int			handle_token(t_token **head, const char *input,
				int *i, t_state *state);
char		*handle_quotes(const char *input, int *i, char **envp, t_state *state);
char		*handle_variable(const char *input, int *i, t_state *state);
	/* -------- quote_utils2.c -------- */
int			process_current_char(const char *input, int *i,
			char **word, t_state *state);
char		*parse_single_quote(const char *input, int *i);
char		*parse_double_quote(const char *input, int *i,
				char **envp, t_state *state);
	/* -------- syntax_utils.c -------- */
void		print_err_msg(char *cmd, char *path, char *msg);
int			check_initial_pipe(t_token *current, t_state *state);
int			check_pipes(t_token *current, t_state *state);
int			check_redirections(t_token *current, t_state *state);
int			check_invalid_tokens(t_token *current, t_state *state);



	/* -------- token_utils_create.c -------- */
t_token		*create_and_add_token(char *content, t_token **head);

	/* -------- token_utils.c -------- */
t_token		*tokenize_input(const char *input, t_state *state);

	/* -------- utils_env.c -------- */
int			is_empty_or_space(const char *str);
char		**set_env_var(char **envp, const char *key, const char *value);
char		**copy_environment(char **envp);
char		**realloc_tab(char **envp, char *new_entry);
char		*search_in_local_env(char **envp, const char *key);

	/* -------- utils_free.c -------- */
void		free_command_list(t_command *cmd);
void		free_args(char **args);
void		free_tokens(t_token *head);
void		ft_free_array(char **split);

	/* -------- utils.c -------- */
void		free_command(t_command *cmd);
int			skip_spaces(const char *input, int i);
int			is_empty_or_space(const char *str);

	/* -------- main.c --------- */
void		free_envp(char **envp);
int			handle_user_input(char *input, t_state *state);

	/* -------- signals.c --------- */
void		reset_signals_after_execution(void);
void		setup_signals_for_heredoc(void);
void		setup_signals_for_execution(void);
void		handle_sigint(int signo);
void		setup_signals(void);


//strtol
long	handle_conversion(const char **str,
	int base, int sign, char **endptr);
int	determine_base(const char **str, int base);
int	ft_convert_digit(char c);
long	ft_handle_overflow(long result, int digit, int base, int sign);
int	validate_base(int base, const char **str, char **endptr);
void	skip_whitespace_and_sign(const char **str, int *sign);
long	ft_strtol(const char *str, char **endptr, int base);
// ft swap string
void	ft_swap_strings(char **s1, char **s2);