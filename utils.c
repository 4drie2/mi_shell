




#include "minishell.h"

int	

int	is_empty_or_space(const char *str)
{
	int	i;

	i = -1;
	while (str[++i])
		if (!ft_isspace(str[i]))
			return (0);
	return (1);
}
