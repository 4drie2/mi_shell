/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthuilli <pthuilli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 11:07:12 by pthuilli          #+#    #+#             */
/*   Updated: 2025/04/01 11:10:37 by pthuilli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	has_trailing_space(const char *str)
{
	size_t	len;

	len = ft_strlen(str);
	if (len == 0)
		return (0);
	if (ft_isspace(str[len - 1]))
		return (1);
	return (0);
}

static char	*allocate_normalized(const char *str, int has_lead)
{
	char	*result;

	result = malloc(ft_strlen(str) + 3);
	if (!result)
		return (NULL);
	ft_memset(result, 0, ft_strlen(str) + 3);
	if (has_lead)
		result[0] = ' ';
	return (result);
}

static void	copy_loop(const char *str, char *result, int *j)
{
	int	i;
	int	in_word;

	i = 0;
	in_word = 0;
	while (str[i])
	{
		if (ft_isspace(str[i]))
		{
			if (in_word)
			{
				result[(*j)++] = ' ';
				in_word = 0;
			}
		}
		else
		{
			result[(*j)++] = str[i];
			in_word = 1;
		}
		i++;
	}
}

static void	copy_normalized(const char *str,
	char *result, int has_lead, int has_trail)
{
	int	j;

	j = 0;
	if (has_lead)
	{
		result[0] = ' ';
		j = 1;
	}
	copy_loop(str, result, &j);
	if (has_trail && j > 0 && result[j - 1] != ' ')
		result[j++] = ' ';
	result[j] = '\0';
}

char	*normalize_spaces(const char *str)
{
	char	*result;
	int		has_lead;
	int		has_trail;

	if (!str)
		return (NULL);
	has_lead = has_leading_space(str);
	has_trail = has_trailing_space(str);
	result = allocate_normalized(str, has_lead);
	if (!result)
		return (NULL);
	copy_normalized(str, result, has_lead, has_trail);
	return (result);
}
