# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abidaux <abidaux@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/17 17:09:20 by abidaux           #+#    #+#              #
#    Updated: 2025/03/17 17:58:29 by abidaux          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = gcc
CFLAGS = -Wall -Werror -Wextra

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

FULL_CFLAGS = $(CFLAGS) -I$(LIBFT_DIR) -lreadline

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

GREEN = \033[0;32m
YELLOW = \033[0;33m
RED = \033[0;31m
BLUE = \033[0;34m
CYAN = \033[0;36m
RESET = \033[0m

all: $(LIBFT) $(NAME)

$(LIBFT):
	@echo "$(BLUE)Téléchargement de la libft...$(RESET)"
	@git clone https://github.com/4drie2/libft.git $(LIBFT_DIR) 2>/dev/null || (cd $(LIBFT_DIR) && git pull)
	@echo "$(BLUE)Compilation de la libft...$(RESET)"
	@make --no-print-directory -C $(LIBFT_DIR)
	@echo "$(GREEN)Libft compilée avec succès !$(RESET)"

%.o: %.c
	@echo "$(YELLOW)Compilation de $<...$(RESET)"
	@$(CC) $(FULL_CFLAGS) -c $< -o $@


$(NAME): $(OBJS)
	@echo "$(YELLOW)Création de l'exécutable $(NAME)...$(RESET)"
	@$(CC) $(OBJS) -o $(NAME) -L$(LIBFT_DIR) -lft -lreadline -lhistory
	@echo "$(GREEN)Compilation de $(NAME) terminée avec succès !$(RESET)"
	@sleep 0.5
	@echo ""
	@echo "$(CYAN)"
	@bash -c 'lines=(" __  __ _____ _   _ _____  _____ _    _ ______  _      _      " "|  \/  |_   _| \ | |_   _|/ ____| |  | |  ____ | |    | |     " "| \  / | | | |  \| | | | | (___ | |__| | |__   | |    | |     " "| |\/| | | | | . \` | | |  \___ \|  __  |  __|  | |    | |     " "| |  | |_| |_| |\  |_| |_ ____) | |  | | |____ | |____| |____ " "|_|  |_|_____|_| \_|_____|_____/|_|  |_|______||______|______|" "" ); for line in "$${lines[@]}"; do for ((i=0; i<$${#line}; i++)); do echo -n "$${line:i:1}"; sleep 0.002; done; echo ""; done'
	@sleep 0.2
	@bash -c 'for letter in "				" b y " " p t h u l l i " " "&" " " a b i d a u x; do echo -ne "$(CYAN)$$letter$(RESET)"; sleep 0.05; done; echo "";'
	@echo "$(RESET)"

clean:
	@echo "$(RED)Suppression des fichiers objets...$(RESET)"
	@rm -f $(OBJS)
	@make -C $(LIBFT_DIR) clean 2>/dev/null || true
	@echo "$(GREEN)Nettoyage terminé !$(RESET)"

fclean: clean
	@echo "$(RED)Suppression de l'exécutable $(NAME)...$(RESET)"
	@rm -f $(NAME)
	@if [ -d "$(LIBFT_DIR)" ]; then \
		echo "$(RED)Suppression du dossier libft...$(RESET)"; \
		rm -rf $(LIBFT_DIR); \
	fi
	@echo "$(GREEN)Nettoyage complet terminé !$(RESET)"

speed:
	@echo "$(YELLOW)Mode Speed: Recompilation sans animation...$(RESET)"
	@rm -f $(OBJS) $(NAME)
	@$(MAKE) --no-print-directory $(OBJS)
	@echo "$(YELLOW)Création de l'exécutable $(NAME)...$(RESET)"
	@$(CC) $(OBJS) -o $(NAME) -L$(LIBFT_DIR) -lft -lreadline -lhistory
	@echo "$(GREEN)Compilation de $(NAME) terminée avec succès !$(RESET)"

re: fclean all

.PHONY: all clean fclean re speed
