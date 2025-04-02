/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthuilli <pthuilli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 07:49:37 by pthuilli          #+#    #+#             */
/*   Updated: 2025/04/02 07:50:05 by pthuilli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_command	*init_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (free(cmd), NULL);
	ft_memset(cmd, 0, sizeof(t_command));
	cmd->args = malloc(sizeof(char *) * 256);
	if (!cmd->args)
		return (free(cmd), NULL);
	ft_memset(cmd->args, 0, sizeof(char *) * 256);
	cmd->in = NULL;
	cmd->nb_in = 0;
	cmd->out = NULL;
	cmd->nb_out = 0;
	cmd->is_pipe = 0;
	cmd->next = NULL;
	cmd->prev = NULL;
	return (cmd);
}

// Fonction pour remplir les arguments d'une commande
int	fill_command_args(t_command *cmd, t_token *current, int *index)
{
	char	*arg_copy;

	if (*index >= 255)
		return (-1);
	arg_copy = ft_strdup(current->content);
	if (!arg_copy)
	{
		while (--(*index) >= 0)
		{
			free(cmd->args[*index]);
			cmd->args[*index] = NULL;
		}
		return (-1);
	}
	cmd->args[*index] = arg_copy;
	(*index)++;
	cmd->args[*index] = NULL;
	return (0);
}

int	handle_pipe(t_command **current_cmd, int *arg_index, t_state *state)
{
	(*current_cmd)->is_pipe = 1;
	(*current_cmd)->args[*arg_index] = NULL;
	(*current_cmd)->next = initialize_command(state);
	if (!(*current_cmd)->next)
	{
		perror("Error creating new command node");
		free(*current_cmd);
		return (-1);
	}
	*current_cmd = (*current_cmd)->next;
	*arg_index = 0;
	(*current_cmd)->args[0] = NULL;
	return (0);
}

int	handle_arguments(t_command *current_cmd, t_token *current, int *arg_index)
{
	if (fill_command_args(current_cmd, current, arg_index) == -1)
	{
		while (--(*arg_index) >= 0)
		{
			free(current_cmd->args[*arg_index]);
			current_cmd->args[*arg_index] = NULL;
		}
		return (-1);
	}
	return (0);
}
