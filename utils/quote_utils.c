/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abidaux <abidaux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 16:46:09 by abidaux           #+#    #+#             */
/*   Updated: 2025/03/26 16:46:11 by abidaux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	should_parse_as_special(const char *input, int i)
{
	int	in_single_quotes;
	int	in_double_quotes;
	int	j;

	in_single_quotes = 0;
	in_double_quotes = 0;
	j = 0;
	while (j < i)
	{
		if (input[j] == '\'' && !in_double_quotes)
			in_single_quotes = !in_single_quotes;
		else if (input[j] == '"' && !in_single_quotes)
			in_double_quotes = !in_double_quotes;
		j++;
	}
	return (!in_single_quotes && !in_double_quotes);
}

// Fonction principale pour gérer un token
int	handle_token(t_token **head, const char *input,
	int *i, t_state *state)
{
	char	*word;

	word = ft_strdup("");
	if (!word)
		return (-1);
	while (input[*i] && !ft_isspace(input[*i]))
	{
		if (input[*i] == '|' && should_parse_as_special(input, *i))
			break ;
		if (process_current_char(input, i, &word, state) == -1)
		{
			free(word);
			return (-1);
		}
	}
	if (!create_and_add_token(word, head))
	{
		free(word);
		return (-1);
	}
	free(word);
	return (0);
}

// Gère les citations simples et doubles
char	*handle_quotes(const char *input, int *i, char **envp, t_state *state)
{
	char	*temp;

	if (input[*i] == '\'')
		temp = parse_single_quote(input, i);
	else
		temp = parse_double_quote(input, i, envp, state);
	return (temp);
}

// Gère les variables d'environnement
char	*handle_variable(const char *input, int *i, t_state *state)
{
	char	*temp;

	if (input[*i + 1] == '\0' || ft_isspace(input[*i + 1])
		|| input[*i + 1] == '"' || input[*i + 1] == '\'')
	{
		temp = ft_strdup("$");
		(*i)++;
	}
	else
	{
		temp = expand_variable((char *)input, i, state);
		if (!temp)
			temp = ft_strdup("");
	}
	return (temp);
}
