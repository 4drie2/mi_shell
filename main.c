/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abidaux <abidaux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:02:40 by abidaux           #+#    #+#             */
/*   Updated: 2025/03/17 19:03:38 by abidaux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(void)
{
	char	*pre_input;
	char	*input;
	bool	keep_running;

	keep_running = true;
	while (keep_running)
	{
		pre_input = readline("my_shell >");
		input = ft_strtrim(pre_input, " \t\n\v\f\r");
		if (!input || !ft_strncmp(input, "exit", 5))
			keep_running=false;
		else
			printf("|%s|\n", input); // rest of the code goes here
		add_history(pre_input);
		free(pre_input);
		free(input);
	}
	rl_clear_history();
	return 0;
}
