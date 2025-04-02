/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_free.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthuilli <pthuilli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 07:49:39 by pthuilli          #+#    #+#             */
/*   Updated: 2025/04/02 08:31:00 by pthuilli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// void	free_args(char **args)
// {
// 	int	i;

// 	if (!args)
// 		return ;
// 	i = 0;
// 	while (args[i])
// 	{
// 		free(args[i]);
// 		args[i] = NULL;
// 		i++;
// 	}
// 	free(args);
// 	args = NULL;
// }

/*
** free_command_list:
**   Parcourt la liste chaînée de t_command et libère chaque noeud.
*/
// void	free_command_list(t_command *cmd)
// {
// 	t_command	*temp;

// 	while (cmd)
// 	{
// 		temp = cmd;
// 		cmd = cmd->next;
// 		free_command(temp);
// 	}
// }
