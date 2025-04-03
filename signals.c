/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abidaux <abidaux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 18:20:56 by abidaux           #+#    #+#             */
/*   Updated: 2025/04/03 10:06:39 by abidaux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	reset_signals_after_execution(void)
{
	setup_signals();
}

/**
 * Configure les gestionnaires de signaux spécifiquement 
 * pour le mode heredoc.
 * Définit le comportement par défaut pour SIGINT 
 * (Ctrl+C) et SIGQUIT (Ctrl+\)
 * pendant la lecture d'un heredoc.
 *
 * @return Aucune valeur (void)
 */
void	setup_signals_for_heredoc(void)
{
	struct sigaction	sa_int;

	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sa_int.sa_handler = SIG_DFL;
	sigaction(SIGINT, &sa_int, NULL);
	sigaction(SIGQUIT, &sa_int, NULL);
}

/**
 * Configure les gestionnaires de signaux pour le mode d'exécution des commandes.
 * Rétablit le comportement par défaut pour SIGINT (Ctrl+C) et SIGQUIT (Ctrl+\)
 * avec le drapeau SA_RESTART pour permettre la 
 * reprise des appels système interrompus.
 *
 * @return Aucune valeur (void)
 */
void	setup_signals_for_execution(void)
{
	struct sigaction	sa_default;

	sigemptyset(&sa_default.sa_mask);
	sa_default.sa_flags = SA_RESTART;
	sa_default.sa_handler = SIG_DFL;
	sigaction(SIGINT, &sa_default, NULL);
	sigaction(SIGQUIT, &sa_default, NULL);
}

/**
 * Gestionnaire pour le signal SIGINT (Ctrl+C).
 * Comporte différemment selon que des processus enfants 
 * sont en cours d'exécution ou non.
 *
 * @param signo Le numéro du signal reçu (inutilisé, mais 
 * requis par le prototype)
 * @return Aucune valeur (void)
 */
void	handle_sigint(int signo)
{
	(void)signo;
	if (!waitpid(-1, NULL, WNOHANG))
		write(STDOUT_FILENO, "\n", 1);
	else
	{
		write (STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

/**
 * Configuration initiale des gestionnaires de signaux pour le shell.
 * - SIGINT (Ctrl+C): Redirigé vers handle_sigint pour gestion personnalisée
 * - SIGQUIT (Ctrl+\): Ignoré
 * Le drapeau SA_RESTART est défini pour les deux signaux afin de permettre
 * la reprise des appels système interrompus.
 *
 * @return Aucune valeur (void)
 */
void	setup_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	sa_int.sa_handler = handle_sigint;
	sigaction(SIGINT, &sa_int, NULL);
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = SA_RESTART;
	sa_quit.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa_quit, NULL);
}
