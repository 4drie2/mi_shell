/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthuilli <pthuilli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 16:46:05 by abidaux           #+#    #+#             */
/*   Updated: 2025/04/02 07:52:36 by pthuilli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_command_incomplete(t_token *tokens, t_state *state)
{
	t_token	*last;

	if (!tokens)
		return (0);
	last = tokens;
	while (last->next)
		last = last->next;
	if (!ft_strcmp(last->content, "|") || !ft_strcmp(last->content, "<")
		|| !ft_strcmp(last->content, "<<") || !ft_strcmp(last->content, ">")
		|| !ft_strcmp(last->content, ">>"))
		return (1);
	if ((!ft_strcmp(last->content, "<")
			&& last->prev && !ft_strcmp(last->prev->content, "<"))
		|| (!ft_strcmp(last->content, ">")
			&& last->prev && !ft_strcmp(last->prev->content, ">")))
	{
		ft_putstr_fd("syntax error near unexpected token `<'\n", 2);
		state->last_exit_status = 2;
		return (1);
	}
	return (0);
}

/**
 * Traite un token de pipe dans la syntaxe de commande.
 * Configure la commande courante comme ayant un pipe, initialise la commande suivante,
 * et met à jour les pointeurs et index.
 *
 * @param cur_cmd Pointeur vers le pointeur de la commande courante
 * @param cur Pointeur vers le pointeur du token courant
 * @param arg_idx Pointeur vers l'index d'argument courant
 * @param state Pointeur vers l'état global du shell
 * @return 0 si succès, 2 pour erreur de syntaxe, -1 pour erreur d'allocation
 */
static int	process_pipe_token(t_command **cur_cmd,
	t_token **cur, int *arg_idx, t_state *state)
{
	if (ft_strcmp((*cur)->content, "|") != 0)
		return (0);
	if (!(*cur)->next || !(*cur)->next->content)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token '|'\n", 2);
		state->last_exit_status = 2;
		return (2);
	}
	(*cur_cmd)->is_pipe = 1;
	(*cur_cmd)->args[*arg_idx] = NULL;
	(*cur_cmd)->next = init_command();
	if (!(*cur_cmd)->next)
	{
		ft_putstr_fd("minishell: malloc error\n", 2);
		free(*cur_cmd);
		return (-1);
	}
	(*cur_cmd)->next->prev = *cur_cmd;
	*cur_cmd = (*cur_cmd)->next;
	*arg_idx = 0;
	(*cur_cmd)->args[0] = NULL;
	(*cur_cmd)->nb_in = 0;
	(*cur_cmd)->nb_out = 0;
	*cur = (*cur)->next;
	return (0);
}

static int	process_redir_token(t_command *cur_cmd,
	t_token **cur, t_state *state)
{
	t_token	*redir_symb;
	t_token	*file_tok;
	int		ret;

	redir_symb = *cur;
	file_tok = redir_symb->next;
	if (is_file_tok(file_tok, state) != 0)
		return (2);
	if (ft_strcmp(redir_symb->content, "<") == 0)
		ret = add_in_redir(cur_cmd, R_INPUT, file_tok->content);
	else if (ft_strcmp(redir_symb->content, "<<") == 0)
		ret = add_in_redir(cur_cmd, R_HEREDOC, file_tok->content);
	else if (ft_strcmp(redir_symb->content, ">") == 0)
		ret = add_out_redir(cur_cmd, R_OUTPUT, file_tok->content);
	else
		ret = add_out_redir(cur_cmd, R_APPEND, file_tok->content);
	if (ret < 0)
		return (-1);
	if (file_tok->next != NULL)
		*cur = file_tok->next;
	else
		*cur = NULL;
	return (0);
}

static int	process_arg_token(t_command *cur_cmd, t_token **cur, int *arg_idx)
{
	char	*dup;

	if (*arg_idx < 255)
	{
		dup = ft_strdup((*cur)->content);
		if (!dup)
			return (-1);
		cur_cmd->args[*arg_idx] = dup;
		*arg_idx = *arg_idx + 1;
		cur_cmd->args[*arg_idx] = NULL;
	}
	*cur = (*cur)->next;
	return (0);
}

int	process_tokenizer(t_command **curr,
	t_token **cur, int *arg_idx, t_state *state)
{
	int	ret;

	if (ft_strcmp((*cur)->content, "|") == 0)
	{
		ret = process_pipe_token(curr, cur, arg_idx, state);
		if (ret != 0)
			return (ret);
		return (0);
	}
	if (is_redirection((*cur)->content))
	{
		ret = process_redir_token(*curr, cur, state);
		if (ret != 0)
			return (ret);
		return (0);
	}
	ret = process_arg_token(*curr, cur, arg_idx);
	if (ret < 0)
		return (-1);
	return (0);
}
