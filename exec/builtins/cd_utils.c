/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthuilli <pthuilli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 11:06:53 by pthuilli          #+#    #+#             */
/*   Updated: 2025/04/01 11:08:31 by pthuilli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/*
** Vérifie les permissions d'accès au dossier.
** Retourne 0 si erreur, 1 sinon.
*/
int	check_cd_path_permission(char *path)
{
	if (access(path, X_OK) == -1)
	{
		print_error("cd: ", path, ": Permission denied\n", 0);
		return (free(path), 0);
	}
	return (1);
}

/*
** Regroupe toutes les vérifications (existence, dossier, permission) 
** sur le path.
** Retourne 0 si erreur, 1 sinon.
*/
int	check_cd_path(char *path, t_state *state)
{
	if (!path)
		return (state->last_exit_status = 1, 0);
	if (!check_cd_path_exists(path))
		return (0);
	if (!check_cd_path_dir(path))
		return (0);
	if (!check_cd_path_permission(path))
		return (0);
	return (1);
}

/*
** Fonction principale qui gère la commande cd.
** Découpée en sous-fonctions pour respecter la Norminette.
*/
void	handle_cd_command(char **argv, t_state *state)
{
	char	*path;

	if (!check_cd_args(argv))
		return ;
	path = get_cd_path(argv, state);
	if (!check_cd_path(path, state))
		return ;
	if (chdir(path) != 0)
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(path, 2);
		perror(": ");
		free(path);
		return ;
	}
	update_env_vars(state);
	free(path);
	state->last_exit_status = 0;
}
