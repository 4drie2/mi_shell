/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthuilli <pthuilli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 19:11:41 by abidaux           #+#    #+#             */
/*   Updated: 2025/04/01 10:34:36 by pthuilli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * Recherche une commande exécutable dans les chemins du PATH.
 *
 * @param paths Tableau de chemins à parcourir (généralement issu de PATH)
 * @param cmd Nom de la commande à rechercher
 * @return Chemin complet de la commande si trouvée, NULL sinon (libère paths)
 */
static char	*try_paths(char **paths, char *cmd)
{
	int		i;
	char	*tmp;
	char	*full;

	i = -1;
	while (paths[++i])
	{
		tmp = ft_strjoin(paths[i], "/");
		if (!tmp)
			return (NULL);
		full = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!full)
			return (NULL);
		if (access(full, X_OK) == 0)
			return (ft_freestr(paths), full);
		free(full);
	}
	return (ft_freestr(paths), NULL);
}

/**
 * Trouve le chemin complet d'une commande en utilisant PATH ou un chemin absolu/relatif.
 *
 * @param cmd Nom ou chemin de la commande à rechercher
 * @param state Structure contenant l'environnement
 * @return Chemin complet de la commande si trouvée, NULL sinon
 */
char	*get_command_path(char *cmd, t_state *state)
{
	struct stat	st;
	char		**paths;
	char		*path_env;
	char		*validated_path;


	if (!access(cmd, X_OK))
	{
		if (!stat(cmd, &st) && !S_ISDIR(st.st_mode))
			return (ft_strdup(cmd));
		else
			return (NULL);
	}
	path_env = search_in_local_env(state->envp, "PATH");
	if (!path_env || path_env[0] == '\0')
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	validated_path = try_paths(paths, cmd);
	return (validated_path);
}

/**
 * Gère les erreurs d'accès aux fichiers avec des messages appropriés.
 *
 * @param target Nom du fichier/dossier cible qui a provoqué l'erreur
 * @param state Structure d'état du shell pour mettre à jour le statut de sortie
 * @return Toujours -1 pour indiquer une erreur
 */
int	handle_access_error(char *target, t_state *state)
{
	if (errno == EACCES)
	{
		ft_putstr_fd(target, 2);
		ft_putstr_fd(": Permission denied", 2);
		state->last_exit_status = 1;
	}
	else
	{
		ft_putstr_fd(target, 2);
		ft_putstr_fd(": No such file or directory", 2);
		state->last_exit_status = 1;
	}
	return (-1);
}

/**
 * Détermine si une commande est spécifiée avec un chemin absolu ou relatif.
 *
 * @param cmd La commande à analyser
 * @return 1 si le chemin est absolu ou relatif, 0 sinon
 */
int	is_path_absolute_or_relative(char *cmd)
{
	if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/'))
		return (1);
	return (0);
}

/**
 * Valide le chemin d'une commande en vérifiant son existence,
 * son type (fichier vs dossier) et ses droits d'exécution.
 *
 * @param cmd Chemin de la commande à valider
 * @param path Pointeur pour stocker le chemin validé (dupliquer)
 * @param st Structure d'état du shell
 * @param check Flag pour indiquer si des vérifications supplémentaires sont nécessaires
 * @return 1 si le chemin est valide, 0 sinon
 */
int	validate_command_path(char *cmd, char **path, t_state *st, bool *check)
{
	if (!check_file_existence(cmd, st, check))
		return (0);
	if (!check_dir_exec(cmd, st, check))
		return (0);
	*path = ft_strdup(cmd);
	if (!path)
		return (0);
	return (1);
}
