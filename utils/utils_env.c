#include "../minishell.h"

static int	var_exists(char **envp, const char *var)
{
	int	i;
	int	len;

	len = 0;
	while (var[len] && var[len] != '=')
		len++;
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var, len) == 0 && envp[i][len] == '=')
			return (i);
		i++;
	}
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

char	**copy_environment(char **envp)
{
	int		i;
	char	**copy;

	i = 0;
	while (envp[i])
		i++;
	copy = malloc(sizeof(char *) * (i + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		copy[i] = ft_strdup(envp[i]);
		if (!copy[i])
		{
			while (--i >= 0)
				free(copy[i]);
			free(copy);
			return (NULL);
		}
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

char	**realloc_tab(char **envp, char *new_entry)
{
	int		count;
	int		i;
	char	**new_envp;

	count = 0;
	while (envp[count])
		count++;
	new_envp = malloc((count + 2) * sizeof(char *));
	if (!new_envp)
	{
		perror("Error reallocating environment variable array");
		return (NULL);
	}
	i = -1;
	while (++i < count)
		new_envp[i] = envp[i];
	new_envp[count] = new_entry;
	new_envp[count + 1] = NULL;
	free(envp);
	return (new_envp);
}

char	*search_in_local_env(char **envp, const char *key)
{
	int		i;
	size_t	len;

	i = 0;
	len = ft_strlen(key);
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], key, len) && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return (NULL);
}
