/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthuilli <pthuilli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 18:52:05 by abidaux           #+#    #+#             */
/*   Updated: 2025/04/01 09:37:53 by pthuilli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * Configure les descripteurs de fichiers pour les pipes dans un processus enfant.
 * Redirige stdin depuis le pipe précédent (s'il existe) et stdout vers le pipe suivant
 * (s'il existe), puis ferme tous les descripteurs de pipes inutilisés.
 *
 * @param pipefds Tableau de descripteurs de fichiers des pipes [num_pipes][2]
 * @param state État global contenant l'index actuel du processus et le nombre total de pipes
 */
void	setup_pipes(int pipefds[][2], t_state *state)
{
	if (state->index > 0)
		if (dup2(pipefds[state->index - 1][0], STDIN_FILENO) == -1)
			exit(1);
	if (state->index < state->num_pipes)
		if (dup2(pipefds[state->index][1], STDOUT_FILENO) == -1)
			exit(1);
	close_pipes(pipefds, state->num_pipes);
}

/**
 * Ferme tous les descripteurs de fichiers des pipes.
 *
 * Cette fonction parcourt tous les pipes et ferme leurs extrémités
 * de lecture (0) et d'écriture (1). Elle est généralement appelée
 * après avoir configuré les redirections ou lorsque les pipes ne sont plus nécessaires.
 *
 * @param pipefds Tableau de descripteurs de fichiers des pipes [numpipes][2]
 * @param numpipes Nombre total de pipes à fermer
 */
void	close_pipes(int	pipefds[][2], int numpipes)
{
	int	i;

	i = -1;
	while (++i < numpipes)
	{
		close(pipefds[i][0]);
		close(pipefds[i][1]);
	}
}

/**
 * Affiche un message d'erreur formaté sur stderr puis termine le processus.
 *
 * Format du message d'erreur: "[cmd][path][msg]"
 * Par exemple: "minishell: cd: home: No such file or directory"
 *
 * @param cmd Commande à l'origine de l'erreur (ex: "minishell: ")
 * @param path Chemin ou argument concerné (ex: "cd: ")
 * @param msg Message d'erreur explicatif (ex: "No such file or directory")
 * @param exit_code Code de sortie à utiliser pour exit()
 */

void	print_error_and_exit(char *cmd, char *path, char *msg, int exit_code)
{
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(msg, 2);
	exit(exit_code);
}

/**
 * Affiche un message d'erreur formaté sur stderr et renvoie un code d'erreur.
 *
 * Format du message d'erreur: "[cmd][path][msg]"
 * Par exemple: "minishell: cd: home: No such file or directory"
 *
 * @param cmd Commande à l'origine de l'erreur (ex: "minishell: ")
 * @param path Chemin ou argument concerné (ex: "cd: ")
 * @param msg Message d'erreur explicatif (ex: "No such file or directory")
 * @param exit_code Code d'erreur à retourner
 * @return Le code d'erreur spécifié
 */
int	print_error(char *cmd, char *path, char *msg, int exit_code)
{
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(msg, 2);
	return (exit_code);
}

/**
 * Valide si un chemin de commande existe et est exécutable.
 *
 * Cette fonction vérifie si le chemin donné existe et vérifie les permissions
 * d'exécution. Elle affiche les erreurs appropriées en cas de problème.
 *
 * @param cmd_path Chemin de la commande à valider
 * @return Une copie du chemin si valide, NULL sinon
 */
char	*valide_path(char *cmd_path)
{
	struct stat	path_stat;

	if (!stat(cmd_path, &path_stat))
	{
		if (access(cmd_path, X_OK) != 0)
			print_error("", cmd_path, ": Permission denied\n", 126);
		return (ft_strdup(cmd_path));
	}
	print_error("", cmd_path, ": No such file or directory\n",127);
	return (NULL);
}
