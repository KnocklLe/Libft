CC = cc
NAME = libft.a
FLAGS = -Wall -Werror -Wextra
OBJECTS = $(SOURCES:.c=.o)
SOURCES = ft_atoi.c ft_bzero.c ft_calloc.c ft_isalnum.c ft_isalpha.c \
ft_isascii.c ft_isdigit.c ft_isprint.c ft_memcmp.c ft_memcpy.c \
ft_memmove.c ft_memset.c ft_strchr.c ft_strdup.c ft_strlcat.c ft_strlcpy.c \
ft_strlen.c ft_strncmp.c ft_strnstr.c ft_strrchr.c ft_tolower.c ft_toupper.c\
ft_substr.c ft_strjoin.c ft_putchar_fd.c ft_memchr.c\
ft_split.c ft_itoa.c ft_striteri.c ft_strtrim.c ft_strmapi.c

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


.PHONY: all clean fclean re bonus main