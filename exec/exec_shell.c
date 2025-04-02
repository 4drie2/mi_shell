/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthuilli <pthuilli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 16:44:34 by abidaux           #+#    #+#             */
/*   Updated: 2025/04/02 08:50:02 by pthuilli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	save_original_fds(t_state *state)
{
	state->o_stdin = dup(STDIN_FILENO);
	if (state->o_stdin == -1)
	{
		perror("dup");
		return (-1);
	}
	state->o_stdout = dup(STDOUT_FILENO);
	if (state->o_stdout == -1)
	{
		perror("dup");
		close(state->o_stdin);
		return (-1);
	}
	return (0);
}

/*
Exécute une commande sans pipeline, avec gestion des redirections et des built.
*/
static void	exec_single_command(t_command *cmd, t_state *st)
{
	if (save_original_fds(st) == -1)
		return ;
	if (exec_if_builtin(cmd, st))
		return ;
	if (check_if_empty_command(cmd, st))
		return ;
	handle_external_cmd(cmd, cmd->args, st);
	restore_and_close_fds(st);
}

/*
** Fonction principale pour l'exécution des commandes.
*/
void	exec_shell(t_command *cmd, t_state *state)
{
	if ((!cmd->args || !cmd->args[0]) && !cmd->is_pipe)
	{
		handle_no_cmd(cmd, state);
	}
	else if (!cmd->is_pipe)
		exec_single_command(cmd, state);
	else
		handle_multiple_pipes(cmd, state);
}
