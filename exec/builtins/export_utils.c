/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthuilli <pthuilli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 11:07:10 by pthuilli          #+#    #+#             */
/*   Updated: 2025/04/01 11:10:31 by pthuilli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/*
** Affiche l'environnement trié sous forme "declare -x VAR=VALUE".
*/
void	print_sorted_env(char **envp)
{
	char	**sorted;
	int		i;

	sorted = get_sorted_env(envp);
	if (!sorted)
		return ;
	i = 0;
	while (sorted[i])
	{
		print_err_msg("declare -x ", sorted[i], "\n");
		i++;
	}
	ft_free_array(sorted);
}

/*
** Si 'export' est appelé sans argument, on affiche l'env trié et on s'arrête.
** Retourne 1 si on a déjà géré l'export, 0 sinon.
*/
static int	handle_export_no_args(char **argv, t_state *state)
{
	if (!argv[1])
	{
		print_sorted_env(state->envp);
		state->last_exit_status = 0;
		return (1);
	}
	return (0);
}

/*
** Ajoute ou met à jour la variable d'environnement (si elle contient '=').
*/
static void	add_export_var(char *arg, t_state *state)
{
	char	*equal_sign;
	char	*key;
	char	*value;
	char	*normalized_value;

	equal_sign = ft_strchr(arg, '=');
	if (!equal_sign)
		return ;
	key = ft_substr(arg, 0, equal_sign - arg);
	value = ft_strdup(equal_sign + 1);
	normalized_value = normalize_spaces(value);
	state->envp = set_env_var(state->envp, key, normalized_value);
	free(key);
	free(value);
	free(normalized_value);
}

/*
** Gère un seul argument 'export'.
** Retourne 0 si identifiant invalide, 1 sinon.
*/
static int	handle_export_arg(char *arg, t_state *state)
{
	if (!is_valid_identifier(arg))
	{
		print_err_msg("export: `", arg, "': not a valid identifier\n");
		state->last_exit_status = 1;
		return (1);
	}
	add_export_var(arg, state);
	return (0);
}

/*
** Fonction principale pour gérer la commande 'export'.
** - Affiche l'env trié si pas d'arguments.
** - Ignore en cas de pipe.
** - Sinon, pour chaque arg, vérifie la validité et ajoute la var.
*/
void	handle_export_command(char **argv, t_state *state)
{
	int	i;
	int	had_error;

	if (handle_export_no_args(argv, state))
		return ;
	if (state->num_pipes > 0)
	{
		state->last_exit_status = 0;
		return ;
	}
	i = 1;
	had_error = 0;
	while (argv[i])
	{
		if (handle_export_arg(argv[i], state))
			had_error = 1;
		i++;
	}
	state->last_exit_status = had_error;
}
