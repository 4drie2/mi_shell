/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthuilli <pthuilli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 16:46:01 by abidaux           #+#    #+#             */
/*   Updated: 2025/04/02 07:52:38 by pthuilli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	convert_tokens_to_command(t_command *cmd, t_token *tokens, t_state *state)
{
	t_command	*curr;
	t_token		*cur;
	int			arg_idx;
	int			ret;

	curr = cmd;
	cur = tokens;
	arg_idx = 0;
	curr->args[0] = NULL;
	while (cur)
	{
		ret = process_tokenizer(&curr, &cur, &arg_idx, state);
		if (ret != 0)
			return (ret);
	}
	curr->args[arg_idx] = NULL;
	return (0);
}

// Vérifie si le contenu est une redirection
int	is_redirection(const char *content)
{
	if (!content)
		return (0);
	return (ft_strcmp(content, "<") == 0
		|| ft_strcmp(content, "<<") == 0
		|| ft_strcmp(content, ">") == 0
		|| ft_strcmp(content, ">>") == 0);
}

int	is_file_tok(t_token *file_tok, t_state *state)
{
	if (!file_tok || !file_tok->content)
	{
		ft_putstr_fd(
			"minishell: syntax error near unexpected token 'newline'\n", 2);
		state->last_exit_status = 2;
		return (2);
	}
	return (0);
}

int	check_redirections_conflicts(t_token *current, t_state *state)
{
	while (current)
	{
		if (is_redirection(current->content))
		{
			if (!current->next || ft_strcmp(current->next->content, "|") == 0)
			{
				ft_putstr_fd("syntax error near unexpected token `", 2);
				if (current->next)
					ft_putstr_fd(current->next->content, 2);
				else
					ft_putstr_fd("newline", 2);
				ft_putstr_fd("'\n", 2);
				state->last_exit_status = 2;
				return (1);
			}
			if (current->next && is_redirection(current->next->content))
			{
				print_err_msg("syntax error near unexpected token `",
					current->next->content, "'\n");
				return (state->last_exit_status = 2, 1);
			}
		}
		current = current->next;
	}
	return (0);
}

/*
** Vérifie les erreurs de syntaxe dans la liste de tokens.
** Retourne 1 si une erreur est détectée, 0 sinon.
*/
int	check_syntax_errors(t_token *tokens, t_state *state)
{
	t_token	*current;

	current = tokens;
	if (check_initial_pipe(current, state))
		return (1);
	while (current)
	{
		if (check_pipes(current, state))
			return (1);
		if (check_redirections(current, state))
			return (1);
		if (check_invalid_tokens(current, state))
			return (1);
		if (check_redirections_conflicts(tokens, state))
			return (1);
		current = current->next;
	}
	return (0);
}
