/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthuilli <pthuilli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:02:40 by abidaux           #+#    #+#             */
/*   Updated: 2025/03/19 20:29:06 by pthuilli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_envp(char **envp)
{
	int	i;

	i = -1;
	if (!envp)
		return ;
	while (envp[++i])
		free(envp[i]);
	free(envp);
}

/*write exit quand exit : strace -e write bash*/
int	handle_user_input(char *input, t_state *state)
{
	t_command	*cmd;

	if (is_empty_or_space(input))
		return (0);
	cmd = parse_input(input, state);
	if (!cmd)
		return (1);
	exec_shell(cmd, state);
	free_command_list(cmd);
	return (0);
}

static int	init_state(t_state *state, char **envp)
{
	char	*shlvl;
	char	*value;
	int		i;

	state->envp = copy_environment(envp); // a faire
	if (!state->envp)
		return (EXIT_FAILURE);
	shlvl = search_in_local_env(state->envp, "SHLVL"); // a faire
	i = ft_atoi(shlvl) + 1;
	value = ft_itoa(i);
	state->envp = set_env_var(state->envp, "SHLVL", value); // a faire
	state->last_exit_status = 0;
	state->num_pipes = 0;
	state->index = 0;
	state->pids = NULL;
	state->pipes = NULL;
	state->o_stdout = -1;
	state->o_stdin = -1;
	if (getcwd(state->current_path, sizeof(state->current_path)) == NULL)
	{
		perror("Initialization: getcwd error");
		ft_strlcpy(state->current_path, "", 1);
		return (free(value), free_envp(state->envp), EXIT_FAILURE);
	}
	return (free(value), EXIT_SUCCESS);
}

static void	display_prompt(t_state *state)
{
	char *input;

	while(1)
	{
		char	*scrap_input;

		input = readline("\001\033[1;32m\002Mi-Shell $ \001\033[0m\002");
		add_history(input);
		scrap_input = ft_strtrim(input, " \t\r\b"); // added
		if (!scrap_input)
		{
			ft_putstr_fd("exit\n", 2); // 1->2
			break;
		}
		if (*scrap_input)
			handle_user_input(scrap_input, state);
		free(input);
		free(scrap_input);
	}
}

int main(int ac, char **av, char **envp)
{
	t_state	shell;

	(void) ac, (void) av;
	if (init_state(&shell, envp) == 1)
	{
		return(1);
	}
	display_prompt(&shell);
	free_envp(shell.envp);
}
