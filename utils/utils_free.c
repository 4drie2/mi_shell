#include "../minishell.h"

/*
** free_command_list:
**   Parcourt la liste chaînée de t_command et libère chaque noeud.
*/
void	free_command_list(t_command *cmd)
{
	t_command	*temp;

	while (cmd)
	{
		temp = cmd;
		cmd = cmd->next;
		free_command_list(temp);
	}
}