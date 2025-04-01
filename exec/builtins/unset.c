/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthuilli <pthuilli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 11:07:22 by pthuilli          #+#    #+#             */
/*   Updated: 2025/04/01 11:11:10 by pthuilli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/*
** Vérifie si la commande unset possède au moins un argument.
** Retourne 1 si oui, 0 sinon.
*/
static int	has_unset_arguments(char **argv)
{
	if (!argv[1])
		return (0);
	return (1);
}

/*
** Cherche l'index de la variable d'environnement correspondant à la clé.
** Retourne l'index si trouvé, -1 sinon.
*/
static int	find_env_var_index(const char *key, char **envp)
{
	int		j;
	size_t	key_len;

	key_len = ft_strlen(key);
	j = 0;
	while (envp[j] != NULL)
	{
		if (ft_strncmp(envp[j], key, key_len) == 0
			&& envp[j][key_len] == '=')
			return (j);
		j++;
	}
	return (-1);
}

/*
** Supprime la variable d'environnement à l'index donné.
** Décale les éléments suivants vers la gauche et termine par NULL.
*/
static void	remove_env_var_at_index(int index, char **envp)
{
	int	j;

	free(envp[index]);
	j = index;
	while (envp[j + 1] != NULL)
	{
		envp[j] = envp[j + 1];
		j++;
	}
	envp[j] = NULL;
}

/*
** Supprime la variable d'environnement correspondant à la clé.
** Ne fait rien si la clé n'est pas trouvée.
*/
static void	remove_env_var(const char *key, char **envp)
{
	int	index;

	index = find_env_var_index(key, envp);
	if (index != -1)
		remove_env_var_at_index(index, envp);
}

/*
** Fonction principale qui gère la commande unset.
** Parcourt les arguments et supprime chaque variable
** d'environnement correspondante.
*/
void	handle_unset_command(char **argv, char **envp)
{
	int	i;

	if (!has_unset_arguments(argv))
		return ;
	i = 1;
	while (argv[i])
	{
		remove_env_var(argv[i], envp);
		i++;
	}
}
