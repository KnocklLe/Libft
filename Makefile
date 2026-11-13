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
ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c ft_lstadd_back.c\
ft_lstdelone.c ft_lstclear.c ft_lstiter.c ft_lstmap.c

all: $(NAME)

$(NAME): $(OBJECTS)
	ar rcs $(NAME) $(OBJECTS)

%.o: %.c libft.h
	$(CC) $(FLAGS) -c $< -o $@
clean:
	@rm -f $(OBJECTS)
	@echo 'fichiers nettoye'

fclean: clean
	@rm -f $(NAME)

re: fclean all

main:
	@$(CC) $(FLAGS) $(SOURCES) main.c
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

test:
	

.PHONY: all clean fclean re testfiles
