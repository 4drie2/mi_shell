

// HEADER 42

#include "../minishell.h"

static char *get_executable_path(t_command *cmd, t_state *state)
{
	char	*path;

	if (cmd->args[0][0] == '/' || (cmd->args[0][0] == '.'
		&& cmd->args[0][1] == '/'))
		path = valide_path(cmd->args[0]);
	else
		path = get_command_path(cmd->args[0], state);
	if (!path)
	{
		print_err_msg(cmd->args[0], ":cmd not found", "\n");
		while (cmd->prev)
			cmd = cmd->prev;
		free_command_list(cmd);
		free_envp(state->envp);
		state->last_exit_status = 127;
		exit (127);
	}
	return (path);
}


/* WHAT A FUCKING 🌟beautyfull🌟 FONCTION */
static void	execute_cmd(char *path, char **args, char **envp)
{
	if (execve(path, args, envp) == -1)
	{
		perror(args[0]);
		free(envp);
		free(path);
		exit(126);
	}
}
