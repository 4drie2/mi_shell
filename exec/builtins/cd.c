/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthuilli <pthuilli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 11:06:56 by pthuilli          #+#    #+#             */
/*   Updated: 2025/04/01 11:23:31 by pthuilli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*get_cd_path(char **argv, t_state *state)
{
	char	*path;
	char	*home;

	if (!argv[1] || (argv[1][0] == '~' && !argv[1][1]))
	{
		home = search_in_local_env(state->envp, "HOME");
		if (!home)
		{
			state->last_exit_status = 1;
			return (ft_putstr_fd("cd: HOME not set\n", 2), NULL);
		}
		return (ft_strdup(home));
	}
	else if (argv[1][0] == '-' && !argv[1][1])
	{
		path = search_in_local_env(state->envp, "OLDPWD");
		if (!path)
		{
			state->last_exit_status = 1;
			return (ft_putstr_fd("cd: OLDPWD not set\n", 2), NULL);
		}
		ft_putendl_fd(path, 1);
		return (ft_strdup(path));
	}
	return (ft_strdup(argv[1]));
}

void	update_env_vars(t_state *state)
{
	char	current[PATH_MAX];
	char	*oldpwd;

	oldpwd = search_in_local_env(state->envp, "PWD");
	if (oldpwd)
		state->envp = set_env_var(state->envp, "OLDPWD", oldpwd);
	if (getcwd(current, PATH_MAX))
	{
		state->envp = set_env_var(state->envp, "PWD", current);
		ft_strlcpy(state->current_path, current, PATH_MAX);
	}
	else
	{
		perror("cd: getcwd error");
		ft_strlcpy(state->current_path, "", 1);
	}
}

/*
** Vérifie si la commande cd possède trop d'arguments.
** Retourne 0 si erreur, 1 sinon.
*/
int	check_cd_args(char **argv)
{
	if (!argv)
		return (0);
	if (argv[1] && argv[2])
	{
		print_error("cd: ", "", "too many arguments\n", 1);
		return (0);
	}
	return (1);
}

/*
** Vérifie si le chemin existe.
** Retourne 0 si erreur, 1 sinon.
*/
int	check_cd_path_exists(char *path)
{
	if (access(path, F_OK) == -1)
	{
		print_error("cd: ", path, ": No such file or directory\n", 1);
		return (free(path), 0);
	}
	return (1);
}

/*
** Vérifie si le chemin est un dossier.
** Retourne 0 si erreur, 1 sinon.
*/
int	check_cd_path_dir(char *path)
{
	struct stat	st;

	if (stat(path, &st) == 0 && !S_ISDIR(st.st_mode))
	{
		print_error("cd: ", path, ": Not a directory\n", 1);
		return (free(path), 0);
	}
	return (1);
}
