#include "../minishell.h"

char	*handle_special_var(char c, t_state *state)
{
	char	*exit_str;

	if (c == '?')
	{
		exit_str = ft_itoa(state->last_exit_status);
		if (!exit_str)
			return (ft_strdup("0"));
		return (exit_str);
	}
	return (NULL);
}

static char	*expand_special_variable(const char *input, int *i,
	char **envp, t_state *state)
{
	char	*value;

	(void)envp;
	if (input[*i] == '$')
	{
		(*i)++;
		if (!input[*i] || ft_isspace(input[*i])
			|| input[*i] == '"' || input[*i] == '\'')
			return (ft_strdup("$"));
	}
	if (input[*i] == '?')
	{
		(*i)++;
		value = handle_special_var('?', state);
		if (value)
			return (value);
		return (ft_strdup("0"));
	}
	return (NULL);
}

char	*expand_variable(char *input, int *i, t_state *state)
{
	char	*result;
	char	*key;
	char	*value;
	int		start;

	result = expand_special_variable(input, i, state->envp, state);
	if (result)
		return (result);
	start = *i;
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		(*i)++;
	key = ft_substr(input, start, *i - start);
	if (!key)
		return (NULL);
	value = search_in_local_env(state->envp, key);
	free(key);
	if (!value)
		return (ft_strdup(""));
	return (ft_strdup(value));
}

void	debug_tokens(t_token *tokens)
{
	printf("Tokens: ");
	while (tokens)
	{
		printf("[%s] -> ", tokens->content);
		tokens = tokens->next;
	}
	printf("NULL\n");
}

void	append_tokens(t_token **current, t_token *new_tokens)
{
	t_token	*last;

	if (!current || !new_tokens)
		return ;
	last = *current;
	while (last && last->next)
		last = last->next;
	if (!last)
	{
		*current = new_tokens;
		return ;
	}
	last->next = new_tokens;
	new_tokens->prev = last;
}
