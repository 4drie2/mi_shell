#include "../minishell.h"

void	print_err_msg(char *cmd, char *path, char *msg)
{
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(msg, 2);
}

/*
** Vérifie si le premier token est un pipe.
** Retourne 1 si une erreur est détectée, 0 sinon.
*/
int	check_initial_pipe(t_token *current, t_state *state)
{
	if (current && ft_strcmp(current->content, "|") == 0)
	{
		ft_putstr_fd("syntax error near unexpected token `|'\n", 2);
		state->last_exit_status = 2;
		return (1);
	}
	return (0);
}

/*
** Vérifie les erreurs de pipe dans la séquence de tokens.
** Retourne 1 si une erreur est détectée, 0 sinon.
*/
int	check_pipes(t_token *current, t_state *state)
{
	if (ft_strcmp(current->content, "|") == 0)
	{
		if (!current->next || ft_strcmp(current->next->content, "|") == 0)
		{
			ft_putstr_fd("syntax error near unexpected token `|'\n", 2);
			state->last_exit_status = 2;
			return (1);
		}
	}
	return (0);
}

/*
** Vérifie les erreurs liées aux redirections.
** Retourne 1 si une erreur est détectée, 0 sinon.
*/
int	check_redirections(t_token *current, t_state *state)
{
	if (is_redirection(current->content))
	{
		if (!current->next)
		{
			ft_putstr_fd("syntax error near unexpected token `newline'\n", 2);
			state->last_exit_status = 2;
			return (1);
		}
	}
	return (0);
}

/*
** Vérifie les tokens invalides tels que ">>>".
** Retourne 1 si une erreur est détectée, 0 sinon.
*/
int	check_invalid_tokens(t_token *current, t_state *state)
{
	if (ft_strcmp(current->content, ">>>") == 0
		|| ft_strcmp(current->content, "<<<") == 0
		|| ft_strcmp(current->content, ">>|") == 0
		|| ft_strcmp(current->content, "<<|") == 0)
	{
		print_err_msg("syntax error near unexpected token `",
			current->content, "'\n");
		state->last_exit_status = 2;
		return (1);
	}
	return (0);
}