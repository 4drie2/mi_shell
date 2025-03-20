#include "../minishell.h"

static t_redir	*create_redir(t_redir_type type, const char *str)
{
	t_redir	*r;

	r = malloc(sizeof(t_redir));
	if (!r)
		return (NULL);
	r->type = type;
	r->target = ft_strdup(str);
	if (!r->target)
	{
		free(r);
		return (NULL);
	}
	return (r);
}

static t_redir	**realloc_redir_array(t_redir **redir_array,
	int current_size, t_redir *new_redir)
{
	t_redir	**new_tab;
	int		i;

	new_tab = malloc(sizeof(t_redir *) * (current_size + 2));
	if (!new_tab)
		return (NULL);
	i = 0;
	while (i < current_size)
	{
		new_tab[i] = redir_array[i];
		i++;
	}
	new_tab[i] = new_redir;
	new_tab[i + 1] = NULL;
	free(redir_array);
	return (new_tab);
}

static int	add_redirection(t_redir ***redir_array,
	int *redir_count, t_redir_type type, const char *content)
{
	t_redir	*redir;
	t_redir	**tmp;

	redir = create_redir(type, content);
	if (!redir)
		return (-1);
	tmp = realloc_redir_array(*redir_array, *redir_count, redir);
	if (!tmp)
	{
		free(redir->target);
		free(redir);
		return (-1);
	}
	*redir_array = tmp;
	(*redir_count)++;
	return (0);
}

int	add_in_redir(t_command *cmd, t_redir_type type, const char *content)
{
	return (add_redirection(&cmd->in, &cmd->nb_in, type, content));
}

int	add_out_redir(t_command *cmd, t_redir_type type, const char *content)
{
	return (add_redirection(&cmd->out, &cmd->nb_out, type, content));
}
