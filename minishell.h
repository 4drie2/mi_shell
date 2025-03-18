/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abidaux <abidaux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 17:17:30 by abidaux           #+#    #+#             */
/*   Updated: 2025/03/18 20:25:08 by abidaux          ###   ########.fr       */
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

	/* ----- utils.c ----- */
int			is_empty_or_space(const char *str);

	/* ----- main.c ------ */
void		free_envp(char **envp);
int			handle_user_input(char *input, t_state *state);
