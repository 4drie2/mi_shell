/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthuilli <pthuilli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 16:47:41 by abidaux           #+#    #+#             */
/*   Updated: 2025/04/01 09:11:56 by pthuilli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * Vérifie si le chemin spécifié correspond à un répertoire.
 *
 * @param path Le chemin à vérifier
 * @return 1 si ce n'est pas un répertoire ou si stat échoue, 0 si c'est un répertoire
 *         (affiche également un message d'erreur dans ce cas)
 */
static int	check_if_directory(char *path)
{
	struct stat	st;

	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
	{
		print_error("", path, ": Is a directory\n", 126);
		return (0);
	}
	return (1);
}

/**
 * Gère une redirection d'entrée (< file).
 * Vérifie que la cible n'est pas un répertoire, ouvre le fichier en lecture
 * et redirige l'entrée standard vers ce fichier.
 *
 * @param redir Structure contenant les informations de redirection
 * @param state État global du shell
 * @return 0 en cas de succès, -1 en cas d'erreur
 */
static int	handle_input_redirection(t_redir *redir, t_state *state)
{
	int	fd;

	if (!check_if_directory(redir->target))
		return (-1);
	fd = open(redir->target, O_RDONLY);
	if (fd < 0)
		return (handle_access_error(redir->target, state));
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close (fd);
	return (0);
}

/**
 * Traite toutes les redirections d'entrée d'une commande.
 * Parcourt le tableau des redirections d'entrée et applique chacune d'elles.
 *
 * @param cmd Structure de commande contenant les redirections à traiter
 * @param state État global du shell
 * @return 0 en cas de succès, -1 si une des redirections échoue
 */
static int	process_input_redirection(t_command *cmd, t_state *state)
{
	int	i;

	i = -1;
	while (++i < cmd->nb_in)
		if (handle_input_redirection(cmd->in[i], state) == -1)
			return (-1);
	return (0);
}

/**
 * Traite toutes les redirections de sortie d'une commande.
 * Parcourt le tableau des redirections de sortie et applique chacune d'elles.
 *
 * @param cmd Structure de commande contenant les redirections à traiter
 * @param state État global du shell
 * @return 0 en cas de succès, -1 si une des redirections échoue
 */
static int	process_output_redirections(t_command *cmd, t_state *state)
{
	int	i;

	i = -1;
	while (++i < cmd->nb_out)
		if (handle_output_redirection(cmd->out[i], state) == -1)
			return (-1);
	return (0);
}

/**
 * Gère toutes les redirections (entrée et sortie) d'une commande.
 * Applique d'abord toutes les redirections d'entrée, puis toutes les redirections de sortie.
 * Si une redirection échoue, la fonction s'arrête immédiatement.
 *
 * @param cmd Structure de commande contenant toutes les redirections à traiter
 * @param state État global du shell
 * @return 0 en cas de succès, -1 si une des redirections échoue
 */
int	handle_all_redirections(t_command *cmd, t_state *state)
{
	if (process_input_redirection(cmd, state) == -1)
		return (-1);
	if (process_output_redirections(cmd, state) == -1)
		return (-1);
	return (0);
}
