# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pthuilli <pthuilli@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/17 17:09:20 by abidaux           #+#    #+#              #
#    Updated: 2025/04/22 17:52:31 by pthuilli         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = gcc
CFLAGS = -Wall -Werror -Wextra -g
FULL_CFLAGS = $(CFLAGS) -I$(LIBFT_DIR) -lreadline

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

TOTAL_FILES = $(words $(SRCS))
CURRENT_FILE = 0

SRCS = \
    main.c \
    utils/ft_arraydup.c \
    utils/ft_swap_strings.c \
    utils/parse_utils.c \
    utils/parse_utils2.c \
    utils/parse_utils3.c \
    utils/quote_utils.c \
    utils/quote_utils2.c \
    utils/signals.c \
    utils/strtol.c \
    utils/strtol2.c \
    utils/syntax_utils.c \
    utils/token_utils.c \
    utils/token_utils_create.c \
    utils/utils.c \
    utils/utils_env.c \
    utils/utils_free.c \
    exec/exec_shell.c \
    exec/exec_shell2.c \
    exec/exec_utils.c \
    exec/exec_utils2.c \
    exec/exec_utils3.c \
    exec/exec_utils4.c \
    exec/external_command.c \
    exec/external_command2.c \
    exec/external_utils.c \
    exec/external_utils2.c \
    exec/forking.c \
    exec/multiples_cmd.c \
    exec/multiples_cmd2.c \
    exec/multiples_utils.c \
    exec/path.c \
    exec/path2.c \
    exec/pipes.c \
    exec/state.c \
    exec/builtins/cd.c \
    exec/builtins/cd_utils.c \
    exec/builtins/echo.c \
    exec/builtins/env.c \
    exec/builtins/exit.c \
    exec/builtins/export.c \
    exec/builtins/export_utils.c \
    exec/builtins/export_utils2.c \
    exec/builtins/pwd.c \
    exec/builtins/unset.c \
    parsing/command_free.c \
    parsing/command_utils.c \
    parsing/heredoc.c \
    parsing/heredoc_utils.c \
    parsing/heredoc_utils2.c \
    parsing/parse_input.c \
    parsing/parse_input2.c \
    parsing/parsing.c \
    parsing/quotes.c \
    parsing/redir.c

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
	@$(eval CURRENT_FILE=$(shell expr $(CURRENT_FILE) + 1))
	@printf "\r$(YELLOW)[$(CURRENT_FILE)/$(TOTAL_FILES)]$(RESET) Compilation: %-50s" "$<"
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
