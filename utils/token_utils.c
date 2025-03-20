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

t_token	*tokenize_input(const char *input, t_state *state)
{
	t_token	*token_list;

	token_list = NULL;

	if (check_unclosed_quotes(input))
	{
		ft_putstr_fd("minishell: syntax error: unclosed quotes\n", 2);
		state->last_exit_status = 2;
		return (NULL);
	}
	if (process_tokens(input, &token_list, state) == -1)
	{
		free_tokens(token_list);
		return (NULL);
	}
	return (token_list);
}
