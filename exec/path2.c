/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthuilli <pthuilli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 14:56:49 by pthuilli          #+#    #+#             */
/*   Updated: 2025/04/03 13:10:30 by pthuilli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_file_existence(char *cmd, t_state *st, bool *check)
{
	if (access(cmd, F_OK) == -1)
	{
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		st->last_exit_status = 127;
		*check = true;
		return (0);
	}
	return (1);
}

int	check_dir_and_exec(char *cmd, t_state *st, bool *check)
{
	struct stat	sb;

	if (stat(cmd, &sb) == -1)
		return (0);
	if (S_ISDIR(sb.st_mode))
	{
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": Is a directory\n", 2);
		st->last_exit_status = 126;
		*check = true;
		return (0);
	}
	if (access(cmd, X_OK) == -1)
	{
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		st->last_exit_status = 126;
		*check = true;
		return (0);
	}
	return (1);
}
