/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthuilli <pthuilli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:29:31 by pthuilli          #+#    #+#             */
/*   Updated: 2025/04/01 13:29:39 by pthuilli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	allocate_and_create_pipes(t_command *start_cmd,
	int **pipes, pid_t **pids, t_state *state)
{
	int	num_cmds;
	int	num_pipes;

	num_cmds = count_commands(start_cmd);
	num_pipes = num_cmds - 1;
	if (allocate_pipes_pids(num_cmds, pipes, pids, state) == -1)
		return (-1);
	if (create_pipes(num_pipes, (int (*)[2]) * pipes) == -1)
	{
		free(*pipes);
		free(*pids);
		return (-1);
	}
	return (num_pipes);
}

void	execute_and_cleanup(t_command *start_cmd,
	t_state *state, int (*pipes)[2], pid_t *pids)
{
	int	num_cmds;
	int	num_pipes;

	num_cmds = count_commands(start_cmd);
	num_pipes = num_cmds - 1;
	if (fork_and_execute(start_cmd, state, pipes, pids) == -1)
	{
		close_all_pipes(num_pipes, pipes);
		free(pipes);
		free(pids);
		return ;
	}
	close_all_pipes(num_pipes, pipes);
	wait_children(num_cmds, pids, state);
	free(pipes);
	free(pids);
}
