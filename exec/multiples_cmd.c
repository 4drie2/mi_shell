/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiples_cmd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abidaux <abidaux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 22:40:04 by abidaux           #+#    #+#             */
/*   Updated: 2025/03/30 18:50:27 by abidaux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * Exécute ces commandes builtin : env, export, exit, cd.
 *
 * Cette fonction identifie et exécute les commandes internes du shell
 * en fonction du premier argument de la commande.
 *
 * @param cmd Structure contenant les arguments de la commande
 * @param temp_state État temporaire du shell pour l'exécution de la commande
 */
static void	execute_builtin_part1(t_command *cmd, t_state *temp_state)
{
	if (!ft_strcmp(cmd->args[0], "env"))
	{
		handle_env_command(temp_state->envp, temp_state);
		if (temp_state->last_exit_status)
			temp_state->last_exit_status = 1;
		else
			temp_state->last_exit_status = 0;
	}
	else if (!ft_strcmp(cmd->args[0], "export"))
		handle_export_command(cmd->args, temp_state);
	else if (!ft_strcmp(cmd->args[0], "exit"))
	{
		if (cmd->args[1] && cmd->args[2])
		{
			free_envp(temp_state->envp);
			while (cmd->prev)
				cmd = cmd->prev;
			free_command_list(cmd);
			print_err_and_exit("exit: ", "", "too many arguments\n", 1);
		}
		handle_exit(cmd, temp_state);
	}
	else if (!ft_strcmp(cmd->args[0], "cd"))
		handle_cd_command(cmd->args, temp_state);
}

/**
 * Exécute les commandes builtin (seconde partie: pwd, echo, unset, neosnitch).
 *
 * Cette fonction identifie et exécute les commandes internes supplémentaires
 * du shell en fonction du premier argument.
 *
 * @param cmd Structure contenant les arguments de la commande
 * @param temp_state État temporaire du shell pour l'exécution
 */
static void	execute_builtin_part2(t_command *cmd, t_state *temp_state)
{
	if (!ft_strcmp(cmd->args[0], "pwd"))
		handle_pwd_command(temp_state);
	else if (!ft_strcmp(cmd->args[0], "echo"))
		handle_echo_command(cmd, temp_state);
	else if (!ft_strcmp(cmd->args[0], "unset"))
		handle_unset_command(cmd->args, temp_state->envp);
	else if (!ft_strcmp(cmd->args[0], "neosnitch"))
		handle_noesnitch_command(cmd->args, temp_state);
}

/**
 * Exécute une commande builtin du shell.
 *
 * Cette fonction détermine quel type de commande builtin est demandé et
 * délègue l'exécution à la fonction appropriée.
 *
 * @param cmd Structure contenant les arguments de la commande
 * @param temp_state État temporaire du shell pour l'exécution
 */
void	execute_builtin(t_command *cmd, t_state *temp_state)
{
	if (!ft_strcmp(cmd->args[0], "env")
		|| !ft_strcmp(cmd->args[0], "export")
		|| !ft_strcmp(cmd->args[0], "exit")
		|| !ft_strcmp(cmd->args[0], "cd"))
		execute_builtin_part1(cmd, temp_state);
	else
		execute_builtin_part2(cmd, temp_state);
}

/**
 * Exécute une commande du shell (builtin ou externe).
 *
 * Cette fonction prépare l'environnement, gère les redirections et
 * détermine si la commande est builtin ou externe pour l'exécuter.
 *
 * @param cmd Structure contenant la commande à exécuter
 * @param state État global du shell
 */
void	execute_command(t_command *cmd, t_state *state)
{
	t_state	temp_state;
	char	**temp_env;

	handle_redirection_or_exit(cmd, state);
	copy_environment_or_exit(state, &temp_state, &temp_env);
	handle_no_args_or_exit(cmd, state, temp_env);
	if (is_builtin(cmd->args[0]))
	{
		free_envp(state->envp);
		exec_builtins(cmd, &temp_state);
		while (cmd->prev)
			cmd = cmd->prev;
		free_command_list(cmd);
		free_envp(temp_env);
		exit(state->last_exit_status);
	}
	free_envp(state->envp);
	execute_external(cmd, &temp_state);
	free_envp(temp_env);
}

/**
 * Gère l'exécution de commandes avec plusieurs pipes.
 *
 * Cette fonction coordonne l'allocation, la création des pipes,
 * l'exécution des commandes et le nettoyage final.
 *
 * @param start_cmd Pointeur vers la première commande de la liste chaînée
 * @param state État global du shell contenant les ressources
 */
void	handle_multiple_pipes(t_command *start_cmd, t_state *state)
{
	state->num_pipes = allocate_and_create_pipes(start_cmd,
		&state->pipes, &state->pids, state);
	if (state->num_pipes == -1)
		return ;
	execute_and_cleanup(start_cmd, state,
		(int (*)[2])state->pipes, state->pids);
}
