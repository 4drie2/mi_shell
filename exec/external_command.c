/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abidaux <abidaux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 22:06:05 by abidaux           #+#    #+#             */
/*   Updated: 2025/04/02 18:08:44 by abidaux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * Gère l'erreur de fork en mettant à jour le statut de sortie
 * et l'environnement.
 *
 * Cette fonction est appelée lorsqu'un appel à fork() échoue. Elle:
 * 1. Affiche un message d'erreur via perror
 * 2. Libère le chemin du programme alloué
 * 3. Définit le statut de sortie à 1
 * 4. Met à jour la variable d'environnement EXIT_STATUS
 *
 * @param state Pointeur vers la structure d'état du shell
 * @param path Chemin du programme à libérer
 */
static void	handle_fork_error(t_state *state, char *path)
{
	char	*exit_str;

	perror("fork error");
	free(path);
	state->last_exit_status = 1;
	exit_str = ft_itoa(state->last_exit_status);
	if (!exit_str)
		return ;
	state->envp = set_env_var(state->envp, "EXIT_STATUS", exit_str);
	free(exit_str);
}

/**
 * Exécute le processus enfant après un fork.
 *
 * La fonction prépare et exécute un programme externe dans le processus enfant:
 * 1. Ferme les descripteurs de fichier non nécessaires
 * 2. Configure le contexte d'exécution (stdin/stdout pour les pipes)
 * 3. Traite les heredocs
 * 4. Configure les gestionnaires de signaux
 * 5. Applique les redirections
 * 6. Exécute le programme
 *
 * @param cmd Commande à exécuter
 * @param st État global du shell
 * @param path Chemin du binaire à exécuter
 */
static void	execute_child(t_command *cmd, t_state *st, char *path)
{
	close_child_fds(st);
	init_child_context(cmd, st);
	process_heredocs_or_fail(cmd, st, path);
	signal(SIGINT, child_cleanup_and_exit);
	process_redirections_or_fail(cmd, st);
	setup_signals_for_execution();
	execute_binary_or_fail(cmd, st, path);
}

/**
 * Gère le processus parent après un fork pour exécuter une commande.
 *
 * Cette fonction:
 * 1. Attend la terminaison du processus enfant
 * 2. Réinitialise les gestionnaires de signaux
 * 3. Récupère et enregistre le code de sortie
 * 4. Met à jour la variable d'environnement EXIT_STATUS
 *
 * @param pid PID du processus enfant à attendre
 * @param state État global du shell
 */
static void	execute_parent(pid_t pid, t_state *state)
{
	char	*exit_str;
	int		status;

	waitpid(pid, &status, 0);
	reset_signals_after_execution();
	if (WIFEXITED(status))
		state->last_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		state->last_exit_status = 128 + WTERMSIG(status);
	exit_str = ft_itoa(state->last_exit_status);
	if (!exit_str)
		return ;
	state->envp = set_env_var(state->envp, "EXIT_STATUS", exit_str);
	free(exit_str);
}

/**
 * Exécute une commande en utilisant fork() pour créer un processus enfant.
 *
 * Cette fonction :
 * 1. Crée un nouveau processus avec fork()
 * 2. Dans le processus enfant, exécute la commande
 * 3. Dans le processus parent, attend la fin du processus enfant
 * 4. Libère le chemin de la commande après exécution
 *
 * @param cmd Structure contenant les informations de la commande
 * @param state État global du shell
 */
static void	execute_fork(t_command *cmd, t_state *state)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (handle_fork_error(state, cmd->path));
	if (pid == 0)
		execute_child(cmd, state, cmd->path);
	else
		execute_parent(pid, state);
	free(cmd->path);
}

/**
 * Gère l'exécution d'une commande externe (non builtin).
 *
 * Cette fonction :
 * 1. Détermine le chemin complet de l'exécutable
 * 2. Vérifie que la commande est accessible et exécutable
 * 3. Lance l'exécution via fork si la commande est valide
 *
 * @param cmd Structure contenant les informations de la commande
 * @param argv Tableau d'arguments de la commande (argv[0] = nom de la commande)
 * @param state État global du shell
 */
void	handle_external_cmd(t_command *cmd, char **argv, t_state *state)
{
	bool	check;

	check = false;
	if (argv[0] && is_path_absolute_or_relative(argv[0]))
	{
		if (!validate_command_path(argv[0], &cmd->path, state, &check))
		{
			if (check)
				return ;
			return (handle_command_error(state, argv[0], 127));
		}
	}
	else
		cmd->path = get_command_path(argv[0], state);
	if (!cmd->path)
		return (handle_command_error(state, argv[0], 127));
	execute_fork(cmd, state);
}
