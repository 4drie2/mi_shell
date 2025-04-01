/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthuilli <pthuilli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 09:17:15 by pthuilli          #+#    #+#             */
/*   Updated: 2025/04/01 09:38:25 by pthuilli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	get_output_flags(t_redir *redir)
{
	int	flags;

	if (redir->type == R_APPEND)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	return (flags);
}

/*
** Ouvre un fichier pour les redirections de sortie (écriture ou ajout).
** Si l'ouverture réussit, duplique le descripteur vers STDOUT.
** Retourne 0 en cas de succès, -1 en cas d'erreur.
*/
int	handle_output_redirection(t_redir *redir, t_state *state)
{
	int			fd;
	int			flags;
	struct stat	st;

	if (check_output_target(redir, &st, state) < 0)
		return (-1);
	flags = get_output_flags(redir);
	fd = open(redir->target, flags, 0644);
	if (fd < 0)
		return (handle_access_error(redir->target, state));
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

void	handle_command_error(t_state *state, char *cmd, int error_code)
{
	char	*exit_str;

	ft_putstr_fd(cmd, 2);
	if (error_code == 127)
		ft_putstr_fd(": command not found\n", 2);
	state->last_exit_status = error_code;
	exit_str = ft_itoa(state->last_exit_status);
	if (!exit_str)
		return ;
	state->envp = set_env_var(state->envp, "EXIT_STATUS", exit_str);
	free(exit_str);
}

// Compte le nombre de commandes liées par des pipes
int	count_commands(t_command *start_cmd)
{
	t_command	*cmd;
	int			count;

	cmd = start_cmd;
	count = 1;
	while (cmd->is_pipe && cmd->next)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}

