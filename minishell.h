/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abidaux <abidaux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 17:17:30 by abidaux           #+#    #+#             */
/*   Updated: 2025/03/17 21:26:51 by abidaux          ###   ########.fr       */
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

typedef struct s_token
{
	char	**str;
	int		type;
	struct s_token	*next;
}	t_token;

#include "libft/libft.h"
#include "readline/history.h"
#include "readline/readline.h"
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>

void	parsing(char *input);
