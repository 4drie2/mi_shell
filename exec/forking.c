/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forking.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthuilli <pthuilli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:35:51 by pthuilli          #+#    #+#             */
/*   Updated: 2025/04/02 07:34:19 by pthuilli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * Gère l'exécution d'une commande dans un processus enfant.
 *
 * @param cmd Double pointeur vers la commande à exécuter (pour mise à jour)
 * @param st Structure d'état du shell
 * @param fi Informations sur les processus fork et les descripteurs
 * @param i Indice du processus courant
 * @return 0 en cas de succès, -1 en cas d'erreur
 */
int	handle_single_command(t_command **cmd,
	t_state *st, t_fork_info *fi, int i)
{
	if (handle_all_heredocs(*cmd, st) < 0)
		return (-1);
	signal(SIGINT, child_cleanup_and_exit);
	fi->pids[i] = fork();
	if (fi->pids[i] == -1)
		return (perror("fork"), -1);
	if (fi->pids[i] == 0)
		child_process(*cmd, st, fi, i);
	reset_signals_after_execution();
	*cmd = (*cmd)->next;
	return (0);
}

/**
 * Gère l'exécution de la commande dans le processus enfant.
 * Configure les redirections, ferme les pipes inutiles et exécute la commande.
 *
 * @param cmd Structure de la commande à exécuter
 * @param st État global du shell
 * @param fi Informations sur les forks et pipes
 * @param i Indice de la commande courante
 */
void	child_process(t_command *cmd, t_state *st,
	t_fork_info *fi, int i)
{
	if (i > 0)
		dup2(fi->pipes[i - 1][0], STDIN_FILENO);
	if (i < fi->num_cmds - 1)
		dup2(fi->pipes[i][1], STDOUT_FILENO);
	close_all_pipes(fi->num_cmds - 1, fi->pipes);
	free(fi->pipes);
	free(fi->pids);
	if (cmd->args[0])
	{
		execute_command(cmd, st);
		exit(st->last_exit_status);
	}
	else if (!cmd->args[0] || !cmd->args[0][0])
		handle_empty_cmd(cmd, st);
	exit(127);
}

/**
 * Gère le cas d'une commande vide (sans arguments).
 * Effectue les redirections nécessaires et nettoie les ressources avant de quitter.
 *
 * @param cmd Structure de la commande vide
 * @param st État global du shell
 */
void	handle_empty_cmd(t_command *cmd, t_state *st)
{
	if (handle_all_redirections(cmd, st) < 0)
	{
		while (cmd->prev)
			cmd = cmd->prev;
		free_command_list(cmd);
		free_envp(st->envp);
		exit(st->last_exit_status = 1);
	}
	while (cmd->prev)
		cmd = cmd->prev;
	free_command_list(cmd);
	free_envp(st->envp);
	exit(st->last_exit_status);
}