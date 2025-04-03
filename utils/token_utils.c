/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abidaux <abidaux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 16:46:26 by abidaux           #+#    #+#             */
/*   Updated: 2025/04/03 10:04:13 by abidaux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*duplicate_redirection(const char c)
{
	char	*result;

	if (c == '<')
		result = ft_strdup("<<");
	else if (c == '>')
		result = ft_strdup(">>");
	else
		result = NULL;
	return (result);
}

static int	handle_redirection_tok(const char *input, int *i,
	t_token **token_list)
{
	char	*two;
	char	c[2];

	if (input[*i + 1] && input[*i + 1] == input[*i])
	{
		two = duplicate_redirection(input[*i]);
		if (!two || !create_and_add_token(two, token_list))
		{
			free(two);
			return (-1);
		}
		free(two);
		*i += 2;
	}
	else
	{
		c[0] = input[*i];
		c[1] = '\0';
		if (!create_and_add_token(c, token_list))
			return (-1);
		(*i)++;
	}
	return (0);
}

static int	handle_pipe_tok(int *i, t_token **token_list)
{
	if (!create_and_add_token("|", token_list))
		return (-1);
	(*i)++;
	return (0);
}

/**
 * Analyse une chaîne d'entrée et génère une liste de tokens.
 *
 * Parcourt la chaîne caractère par caractère pour identifier
 * les tokens suivants:
 * - Redirections (< et >)
 * - Pipes (|)
 * - Autres tokens (commandes, arguments, etc.)
 *
 * @param input Chaîne d'entrée à analyser
 * @param token_list Pointeur vers la liste de tokens à construire
 * @param state État global du shell
 * @return 0 en cas de succès, -1 en cas d'erreur
 */
static int	process_tokens(const char *input, t_token **token_list,
	t_state *state)
{
	int	i;

	i = 0;
	while (input[i])
	{
		i = skip_spaces(input, i);
		if (!input[i])
			break ;
		if (input[i] == '<' || input[i] == '>')
		{
			if (handle_redirection_tok(input, &i, token_list) == -1)
				return (-1);
		}
		else if (input[i] == '|')
		{
			if (handle_pipe_tok(&i, token_list) == -1)
				return (-1);
		}
		else
		{
			if (handle_token(token_list, input, &i, state) == -1)
				return (-1);
		}
	}
	return (0);
}

/**
 * Tokenise une chaîne d'entrée en une liste de tokens.
 *
 * Cette fonction effectue deux vérifications principales avant la tokenisation:
 * 1. Vérifie si toutes les citations (guillemets) sont correctement fermées
 * 2. Exécute le traitement des tokens sur la chaîne d'entrée
 *
 * En cas d'erreur à n'importe quelle étape, la fonction nettoie les ressources
 * et renvoie NULL.
 *
 * @param input Chaîne d'entrée à tokeniser
 * @param state État global du shell
 * @return Liste de tokens en cas de succès, NULL en cas d'erreur
 */
t_token	*tokenize_input(const char *input, t_state *state)
{
	t_token	*tokens;

	tokens = NULL;
	if (check_unclosed_quotes(input))
	{
		ft_putstr_fd("minishell: syntax error: unclosed quotes\n", 2);
		state->last_exit_status = 2;
		return (NULL);
	}
	if (process_tokens(input, &tokens, state) == -1)
	{
		free_tokens(tokens);
		return (NULL);
	}
	return (tokens);
}
