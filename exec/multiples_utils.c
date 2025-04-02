/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiples_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthuilli <pthuilli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 11:01:12 by pthuilli          #+#    #+#             */
/*   Updated: 2025/04/01 13:34:27 by pthuilli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Alloue les structures pour les pipes et les PID
int	allocate_pipes_pids(int num_cmds, int **pipes, pid_t **pids, t_state *state)
{
	*pids = malloc(sizeof(pid_t) * num_cmds);
	*pipes = malloc(sizeof(int [2]) * (num_cmds - 1));
	if (!(*pipes) || !(*pids))
	{
		ft_putstr_fd("minishell: memory allocation error\n", 2);
		free(*pipes);
		free(*pids);
		state->last_exit_status = 1;
		return (-1);
	}
	return (0);
}

// Crée les pipes nécessaires
int	create_pipes(int num_pipes, int pipes[][2])
{
	int	i;

	i = 0;
	while (i < num_pipes)
	{
		if (pipe(pipes[i]) == -1)
		{
			close_all_pipes(i, pipes);
			perror("pipe");
			return (-1);
		}
		i++;
	}
	return (0);
}

int	fork_and_execute(t_command *cmd, t_state *state,
	int pipes[][2], pid_t *pids)
{
	int			i;
	t_fork_info	fi;
	t_context	*ctx;

	ctx = get_signal();
	ctx->cmd = cmd;
	ctx->state = state;
	fi.pipes = pipes;
	fi.pids = pids;
	fi.num_cmds = count_commands(cmd);
	i = -1;
	reset_signals_after_execution();
	while (++i < fi.num_cmds)
		if (handle_single_command(&cmd, state, &fi, i) < 0)
			return (-1);
	return (0);
}

// Ferme tous les pipes
void	close_all_pipes(int num_pipes, int pipes[][2])
{
	int	i;

	i = 0;
	while (i < num_pipes)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

// Attend les processus enfants et gère le statut de sortie
void	wait_children(int num_cmds, pid_t *pids, t_state *state)
{
	int		i;
	int		status;
	int		last_status;
	char	*exit_str;

	i = 0;
	last_status = 0;
	status = 0;
	while (i < num_cmds)
	{
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status))
			last_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			last_status = 128 + WTERMSIG(status);
		i++;
	}
	reset_signals_after_execution();
	state->last_exit_status = last_status;
	exit_str = ft_itoa(last_status);
	if (exit_str)
	{
		state->envp = set_env_var(state->envp, "EXIT_STATUS", exit_str);
		free(exit_str);
	}
}
