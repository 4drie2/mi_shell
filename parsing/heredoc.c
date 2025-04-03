/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abidaux <abidaux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 16:45:13 by abidaux           #+#    #+#             */
/*   Updated: 2025/04/03 10:00:09 by abidaux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * Génère un nom de fichier temporaire unique pour stocker
 * le contenu d'un heredoc.
 * Format: /tmp/heredoc_[PID]_[COMPTEUR]
 * Le compteur s'incrémente à chaque appel pour garantir l'unicité.
 *
 * @return Chaîne allouée contenant le chemin du fichier temporaire,
 *         ou NULL en cas d'échec d'allocation.
 */
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

/**
 * Écrit dans un fichier temporaire les entrées de l'utilisateur
 * jusqu'au délimiteur.
 * Chaque ligne est terminée par un retour à la ligne.
 *
 * @param fd      Descripteur du fichier cible
 * @param limiter Délimiteur qui arrête la saisie
 * @return        0 si succès, -1 si erreur
 */
int	write_to_tmp_file(int fd, const char *limiter)
{
	char	*line;

	line = NULL;
	while (1)
	{
		line = readline("> ");
		if (!line || !ft_strcmp(line, limiter))
			break ;
		if (write(fd, line, ft_strlen(line)) == -1 || write(fd, "\n", 1) == -1)
		{
			perror("Error writing to temporary file");
			free(line);
			return (-1);
		}
		free(line);
	}
	free(line);
	return (0);
}

/**
 * Initialise le contexte de signal pour le processus enfant du heredoc.
 * Configure les gestionnaires de signaux SIGINT et SIGQUIT.
 *
 * @param hd   Pointeur vers la structure heredoc
 * @param cmd  Pointeur vers la structure de commande active
 * @param st   Pointeur vers l'état global du shell
 */
void	init_signal_context(t_heredoc *hd, t_command *cmd, t_state *st)
{
	t_context	*ctx;

	ctx = get_signal();
	ctx->cmd = cmd;
	ctx->state = st;
	st->hd = hd;
	signal(SIGINT, child_cleanup_and_exit);
	signal(SIGQUIT, SIG_DFL);
}

/**
 * open with O_WRONLY | O_CREAT | O_TRUNC, 0644
 * the heredoc file
 *
 * @return 0 if succes, -1 if failed
 */
int	open_heredoc_file(t_heredoc *hd)
{
	hd->tmp_fd = open(hd->tmp_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (hd->tmp_fd < 0)
	{
		perror("open heredoc file");
		return (-1);
	}
	return (0);
}

/**
 * Lit l'entrée utilisateur ligne par ligne et l'écrit dans le fichier temporaire
 * du heredoc jusqu'à rencontrer le délimiteur ou EOF.
 *
 * @param hd Pointeur vers la structure heredoc contenant le fd et le délimiteur
 */
void	read_and_write_heredoc(t_heredoc *hd)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (!ft_strcmp(line, hd->limiter))
		{
			free(line);
			break ;
		}
		if (write(hd->tmp_fd, line, ft_strlen(line)) < 0
			|| write(hd->tmp_fd, "\n", 1) < 0)
		{
			perror("write");
			free(line);
			return ;
		}
		free(line);
	}
}
