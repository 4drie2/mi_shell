/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthuilli <pthuilli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 10:38:56 by pthuilli          #+#    #+#             */
/*   Updated: 2025/04/01 10:42:08 by pthuilli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	cleanup_command(t_command *cmd)
{
	if (cmd->tmp_fd)
		close(cmd->tmp_fd);
	if (cmd->tmp_filename)
		free(cmd->tmp_filename);
	if (cmd->path)
		free(cmd->path);
	while (cmd->prev)
		cmd = cmd->prev;
	free_command_list(cmd);
}

void	cleanup_heredoc(t_heredoc *hd)
{
	if (!hd)
		return ;
	if (hd->tmp_fd)
		close(hd->tmp_fd);
	if (hd->tmp_filename)
		free(hd->tmp_filename);
}

void	cleanup_state(t_state *st)
{
	if (!st)
		return ;
	if (st->envp)
		free_envp(st->envp);
	if (st->pids)
	{
		free(st->pids);
		st->pids = NULL;
	}
	if (st->pipes)
	{
		close_pipes(st->num_pipes, (int (*)[2])st->pipes);
		free(st->pipes);
		st->pipes = NULL;
	}
}
