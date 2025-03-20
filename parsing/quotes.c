

// HEADER

#include "../minishell.h"

int	check_unclosed_quotes(const char *input)
{
	int		i;
	char	quote;

	i = -1;
	while (input[++i])
	{
		if (input[i] == '\'' || input[i] == '\"')
		{
			quote = input[i++];
			while (input[i] && input[i] != quote)
				++i;
			if (!input[i])
				return (1);
		}
	}
	return (0);
}

static char *handle_escaped_quote(int *i, char *result)
{
	result = ft_strjoin_free(result, "\"");
	(*i) += 2;
	return (result);
}

static char *handle_variable_expension(const char *input, int *i,
	t_state *state, char *result)
{
	char	*temp;

	(*i)++;
	if (input[*i] == '?' || input[*i] == '$')
	{
		temp = handle_special_var(input[*i], state);
		(*i)++;
	}
	else
		temp = expand_variable((char *)input, i, state);
	result = ft_strjoin_free(result, temp);
	free(temp);
	return (result);
}

static char *handle_regular_text(const char *input, int *i, char *result)
{
	char	*temp;
	int		start;

	start = *i;
	while (input[*i] && input[*i] != '"' && input[*i] != '$'
		&& !(input[*i] == '\\' && input[*i + 1] == '"'))
		(*i)++;
	temp = ft_substr(input, start, *i - start);
	result = ft_strjoin_free(result, temp);
	free(temp);
	return (result);
}

char	*handle_double_quotes(const char *input, int *i, t_state *state)
{
	char	*result;

	result = ft_strdup("");
	(*i)++;
	while (input[*i] && input[*i] != '"')
	{
		if (input[*i] == '\\' && input[*i + 1] == '"')
			result = handle_escaped_quote(i, result);
		else if (input[*i] == '$')
			result = handle_variable_expension(input, i, state, result);
		else
			result = handle_regular_text(input, i, result);
	}
	if (input[*i] == '"')
		(*i)++;
	return (result);
}