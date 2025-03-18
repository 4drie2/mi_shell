/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthuilli <pthuilli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:02:40 by abidaux           #+#    #+#             */
/*   Updated: 2025/03/18 17:48:27 by pthuilli         ###   ########.fr       */
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

int	handle_user_input(char *input, t_state *state)
{
	t_command	*cmd;

	if (!input || ft_strncmp(input, "exit", 4) == 0)
	{
		ft_putstr_fd("exit\n", 1);
		free_envp(state->envp);
		free(input);
		exit(EXIT_SUCCESS);
	}
}

/*

- Add History va etre dans une fonction Handle User Input.
- Faire l'init pour les PIDS, PIPE etc
*/


static int	init_state(t_state *state, char **envp)
{
	char	*shlvl;
	char	*value;
	int		i;

	state->envp = copy_environment(envp);
	if (!state->envp)
		return (EXIT_FAILURE);
	shlvl = search_in_local_env(state->envp, "SHLVL");
	i = ft_atoi(shlvl) + 1;
	value = ft_itoa(i);
	state->envp = set_env_var(state->envp, "SHLVL", value);
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
		return (free(value), free_environment(state->envp), EXIT_FAILURE);
	}
	return (free(value), EXIT_SUCCESS);
}



static void	display_prompt(void)
{
	char *input;

	while(1)
	{
		input = readline("\001\033[1;32m\002Mi-Shell $ \001\033[0m\002");
		if (!input)
		{
			ft_putstr_fd("exit\n", 1);
			break;
		}
		if (*input)
		{
			// faire la fonctione handle user input avec le add history
		}
	}
}


int main(int argc, char **argv, char **envp)
{
	t_state shell;

	(void) argc, (void) argv;
	if (init_state(&shell, envp) == 1)
	{
		return(1);
	}
	display_prompt();
}


// int main(int argc, char **argv, char **envp)
// {
// 	(void) argc, (void) argv;
// 	char	*pre_input;
// 	char	*input;
// 	bool	keep_running;

// 	keep_running = true;
// 	while (keep_running)
// 	{
// 		pre_input = readline("my_shell >");
// 		input = ft_strtrim(pre_input, " \t\n\v\f\r");
// 		if (!input || !ft_strncmp(input, "exit", 5))
// 			keep_running=false;
// 		else
// 			printf("input: %s\n", input); // next step: parsing(input);
// 		add_history(pre_input);
// 		free(pre_input);
// 		free(input);
// 	}
// 	rl_clear_history();
// 	return 0;
// }


// /*
// ** main: init l'état, setup_signals, lance la boucle du prompt
// */
// int	main(int argc, char **argv, char **envp)
// {
// 	t_state	shell_state;

// 	(void) argc, (void) argv;
// 	if (init_state(&shell_state, envp) == EXIT_FAILURE) // permet de preparer la variable d'env, le numero de pipe,
// 	{
// 		return (EXIT_FAILURE);
// 	}
// 	setup_signals(); // permet de gerer les signaux
// 	display_prompt(&shell_state); // le readline ici.
// 	rl_clear_history(); // free l'historique de readline
// 	free_environment(shell_state.envp); // permet de free l'envp
// 	return (shell_state.last_exit_status);
// }
