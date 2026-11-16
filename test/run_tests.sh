#!/bin/sh
# Tests Libft v20 : LE GROS MAIN
#   norminette + tests de base + tests portes de francinette
#   (fsoares, Tripouille, alelievr, war-machine).
# Usage : sh test/run_tests.sh [options du main]   (ex: ft_split, --base)

set -u

GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# Se placer a la racine du projet (parent du dossier test).
cd "$(dirname "$0")/.." || exit 2

# norminette est souvent installee dans ~/.local/bin (pip --user)
PATH="$PATH:$HOME/.local/bin"
export PATH

SOURCES_TEST="test/main.c test/framework.c test/norminette.c \
test/fr_part1.c test/fr_part2.c test/fr_bonus.c"
FLAGS="-Wall -Wextra -Werror -g"
EXE="test/test_libft"

# ==========================================
# 1/3 : CONSTRUCTION DE LA LIBFT
# ==========================================

printf "${BLUE}===== 1/3 : make re (Makefile de la libft) =====${NC}\n"
# -g en plus des flags du Makefile : permet d'afficher la ligne de TON code
# (fichier.c:ligne) quand une fonction crashe, boucle ou fuit.
if ! make re FLAGS="-Wall -Wextra -Werror -g" > test/make.log 2>&1; then
    cat test/make.log
    printf "${RED}[KO] make re a echoue : la libft ne compile pas (voir ci-dessus)${NC}\n"
    exit 2
fi
printf "${GREEN}[OK]${NC} libft.a compilee\n"

# ==========================================
# 2/3 : COMPILATION DU GROS MAIN
# ==========================================

printf "${BLUE}===== 2/3 : compilation du banc de tests =====${NC}\n"
# shellcheck disable=SC2086
if cc $FLAGS -DTEST_WRAP_MALLOC $SOURCES_TEST libft.a \
    -Wl,--wrap=malloc -Wl,--wrap=free -o "$EXE" 2> test/cc.log; then
    printf "${GREEN}[OK]${NC} banc compile avec suivi de malloc (-Wl,--wrap)\n"
elif cc $FLAGS $SOURCES_TEST libft.a -o "$EXE" 2> test/cc_sans_wrap.log; then
    printf "${YELLOW}[--]${NC} l'editeur de liens ne supporte pas --wrap (macOS ?) :\n"
    printf "     tailles de malloc, fuites et protections malloc ne seront pas testees\n"
else
    cat test/cc.log
    printf "${RED}[KO] le banc de tests ne compile pas.${NC}\n"
    printf "     Verifie d'abord les prototypes de libft.h (sh test/text_prototypes.sh)\n"
    exit 2
fi

# ==========================================
# 3/3 : EXECUTION
# ==========================================

printf "${BLUE}===== 3/3 : execution =====${NC}\n"
"./$EXE" "$@"
status=$?
rm -f test/make.log test/cc.log test/cc_sans_wrap.log
exit $status
