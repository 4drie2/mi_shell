/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthuilli <pthuilli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 16:45:30 by abidaux           #+#    #+#             */
/*   Updated: 2025/04/02 07:32:29 by pthuilli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	convert_tokens_to_commands(t_command **cmd, t_token *tokens, t_state *state)
{
	if (convert_tokens_to_command(*cmd, tokens, state) == -1)
	{
		free_command_list(*cmd);
		return (-1);
	}
	return (0);
}

int	validate_input(char *input)
{
	if (!input || !*input)
		return (0);
	return (1);
}

/*
** Transforme la ligne de commande en tokens (lexèmes) 
** et vérifie leur validité syntaxique.
** Prend en entrée la ligne brute et l'état du shell.
** Retourne la liste des tokens ou NULL en cas d'erreur.
** En cas d'échec, met à jour le statut de sortie dans la structure d'état.
*/
t_token	*tokenize_and_validate(char *input, t_state *state)
{
	t_token	*tokens;

	tokens = tokenize_input(input, state);
	if (!tokens)
	{
		state->last_exit_status = 2;
		return (NULL);
	}
	if (check_syntax_errors(tokens, state))
	{
		free_tokens(tokens);
		return (NULL);
	}
	return (tokens);
}
