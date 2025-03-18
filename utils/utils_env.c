


#include "../minishell.h"

static int	var_exists(char **envp, const char *var)
{
	int	i;
	int	len;

	len = 0;
	while (var[len] && var[len] != "=")
		len++;
	i = -1;
	while (envp[++i])
		if (ft_strncmp(envp[i], var, len) == 0 && envp[i][len] == "=")
			return (i);
	return (-1);
}

char	**set_env_var(char **envp, const char *key, const char *value)
{
	char	**new_envp;
	char	*new_entry;
	int		pos;

	if (!key || !envp)
		return (envp);
	new_entry = join_key_value(key, value);
	if (!new_entry)
		return (envp);
	pos = var_exists(envp, key);
	if (pos >= 0)
	{
		free(envp[pos]);
		envp[pos] = new_entry;
	}
	else
	{
		new_envp = realloc_tab(envp, new_entry);
		if (!new_envp)
			return (free(new_entry), envp);
		envp = new_envp;
	}
	return (envp);
}
