/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthuilli <pthuilli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 16:44:49 by abidaux           #+#    #+#             */
/*   Updated: 2025/04/01 09:38:10 by pthuilli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char *get_executable_path(t_command *cmd, t_state *state)
{
	char	*path;

	if (cmd->args[0][0] == '/' || (cmd->args[0][0] == '.'
		&& cmd->args[0][1] == '/'))
		path = valide_path(cmd->args[0]);
	else
		path = get_command_path(cmd->args[0], state);
	if (!path)
	{
		print_err_msg(cmd->args[0], ":cmd not found", "\n");
		while (cmd->prev)
			cmd = cmd->prev;
		free_command_list(cmd);
		free_envp(state->envp);
		state->last_exit_status = 127;
		exit (127);
	}
	return (path);
}

/* WHAT A FUCKING 🌟beautyfull🌟 FONCTION */
static void	execute_cmd(char *path, char **args, char **envp)
{
	if (execve(path, args, envp) == -1)
	{
		perror(args[0]);
		free(envp);
		free(path);
		exit(126);
	}
}

void	execute_external(t_command *cmd, t_state *state)
{
	char	*path;

	path = get_executable_path(cmd, state);
	if (!path)
	{
		free_command_list(cmd);
		free_envp(state->envp);
		exit(state->last_exit_status);
	}
	execute_cmd(path, cmd->args, state->envp);
}

int	is_builtin(char *cmd)
{
	if (!ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "echo")
		|| !ft_strcmp(cmd, "exit") || !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "env")
		|| !ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, ".")
		|| !ft_strcmp(cmd, "neosnitch"))
		return (1);
	return (0);
}


/*
stat() : fonction systeme, place les metadonnee dans st et retourne 0 si succes
S_ISDIR retourne 1 si c'est un repertoir (sinon 0)
*/
int	check_output_target(t_redir *redir, struct stat *st, t_state *state)
{
	if (stat(redir->target, st) == 0 && S_ISDIR(st->st_mode))
	{
		ft_putstr_fd(redir->target, 2);
		ft_putstr_fd(": Is a directory\n", 2);
		state->last_exit_status = 126;
		return (-1);
	}
	if (!redir->target || redir->target[0] == '\0')
	{
		ft_putstr_fd("Syntax error near unexpected token 'newline'\n", 2);
		state->last_exit_status = 2;
		return (-1);
	}
	return (0);
}
