#include "../../minishell.h"

char	*join_key_value(const char *key, const char *value)
{
	char	*temp;
	char	*result;

	if (!key || !value)
	{
		perror("Invalid key or value for environment variable");
		return (NULL);
	}
	temp = ft_strjoin(key, "=");
	if (!temp)
	{
		perror("Failed to allocate memory for key=value");
		free(temp);
		return (NULL);
	}
	result = ft_strjoin(temp, value);
	free(temp);
	if (!result)
		perror("Failed to allocate memory for key=value");
	return (result);
}