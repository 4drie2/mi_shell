/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthuilli <pthuilli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 16:44:30 by abidaux           #+#    #+#             */
/*   Updated: 2025/04/01 11:09:57 by pthuilli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	has_equals_as(char *str, char *ui)
{
	int	j;

	j = 0;
	while (str[j])
	{
		if (str[j] != *ui)
			return (0);
		j++;
	}
	return (1);
}

static int	has_equals_sign(const char *str)
{
	int	j;

	j = 0;
	while (str[j])
	{
		if (str[j] == '=')
			return (1);
		j++;
	}
	return (0);
}

void	handle_env_command(char **envp, t_state *state)
{
	int	i;

	i = 0;
	if (!envp)
	{
		ft_putstr_fd("env: No environment variables found.\n", 2);
		state->last_exit_status = 1;
		return ;
	}
	while (envp[i])
	{
		if (has_equals_sign(envp[i]))
			printf("%s\n", envp[i]);
		i++;
	}
	state->last_exit_status = 0;
}

char	*join_key_value(const char *key, const char *value)
{
	char	*temp;
	char	*result;

	if (!key || !value)
	{
		perror("Invalid key or value for environment variable");
		return (NULL);
	}
	temp = ft_strjoin(key, "=");
	if (!temp)
	{
		perror("Failed to allocate memory for key=value");
		free(temp);
		return (NULL);
	}
	result = ft_strjoin(temp, value);
	free(temp);
	if (!result)
		perror("Failed to allocate memory for key=value");
	return (result);
}

void	replace_env_entry(char **envp, int index, const char *key,
	const char *value)
{
	char	*new_entry;

	new_entry = join_key_value(key, value);
	if (!new_entry)
	{
		perror("Error updating environment variable");
		exit(EXIT_FAILURE);
	}
	free(envp[index]);
	envp[index] = new_entry;
}
