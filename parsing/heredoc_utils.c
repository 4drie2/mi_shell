/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthuilli <pthuilli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 16:45:03 by abidaux           #+#    #+#             */
/*   Updated: 2025/04/03 11:21:30 by pthuilli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * Crée un fichier temporaire pour un heredoc en utilisant un processus fils.
 * Gère les signaux pour permettre l'interruption du heredoc avec SIGINT.
 * 
 * @param lim Délimiteur de fin du heredoc
 * @param out_tmp Pointeur vers le nom du fichier temporaire généré
 * @param cmd Commande en cours d'exécution
 * @param st État global du shell
 * @return 0 en cas de succès, -1 en cas d'erreur
 */
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
	hd.tmp_filename = generate_tmp_file();
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

/**
 * Traite tous les heredocs d'une commande en créant des fichiers temporaires.
 * 
 * @param cmd Pointeur vers la structure de commande
 * @param state Pointeur vers l'état global du shell
 * @return 0 en cas de succès, -1 en cas d'erreur
 */
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

/**
 * Libère toutes les ressources allouées dans le processus fils du heredoc 
 * avant de quitter.
 * 
 * @param hd Structure contenant les informations du heredoc
 * @param cmd Commande en cours de traitement
 * @param st État global du shell
 */
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
	free_envp(st->envp);
	exit(0);
}

/**
 * Fonction exécutée par le processus fils pour gérer la lecture d'un heredoc.
 * Ce processus configure les signaux, ouvre le fichier temporaire,
 * lit l'entrée utilisateur jusqu'au délimiteur, et termine proprement.
 * 
 * @param hd Structure contenant les informations du heredoc
 * @param cmd Commande en cours de traitement
 * @param st État global du shell
 */
void	child_read_heredoc(t_heredoc *hd, t_command *cmd, t_state *st)
{
	init_signal_context(hd, cmd, st);
	if (open_heredoc_file(hd) < 0)
		exit (1);
	read_and_write_heredoc(hd);
	free_child_and_exit(hd, cmd, st);
	exit (0);
}
