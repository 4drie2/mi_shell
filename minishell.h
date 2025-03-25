/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abidaux <abidaux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 17:17:30 by abidaux           #+#    #+#             */
/*   Updated: 2025/03/25 16:26:34 by abidaux          ###   ########.fr       */
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
#include <signal.h>

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
void	child_cleanup_and_exit(int signal);
void	handle_no_cmd(t_command *cmd, t_state *state);
	/* -------- exec_shell.c -------- */
void		exec_shell(t_command *cmd, t_state *state);

	/* -------- exec_shell2.c -------- */
int			check_if_empty_command(t_command *cmd, t_state *st);
int			exec_if_builtin(t_command *cmd, t_state *st);

	/* -------- exec_utils3.c -------- */
void		execute_external(t_command *cmd, t_state *state);
int			is_builtin(char *cmd);
int			check_output_target(t_redir *redir, struct stat *st, t_state *state);

	/* -------- builtins --------- */
		/* ----- env.c ------ */
char		*join_key_value(const char *key, const char *value);

/* ----------------    parsing    ----------------  */
	/* -------- heredoc_utils.c -------- */
int	fork_one_heredoc(const char *lim, char **out_tmp,
		t_command *cmd, t_state *st);
int	handle_all_heredocs(t_command *cmd, t_state *state);
void	free_child_and_exit(t_heredoc *hd, t_command *cmd, t_state *st);
void	child_read_heredoc(t_heredoc *hd, t_command *cmd, t_state *st);

	/* -------- parse_input.c -------- */
t_command	*initialize_command(t_state *state);
int			handle_commands(t_command **cmd, t_token *tokens,
				t_state *state);
void		cleanup_on_error(t_token *tokens, t_command *cmd,
				int error_code, t_state *state);
t_command	*parse_input(char *input, t_state *state);

	/* -------- parse_input2.c -------- */
int			validate_input(char *input);

	/* -------- parsing.c -------- */
char		*handle_special_var(char c, t_state *state);
char		*expand_variable(char *input, int *i, t_state *state);
void		debug_tokens(t_token *tokens);
void		append_tokens(t_token **current, t_token *new_tokens);
	/* -------- redir.c -------- */
int			add_in_redir(t_command *cmd, t_redir_type type, const char *content);
int			add_out_redir(t_command *cmd, t_redir_type type, const char *content);

	/* -------- quotes.c ------ */
int			check_unclosed_quotes(const char *input);
char		*handle_double_quotes(const char *input, int *i, t_state *state);

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

	/* -------- syntax_utils2.c -------- */
int			check_redirections_conflicts(t_token *current, t_state *state);
int			check_syntax_errors(t_token *tokens, t_state *state);

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

	/* -------- utils.c -------- */
void		free_command(t_command *cmd);
int			skip_spaces(const char *input, int i);
int			is_empty_or_space(const char *str);

	/* -------- main.c --------- */
void		free_envp(char **envp);
int			handle_user_input(char *input, t_state *state);
