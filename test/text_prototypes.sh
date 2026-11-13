
#!/bin/sh

# ==========================================
# LIBFT - CHECK PROTOTYPES
# ==========================================

HEADER="$(dirname "$0")/../libft.h"
TMP_EXPECTED="/tmp/libft_expected_$$"
TMP_ACTUAL="/tmp/libft_actual_$$"

# Couleurs ANSI
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

trap 'rm -f "$TMP_EXPECTED" "$TMP_ACTUAL"' 0
trap 'exit 1' HUP INT TERM

# Vérification du header
if [ ! -f "$HEADER" ]; then
    printf "${RED}[KO]${NC} Header introuvable : %s\n" "$HEADER"
    exit 1
fi

# ==========================================
# PROTOTYPES ATTENDUS
# ==========================================

cat > "$TMP_EXPECTED" <<'EOF'
void ft_lstdelone(t_list *lst, void (*del)(void *));
char *ft_strmapi(char const *s, char (*f)(unsigned int, char));
char *ft_strtrim(char const *s1, char const *set);
void ft_striteri(char *s, void (*f)(unsigned int, char *));
char *ft_itoa(int n);
char **ft_split(char const *s, char c);
void ft_putstr_fd(char *s, int fd);
char *ft_strjoin(char const *s1, char const *s2);
char *ft_substr(char const *s, unsigned int start, size_t len);
char *ft_strrchr(const char *s, int c);
char *ft_strnstr(const char *big, const char *little, size_t len);
int ft_strncmp(const char *s1, const char *s2, size_t n);
size_t ft_strlcat(char *dest, const char *src, size_t size);
char *ft_strchr(const char *s, int c);
void *ft_memset(void *b, int c, size_t len);
void *ft_memmove(void *dest, const void *src, size_t n);
void *ft_memcpy(void *dest, const void *src, size_t n);
int ft_memcmp(const void *s1, const void *s2, size_t n);
void *ft_memchr(const void *s, int c, size_t n);
int ft_isprint(int c);
int ft_isdigit(int c);
int ft_isascii(int c);
int ft_isalpha(int c);
int ft_isalnum(int c);
void *ft_calloc(size_t nmemb, size_t size);
int ft_atoi(const char *str);
void ft_bzero(void *tab, size_t n);
size_t ft_strlen(const char *str);
char *ft_strdup(const char *s);
size_t ft_strlcpy(char *dest, const char *src, size_t size);
void ft_putendl_fd(char *s, int fd);
void ft_putchar_fd(char c, int fd);
void ft_putnbr_fd(int n, int fd);
t_list *ft_lstnew(void *content);
void ft_lstadd_front(t_list **lst, t_list *new);
unsigned int ft_lstsize(t_list *lst);
t_list *ft_lstlast(t_list *lst);
void ft_lstadd_back(t_list **lst, t_list *new);
void ft_lstclear(t_list **lst, void (*del)(void *));
void ft_lstiter(t_list *lst, void (*f)(void *));
t_list *ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));
int ft_tolower(int c);
int ft_toupper(int c);
EOF

# ==========================================
# EXTRACTION DU HEADER
# ==========================================

sed \
    -e '/^[[:space:]]*\/\*/d' \
    -e 's/\/\*.*\*\///g' \
    "$HEADER" |
tr '\n' ' ' |
sed 's/;/;\
/g' |
while IFS= read -r line
do
    # Supprime tous les espaces et tabulations
    compact=$(printf '%s' "$line" | tr -d '[:space:]')

    case "$compact" in
        *ft_*)
            printf '%s\n' "$compact"
            ;;
    esac
done > "$TMP_ACTUAL"

# ==========================================
# COMPARAISON
# ==========================================

ok=0
ko=0

printf '\n========== LIBFT PROTOTYPE CHECK ==========\n\n'

while IFS= read -r expected
do
    # Normalise le prototype attendu
    expected_compact=$(printf '%s' "$expected" | tr -d '[:space:]')

    # Extrait le nom de la fonction
    name=$(printf '%s\n' "$expected_compact" |
        sed 's/.*\(ft_[a-zA-Z0-9_]*\)(.*/\1/')

    # Recherche le prototype correspondant
    found=$(grep -F "$name(" "$TMP_ACTUAL" | head -n 1)

    if [ "$found" = "$expected_compact" ]; then
        printf "${GREEN}[OK]${NC} %s\n" "$name"
        ok=$((ok + 1))
    else
        printf "${RED}[KO]${NC} %s\n" "$name"
        printf '     Attendu : %s\n' "$expected"

        if [ -n "$found" ]; then
            printf '     Trouvé  : %s\n' "$found"
        else
            printf '     Trouvé  : ABSENT\n'
        fi

        ko=$((ko + 1))
    fi
done < "$TMP_EXPECTED"

# ==========================================
# BILAN
# ==========================================

printf '\n========== BILAN ==========\n'
printf 'Total : %s\n' "$((ok + ko))"
printf "${GREEN}[OK]${NC}   : %s\n" "$ok"
printf "${RED}[KO]${NC}   : %s\n" "$ko"

if [ "$ko" -ne 0 ]; then
    exit 1
fi

exit 0
