/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthuilli <pthuilli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 17:17:30 by abidaux           #+#    #+#             */
/*   Updated: 2025/03/18 17:35:06 by pthuilli         ###   ########.fr       */
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

typedef struct s_token
{
	char	**str;
	int		type;
	struct s_token	*next;
}	t_token;

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

