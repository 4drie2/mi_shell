/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strtol2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthuilli <pthuilli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 10:11:54 by abidaux           #+#    #+#             */
/*   Updated: 2025/04/07 11:49:33 by pthuilli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	skip_whitespace_and_sign(const char **str, int *sign)
{
	while (**str && ft_isspace(**str))
		(*str)++;
	if (**str == '+' || **str == '-')
	{
		if (**str == '-')
			*sign = -1;
		(*str)++;
	}
}

long	ft_strtol(const char *str, char **endptr, int base)
{
	int	sign ;

	sign = 1;
	if (!validate_base(base, &str, endptr))
		return (0);
	skip_whitespace_and_sign(&str, &sign);
	base = determine_base(&str, base);
	return (handle_conversion(&str, base, sign, endptr));
}
