

// HEADER

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
