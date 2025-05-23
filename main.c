/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthuilli <pthuilli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:02:40 by abidaux           #+#    #+#             */
/*   Updated: 2025/04/07 11:46:54 by pthuilli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * ---------- TO DO LIST ----------
 * - Mauvais exit code (signaux)
 * - Mauvais free (en cas de d'execVE qui foire)
 * - "." et "./" (. | ./) (leak)
 * - (pipes par tout en meme temps mais un par 
 * un entre chaque commandes)
 * - changer nom de certains malloc pour free(malloc) 
 * ou free(palestine)..
 * - fusionner convert_tokens_to_commands 
 * et convert_tokens_to_command
 * - S'occuper dereset_signals_after_execution 
 * (a integrer dans les fonctions)
 * - validate_input(char *input) a integrer dans les fonctions
 * - Mettre le bon exit code qd exit (ctr+c ou exit)
 *
 * ---------- ASTUCES -------------
 * voir ce que fais le shell: strace -e write bash
 */

#include "minishell.h"

/* free envp et toutes ses sous chaines. */
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

	if (is_empty_or_space(input))
		return (0);
	cmd = parse_input(input, state);
	if (!cmd)
		return (1);
	exec_shell(cmd, state);
	free_command_list(cmd);
	return (0);
}

/**
 * Initialise la structure d'état du shell.
 *
 * Configure l'environnement, incrémente SHLVL, et initialise les autres
 * champs de la structure d'état.
 *
 * @param state Pointeur vers la structure d'état à initialiser
 * @param envp Environnement du processus parent
 * @return EXIT_SUCCESS en cas de réussite, EXIT_FAILURE sinon
 */
static int	init_state(t_state *state, char **envp)
{
	char	*shlvl;
	char	*value;

	state->envp = copy_environment(envp);
	if (!state->envp)
		return (EXIT_FAILURE);
	shlvl = search_in_local_env(state->envp, "SHLVL");
	value = ft_itoa(ft_atoi(shlvl) + 1);
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
		return (free(value), free_envp(state->envp), EXIT_FAILURE);
	}
	return (free(value), EXIT_SUCCESS);
}

void	display_prompt(t_state *state)
{
	char	*input;

	while (1)
	{
		input = readline("our shell ☭ ");
		add_history(input);
		if (!input)
		{
			ft_putstr_fd("exit\n", 2);
			break ;
		}
		reset_state(state);
		if (*input)
			handle_user_input(input, state);
		free(input);
	}
}

int	main(int ac, char **av, char **envp)
{
	t_state	state;

	(void) ac, (void) av;
	if (init_state(&state, envp) == 1)
		return (1);
	setup_signals();
	display_prompt(&state);
	rl_clear_history();
	free_envp(state.envp);
	return (state.last_exit_status);
}
