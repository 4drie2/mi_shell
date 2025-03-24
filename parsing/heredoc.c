

// HEADER 42


#include "../minishell.h"

char	*generate_tmp_file(void)
{
	static int	counter = 0;
	char		*filename;
	char		*pid_str;
	char		*count_str;
	char		*temp;

	pid_str = ft_itoa((int)getpid());
	if (!pid_str)
		return (NULL);
	count_str = ft_itoa(counter++);
	if (!count_str)
		return (free(pid_str), NULL);
	temp = ft_strjoin(pid_str, "_");
	if (!temp)
		return (free(count_str), NULL);
	filename = ft_strjoin("/tmp/heredoc_", temp);
	free(temp);
	free(count_str);
	if (!filename)
		perror("Error generating temporary filename");
	return (filename);
}
