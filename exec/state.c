/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abidaux <abidaux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 17:35:34 by abidaux           #+#    #+#             */
/*   Updated: 2025/04/02 18:08:10 by abidaux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * Exécute les commandes internes (builtins) du shell.
 * Identifie la commande à partir du premier argument
 * et appelle le gestionnaire approprié.
 *
 * @param cmd Structure de commande contenant les arguments à traiter
 * @param state État global du shell
 * @return 0 si ce n'est pas une commande builtin
 *         1 si c'est une commande builtin qui a été exécutée
 *         2 spécifiquement pour la commande exit (signale la fin du shell)
 */
int	exec_builtins(t_command *cmd, t_state *state)
{
	if (!cmd->args || !cmd->args[0])
		return (0);
	else if (!ft_strcmp(cmd->args[0], "exit"))
		return (handle_exit(cmd, state), 2);
	else if (!ft_strcmp(cmd->args[0], "cd"))
		return (handle_cd_command(cmd->args, state), 1);
	else if (!ft_strcmp(cmd->args[0], "pwd"))
		return (handle_pwd_command(state), 1);
	else if (!ft_strcmp(cmd->args[0], "env"))
		return (handle_env_command(state->envp, state), 1);
	else if (!ft_strcmp(cmd->args[0], "echo"))
		return (handle_echo_command(cmd, state), 1);
	else if (!ft_strcmp(cmd->args[0], "unset"))
		return (handle_unset_command(cmd->args, state->envp), 1);
	else if (!ft_strcmp(cmd->args[0], "export"))
		return (handle_export_command(cmd->args, state), 1);
	else if (!ft_strcmp(cmd->args[0], "."))
		return (handle_source_command(cmd->args, state), 1);
	return (0);
}

/**
 * Gère la commande source (.) qui exécute un fichier de commandes.
 * Vérifie si un argument est fourni, sinon affiche une erreur.
 *
 * @param args Tableau d'arguments où args[1] doit contenir le chemin du fichier
 * @param state État global du shell pour mettre à jour le statut de sortie
 */
void	handle_source_command(char **args, t_state *state)
{
	if (!args[1])
	{
		ft_putstr_fd(".: command not found\n", 2);
		state->last_exit_status = 127;
		return ;
	}
}

/**
 * Réinitialise l'état du shell pour préparer une nouvelle exécution de commande.
 * Met à zéro les compteurs, réinitialise les index
 * et les descripteurs de fichiers,
 * et nullifie les pointeurs de ressources.
 *
 * @param state Pointeur vers la structure d'état à réinitialiser
 * @return Aucune valeur (void)
 */
void	reset_state(t_state *state)
{
	state->num_pipes = 0;
	state->index = 0;
	state->o_stdin = -1;
	state->o_stdout = -1;
	state->pids = NULL;
	state->pipes = NULL;
}

/**
 * Vérifie la syntaxe des commandes pour s'assurer qu'elles
 * sont correctement formées.
 * Détecte notamment les pipes mal formés (sans commande les précédant).
 *
 * @param start_cmd Pointeur vers la première commande à vérifier
 * @param state Pointeur vers l'état global pour mettre à jour le statut d'erreur
 * @return 1 si la syntaxe est correcte, 0 en cas d'erreur
 */
int	check_synthax(t_command *start_cmd, t_state *state)
{
	if (!start_cmd->args[0] && start_cmd->is_pipe)
	{
		ft_putstr_fd("minishell: synthax error near unexpected token `|'\n", 2);
		state->last_exit_status = 2;
		return (0);
	}
	return (1);
}
