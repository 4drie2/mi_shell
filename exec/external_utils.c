/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthuilli <pthuilli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 16:44:56 by abidaux           #+#    #+#             */
/*   Updated: 2025/04/01 10:38:26 by pthuilli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_context	*get_signal(void)
{
	static t_context	context;

	return (&context);
}

/**
 * Gestionnaire de signal pour le processus enfant qui nettoie les ressources
 * et termine le processus avec un code d'erreur approprié.
 *
 * @param signal Numéro du signal reçu
 */
void	child_cleanup_and_exit(int signal)
{
	t_context	*ctx;

	ctx = get_signal();
	cleanup_command(ctx->cmd);
	cleanup_heredoc(ctx->state->hd);
	cleanup_state(ctx->state);
	exit(ctx->state->last_exit_status = (127 + signal));
}

static void	child_no_cmd(t_command *cmd, t_state *st)
{
	t_context	*context;

	context = get_signal();
	context->cmd = cmd;
	context->state = st;
	if (handle_all_heredocs(cmd, st) < 0)
	{
		free_envp(st->envp);
		free_command_list(cmd);
		exit(EXIT_FAILURE);
	}
	signal(SIGINT, child_cleanup_and_exit);
	if (handle_all_redirections(cmd, st) < 0)
	{
		free_envp(st->envp);
		free_command_list(cmd);
		exit(st->last_exit_status);
	}
	free_envp(st->envp);
	free_command_list(cmd);
	exit(EXIT_FAILURE);
}

/**
 * Gère le cas d'une commande vide en créant un processus enfant
 * pour configurer les redirections sans exécuter de commande.
 *
 * @param cmd   Structure de commande contenant les redirections
 * @param state État global du shell
 */
void	handle_no_cmd(t_command *cmd, t_state *state)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
		child_no_cmd(cmd, state);
	else
	{
		waitpid(pid, NULL, 0);
		reset_signals_after_execution();
	}
}
