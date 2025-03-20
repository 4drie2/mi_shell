#include "../minishell.h"

// Fonction pour ajouter un token dans la liste chaînée
t_token	*create_and_add_token(char *content, t_token **head)
{
	t_token	*new_token;
	t_token	*tmp;

	if (!content)
		return (NULL);
	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->content = ft_strdup(content);
	if (!new_token->content)
		return (free(new_token), NULL);
	new_token->next = NULL;
	new_token->prev = NULL;
	if (!*head)
		*head = new_token;
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_token;
		new_token->prev = tmp;
	}
	return (new_token);
}

