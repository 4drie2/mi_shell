

/// HEADER MES COUILLES

#include "../minishell.h"

int	exec_if_builtin(t_command *cmd, t_state *st)
{
	if (!cmd->args[0] || !is_builtin(cmd->args[0]))
		return (0);
	if (handle_all_heredocs(cmd, st) < 0)
	{
		restore_and_close_fds(st);
		return (1);
	}
	signal(SIGINT, child_cleanup_and_exit);
	if (handle_all_redirections(cmd, st) < 0)
	{
		restore_and_close_fds(st);
		return (1);
	}
	if (exec_builtins(cmd, st) != 2)
		if (!ft_strcmp(cmd->args[0], "."))
			st->last_exit_status = 127;
	restore_and_close_fds(st);
	reset_signals_after_execution();
	return (1);
}

int	check_if_empty_command(t_command *cmd, t_state *st)
{
	if (cmd->args[0][0] == '\0')
	{
		ft_putstr_fd(": command not found\n", 2);
		restore_and_close_fds(st);
		return (1);
	}
	return (0);
}
