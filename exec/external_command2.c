/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_command2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthuilli <pthuilli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 09:41:40 by pthuilli          #+#    #+#             */
/*   Updated: 2025/04/03 12:39:04 by pthuilli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	close_child_fds(t_state *st)
{
	close(st->o_stdin);
	close(st->o_stdout);
}

void	init_child_context(t_command *cmd, t_state *st)
{
	t_context	*ctx;

	ctx = get_signal();
	ctx->cmd = cmd;
	ctx->state = st;
}

void	process_heredocs_or_fail(t_command *cmd, t_state *st, char *path)
{
	if (handle_all_heredocs(cmd, st) < 0)
	{
		free(path);
		free_envp(st->envp);
		while (cmd->prev)
			cmd = cmd->prev;
		free_command_list(cmd);
		exit(EXIT_FAILURE);
	}
}

void	process_redirections_or_fail(t_command *cmd, t_state *st)
{
	if (handle_all_redirections(cmd, st) < 0)
	{
		free(cmd->path);
		free_envp(st->envp);
		while (cmd->prev)
			cmd = cmd->prev;
		free_command_list(cmd);
		exit(st->last_exit_status);
	}
}

void	execute_binary_or_fail(t_command *cmd, t_state *st, char *path)
{
	struct stat	sb;
	
	if (stat(path, &sb) == 0 && S_ISDIR(sb.st_mode))
	{
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": Is a directory\n", 2);
		free(path);
		while (cmd->prev)
			cmd = cmd->prev;
		free_command_list(cmd);
		free_envp(st->envp);
		exit(126);
	}
	if (execve(path, cmd->args, st->envp) == -1)
	{
		free(path);
		while (cmd->prev)
			cmd = cmd->prev;
		free_command_list(cmd);
		free_envp(st->envp);
		exit(EXIT_FAILURE);
	}
}
