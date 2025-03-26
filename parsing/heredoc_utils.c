/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abidaux <abidaux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 16:45:03 by abidaux           #+#    #+#             */
/*   Updated: 2025/03/26 16:45:06 by abidaux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// gère la création d'un heredoc (<<) dans un shell en trois étapes.
// - elle configure le programme
// pour ignorer temporairement Ctrl+C (SIGINT) et
// génère un nom de fichier temporaire.
// - crée un processus enfant avec fork(), et selon que l'on soit
// dans le parent ou l'enfant
// - exécute différentes fonctions
// pour traiter l'entrée utilisateur jusqu'au délimiteur spécifié.
int	fork_one_heredoc(const char *lim, char **out_tmp,
		t_command *cmd, t_state *st)
{
	struct sigaction	ign;
	struct sigaction	old;
	t_heredoc			hd;
	pid_t				pid;

	sigemptyset(&ign.sa_mask);
	ign.sa_flags = 0;
	ign.sa_handler = SIG_IGN;
	if (sigaction(SIGINT, &ign, &old) < 0)
		return (-1);
	hd.tmp_filename = generate_tmp_filename();
	if (!hd.tmp_filename)
		return (-1);
	hd.limiter = (char *)lim;
	hd.out_tmp_filename = out_tmp;
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		free(hd.tmp_filename);
		return (-1);
	}
	if (pid == 0)
		return (fork_child(&hd, cmd, st, &old));
	return (fork_parent(pid, &hd, st, &old));
}

// Cette fonction traite tous les heredocs (<<)
// présents dans une structure de commande, en remplaçant chaque heredoc
// par un fichier temporaire contenant son contenu.
// Elle prend en paramètre la commande à traiter (cmd)
// et l'état global du programme (state),
// puis renvoie 0 en cas de succès ou -1 en cas d'erreur.
int	handle_all_heredocs(t_command *cmd, t_state *state)
{
	char	*tmpfile;
	int		i;

	i = -1;
	while (++i < cmd->nb_in)
	{
		if (cmd->in[i]->type == R_HEREDOC)
		{
			tmpfile = NULL;
			if (fork_one_heredoc(cmd->in[i]->target, &tmpfile, cmd, state) < 0)
				return (-1);
			free(cmd->in[i]->target);
			cmd->in[i]->target = tmpfile;
		}
	}
	return (0);
}

// free les chids et leurs contenus puis exit
void	free_child_and_exit(t_heredoc *hd, t_command *cmd, t_state *st)
{
	close(hd->tmp_fd);
	if (cmd->is_pipe)
		close_all_pipes(st->num_pipes, (int (*)[2])st->pipes);
	if (st->pids)
		free(st->pids);
	if (st->pipes)
		free(st->pipes);
	if (cmd->path)
		free(cmd->path);
	while (cmd->prev)
		cmd = cmd->prev;
	free_command_list(cmd);
	if (hd->tmp_filename)
		free(hd->tmp_filename);
	free_environment(st->envp);
	exit(0);
}

void	child_read_heredoc(t_heredoc *hd, t_command *cmd, t_state *st)
{
	init_signal_context(hd, cmd, st);
	if (open_heredoc_file(hd) < 0)
		exit (1);
	read_and_write_heredoc(hd);
	free_child_and_exit(hd, cmd, st);
	exit (0);
}
