/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthuilli <pthuilli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 16:45:56 by abidaux           #+#    #+#             */
/*   Updated: 2025/04/02 07:52:40 by pthuilli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	unlink_all_heredocs(t_command *cmd)
{
	int	i;

	while (cmd)
	{
		i = 0;
		while (i < cmd->nb_in)
		{
			if (cmd->in[i]->type == R_HEREDOC && cmd->in[i]->target)
			{
				unlink(cmd->in[i]->target);
				free(cmd->in[i]->target);
				cmd->in[i]->target = NULL;
			}
			i++;
		}
		cmd = cmd->next;
	}
}

void	restore_and_close_fds(t_state *state)
{
	if (state->o_stdin && dup2(state->o_stdin, STDIN_FILENO) == -1)
		perror("dup2");
	if (state->o_stdout && dup2(state->o_stdout, STDOUT_FILENO) == -1)
		perror("dup2");
	if (state->o_stdin)
		close(state->o_stdin);
	if (state->o_stdout)
		close(state->o_stdout);
}
