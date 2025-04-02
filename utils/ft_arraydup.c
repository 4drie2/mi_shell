/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arraydup.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthuilli <pthuilli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 15:15:21 by pthuilli          #+#    #+#             */
/*   Updated: 2025/04/01 15:16:23 by pthuilli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**ft_arraydup(char **array)
{
	char	**new_array;
	int		size;

	if (!array)
		return (NULL);
	size = 0;
	while (array[size])
		size++;
	new_array = (char **)malloc(sizeof(char *) * (size + 1));
	if (!new_array)
		return (NULL);
	size = 0;
	while (array[size])
	{
		new_array[size] = ft_strdup(array[size]);
		if (!new_array[size])
		{
			while (size > 0)
				free(new_array[--size]);
			return (free(new_array), NULL);
		}
		size++;
	}
	new_array[size] = NULL;
	return (new_array);
}