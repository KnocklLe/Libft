CC = cc
NAME = libft.a
FLAGS = -Wall -Werror -Wextra
OBJECTS = $(SOURCES:.c=.o)
SOURCES = ft_atoi.c ft_bzero.c ft_calloc.c ft_isalnum.c ft_isalpha.c \
ft_isascii.c ft_isdigit.c ft_isprint.c ft_memchr.c ft_memcmp.c ft_memcpy.c \
ft_memmove.c ft_memset.c ft_strchr.c ft_strdup.c ft_strlcat.c ft_strlcpy.c \
ft_strlen.c ft_strncmp.c ft_strnstr.c ft_strrchr.c ft_tolower.c ft_toupper.c\
ft_substr.c ft_strjoin.c ft_putchar_fd.c ft_putendl_fd.c ft_putstr_fd.c\
ft_split.c ft_itoa.c ft_striteri.c ft_strtrim.c ft_strmapi.c ft_putnbr_fd.c\
ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c ft_lstadd_back.c

all: $(NAME)

$(NAME): $(OBJETS)
	ar rcs $(NAME) $(OBJETS)

%.o: %.c libft.h
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	@rm -f $(OBJETS)
	@echo 'fichiers nettoye'

fclean: clean
	@rm -f $(NAME)

re: fclean all

main:
	@$(CC) $(CFLAGS) $(SOURCES) main.c
	@./a.out
	@rm a.out


testfiles:
	@for file in $(SOURCES); do \
		if [ ! -f "$$file" ]; then \
			echo "[ERREUR] : $$file WRONG NAME !"; \
		else \
			echo "[OK] : $$file FOUND"; \
		fi; \
	done

checkall:
	@echo " __Vérification des fichiers .c et du code___"
	@echo ""
	@for file in $(SOURCES); do \
		if [ ! -f "$$file" ]; then \
			echo "[ERROR] : $$file NOT FOUND !"; \
		else \
			func_name=$${file%.c}; \
			if ! grep -q "$$func_name" "$$file"; then \
				echo "WARNING : $$file EXIT, BUT THE '$$func_name' IS NOT CORRECT."; \
			else \
				echo "OK : $$file EXIT AND HAS '$$func_name'"; \
			fi; \
		fi; \
	done
	@echo ""
	@echo " __Vérification des déclarations dans libft.h___"
	@echo ""
	@if [ ! -f "libft.h" ]; then \
		echo "[ERROR] : libft.h NOT FOUND !"; \
	else \
		for file in $(SOURCES); do \
			func_name=$${file%.c}; \
			if ! grep -q "$$func_name" "libft.h"; then \
				echo "[ERROR] : '$$func_name' NOT IN libft.h !"; \
			else \
				echo "[OK] : '$$func_name' IS IN libft.h"; \
			fi; \
		done; \
	fi

.PHONY: all clean fclean re bonus main testfiles name checkall