# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pthuilli <pthuilli@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/17 17:09:20 by abidaux           #+#    #+#              #
#    Updated: 2025/03/19 20:23:50 by pthuilli         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = gcc
CFLAGS = -Wall -Werror -Wextra

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

FULL_CFLAGS = $(CFLAGS) -I$(LIBFT_DIR) -lreadline

SRCS = $(wildcard *.c) $(wildcard utils/*.c) $(wildcard exec/*.c) $(wildcard exec/builtins/*.c) $(wildcard parsing/*.c)
OBJ_DIR = obj
OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)

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

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
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
	@rm -rf $(OBJ_DIR)
	@make --no-print-directory -C $(LIBFT_DIR) clean 2>/dev/null || true
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
	@echo "$(YELLOW)⚡⚡⚡ make Speed: recopile les objs hors libft ⚡⚡⚡$(RESET)"
	@rm -f $(NAME)
	@rm -rf $(OBJ_DIR)
	@for src in $(SRCS); do \
		mkdir -p $(OBJ_DIR)/$$(dirname $$src); \
		$(CC) $(FULL_CFLAGS) -c $$src -o $(OBJ_DIR)/$${src%.c}.o; \
	done
	@echo "$(YELLOW)Création de l'exécutable $(NAME)...$(RESET)"
	@$(CC) $(OBJS) -o $(NAME) -L$(LIBFT_DIR) -lft -lreadline -lhistory
	@echo "$(GREEN)Compilation de $(NAME) terminée avec succès !$(RESET)"

re: fclean all

.PHONY: all clean fclean re speed
