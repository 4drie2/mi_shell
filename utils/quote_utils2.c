/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abidaux <abidaux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 16:46:13 by abidaux           #+#    #+#             */
/*   Updated: 2025/03/26 16:46:14 by abidaux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Gère les caractères normaux
static char	*handle_normal_char(const char *input, int *i)
{
	char	*temp;

	temp = malloc(2);
	if (!temp)
		return (NULL);
	temp[0] = input[*i];
	temp[1] = '\0';
	(*i)++;
	return (temp);
}

// Ajoute la partie parsée à `word`
static int	append_to_word(char **word, char *temp)
{
	char	*new_word;

	if (!temp)
		return (-1);
	new_word = ft_strjoin_free(*word, temp);
	if (!new_word)
	{
		free(temp);
		return (-1);
	}
	*word = new_word;
	return (0);
}

// Traite le caractère actuel et met à jour `word`
int	process_current_char(const char *input, int *i,
	char **word, t_state *state)
{
	char	*temp;

	if (input[*i] == '\'' || input[*i] == '"')
	{
		temp = handle_quotes(input, i, state->envp, state);
		if (!temp)
			return (-1);
	}
	else if (input[*i] == '$' && should_parse_as_special(input, *i))
	{
		temp = handle_variable(input, i, state);
		if (!temp)
			return (-1);
	}
	else
	{
		temp = handle_normal_char(input, i);
		if (!temp)
			return (-1);
	}
	if (append_to_word(word, temp) == -1)
		return (free(temp), -1);
	return (0);
}

char	*parse_single_quote(const char *input, int *i)
{
	char	*result;
	int		start;
	int		len;

	(*i)++;
	start = *i;
	while (input[*i] && input[*i] != '\'')
		(*i)++;
	len = *i - start;
	result = ft_substr(input, start, len);
	if (input[*i] == '\'')
		(*i)++;
	return (result);
}

char	*parse_double_quote(const char *input, int *i,
	char **envp, t_state *state)
{
	char	*result;
	char	*temp;
	int		start;

	(*i)++;
	(void)envp;
	result = ft_strdup("");
	if (!result)
		return (NULL);
	while (input[*i] && input[*i] != '"')
	{
		start = *i;
		while (input[*i] && input[*i] != '"' && input[*i] != '$')
			(*i)++;
		temp = ft_substr(input, start, *i - start);
		result = ft_strjoin_free(result, temp);
		if (input[*i] == '$')
		{
			temp = expand_variable((char *)input, i, state);
			result = ft_strjoin_free(result, temp);
		}
	}
	if (input[*i] == '"')
		(*i)++;
	return (result);
}
