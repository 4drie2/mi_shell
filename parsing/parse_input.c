/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthuilli <pthuilli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 20:35:24 by abidaux           #+#    #+#             */
/*   Updated: 2025/03/20 13:28:32 by pthuilli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	handle_incomplete_command(t_token **tokens,
	t_state *state)
{
	char	*line;
	t_token	*new_tokens;

	while (is_command_incomplete(*tokens, state))
	{
		printf("> ");
		line = readline(NULL);
		if (!line)
			return (2);
		new_tokens = tokenize_input(line, state);
		free(line);
		if (!new_tokens)
			return (-1);
		append_tokens(tokens, new_tokens);
		if (is_command_incomplete(*tokens, state) && !(*tokens)->next)
		{
			ft_putstr_fd(
				"minishell: syntax error: incomplete command after '|'\n", 2);
			return (2);
		}
	}
	return (0);
}

/*
** Initialise une nouvelle structure de commande.
** Retourne un pointeur vers la commande, ou NULL en cas d'échec.
*/
t_command	*initialize_command(t_state *state)
{
	t_command	*cmd;

	cmd = init_command();
	if (!cmd)
	{
		state->last_exit_status = 2;
		return (NULL);
	}
	return (cmd);
}

/*
** Gère les commandes incomplètes et convertit les tokens en commandes.
** Retourne 0 si réussi, -1 sinon.
*/
int	handle_commands(t_command **cmd, t_token *tokens,
	t_state *state)
{
	if (handle_incomplete_command(&tokens, state) != 0
		|| convert_tokens_to_commands(cmd, tokens, state) == -1)
	{
		state->last_exit_status = 2;
		return (-1);
	}
	return (0);
}

/*
** Nettoie les ressources en cas d'erreur.
*/
void	cleanup_on_error(t_token *tokens, t_command *cmd,
	int error_code, t_state *state)
{
	state->last_exit_status = error_code;
	free_tokens(tokens);
	if (cmd)
		free_command_list(cmd);
}

/*
** Analyse l'entrée utilisateur et construit la structure de commande.
** Retourne un pointeur vers la commande, ou NULL en cas d'échec.
*/
t_command	*parse_input(char *input, t_state *state)
{
	t_token		*tokens;
	t_command	*cmd;

	if (!validate_input(input))
		return (NULL);
	tokens = tokenize_and_validate(input, state);
	if (!tokens)
		return (NULL);
	cmd = initialize_command(state);
	if (!cmd)
	{
		free_tokens(tokens);
		return (NULL);
	}
	if (handle_commands(&cmd, tokens, state) == -1)
	{
		cleanup_on_error(tokens, cmd, 2, state);
		return (NULL);
	}
	free_tokens(tokens);
	return (cmd);
}
