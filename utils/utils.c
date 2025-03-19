/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthuilli <pthuilli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 20:45:17 by pthuilli          #+#    #+#             */
/*   Updated: 2025/03/19 07:00:44 by pthuilli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	free_input_redirection(t_command *cmd)
{
	int	i;

	i = 0;
	while (i < cmd->nb_in)
	{
		if (cmd->in[i])
		{
			free(cmd->in[i]->target);
			free(cmd->in[i]);
			cmd->in[i] = NULL;
		}
		i++;
	}
	free(cmd->in);
	cmd->in = NULL;
}

static void	free_output_redirection(t_command *cmd)
{
	int	i;

	i = 0;
	while (i < cmd->nb_out)
	{
		if (cmd->out[i])
		{
			free(cmd->out[i]->target);
			free(cmd->out[i]);
			cmd->out[i] = NULL;
		}
		i++;
	}
	free(cmd->out);
	cmd->out = NULL;
}

void	free_command(t_command *cmd)
{
	int	i;

	if (!cmd)
		return ;
	if (cmd->args)
	{
		i = -1;
		while (cmd->args[++i])
		{
			free(cmd->args[i]);
			cmd->args[i] = NULL;
		}
		free(cmd->args);
		cmd->args = NULL;
	}
	free_input_redirection(cmd);
	free_output_redirection(cmd);
	free(cmd);
}

int	skip_spaces(const char *input, int i)
{
	while (input[i] && ft_isspace(input[i]))
		++i;
	return (i);
}

int	is_empty_or_space(const char *str)
{
	int	i;

	i = -1;
	while (str[++i])
		if (!ft_isspace(str[i]))
			return (0);
	return (1);
}
