/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthuilli <pthuilli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 11:06:59 by pthuilli          #+#    #+#             */
/*   Updated: 2025/04/01 11:08:59 by pthuilli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"


void	handle_echo_command(t_command *cmd, t_state *state)
{
	int		i;
	int		newline;

	if (!cmd->args[1])
		return (ft_putchar_fd('\n', STDOUT_FILENO));
	i = 1;
	newline = 1;
	while (cmd->args[i] && ft_strncmp(cmd->args[i], "-n", 2) == 0
		&& has_equals_as((cmd->args[i] + 2), "n"))
	{
		newline = 0;
		i++;
	}
	while (cmd->args[i])
	{
		ft_putstr_fd(cmd->args[i], STDOUT_FILENO);
		if (cmd->args[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (newline)
		ft_putchar_fd('\n', STDOUT_FILENO);
	state->last_exit_status = 0;
}
