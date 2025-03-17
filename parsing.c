/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abidaux <abidaux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 21:09:16 by abidaux           #+#    #+#             */
/*   Updated: 2025/03/17 21:28:28 by abidaux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parsing(char *input)
{
	t_token	*token;
	int		i;

	token = malloc(sizeof(t_token));
	i = -1;
	while (input[++i])
	{
		if (input[i] == '<')
			token->type = 1;
		else if (input[i] == '>')
			token->type = 3;
		else if (input[i] == '|')
			token->type = 5;
		else if (input[i] == ' ')
			token->next = malloc(sizeof(t_token));
		else
			printf("CMD\n");
	}
}
