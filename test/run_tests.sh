
#!/bin/sh
# Tests Libft v19.3 : execution standard + injection d'echecs malloc.
# Le seul standard de compilation utilise est celui par defaut de cc.

set -u

# ==========================================
# COULEURS
# ==========================================

GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m'

# Se placer a la racine du projet (parent du dossier test).
cd "$(dirname "$0")/.." || exit 2

# ==========================================
# 1/3 : CONSTRUCTION DE LA LIBFT
# ==========================================

printf "\n${BLUE}===== 1/3 : construire la libft selon son Makefile =====${NC}\n"

if ! make re; then
    printf "${RED}ECHEC : make re${NC}\n" >&2
    exit 2
fi

printf "${GREEN}[OK] Compilation de la libft${NC}\n"

# ==========================================
# 2/3 : TESTS NORMAUX
# ==========================================

printf "\n${BLUE}===== 2/3 : compiler et executer les tests normaux =====${NC}\n"

if ! cc -Wall -Wextra -Werror test/main.c libft.a -o test/test_libft; then
    printf "${RED}ECHEC : compilation des tests normaux${NC}\n" >&2
    exit 2
fi

printf "${GREEN}[OK] Compilation des tests normaux${NC}\n"

normal=0
./test/test_libft || normal=1

if [ "$normal" -eq 0 ]; then
    printf "${GREEN}[OK] Tests normaux${NC}\n"
else
    printf "${RED}[KO] Tests normaux${NC}\n"
fi

# ==========================================
# 3/3 : TESTS ECHECS MALLOC
# ==========================================

printf "\n${BLUE}===== 3/3 : tester les echecs successifs de malloc =====${NC}\n"
printf 'Utilise une option de l editeur de liens GNU, uniquement pour le test.\n'

if ! cc -Wall -Wextra -Werror -DTEST_WRAP_MALLOC test/main.c libft.a \
    -Wl,--wrap=malloc -Wl,--wrap=free -o test/test_libft_malloc; then
    printf "${RED}ECHEC : compilation des tests malloc (linker GNU requis)${NC}\n" >&2
    exit 2
fi

printf "${GREEN}[OK] Compilation des tests malloc${NC}\n"

alloc=0
./test/test_libft_malloc || alloc=1

if [ "$alloc" -eq 0 ]; then
    printf "${GREEN}[OK] Tests avec malloc${NC}\n"
else
    printf "${RED}[KO] Tests avec malloc${NC}\n"
fi

# ==========================================
# BILAN
# ==========================================

printf "\n${BLUE}===== BILAN =====${NC}\n"

if [ "$normal" -eq 0 ]; then
    printf "Tests normaux       : ${GREEN}OK${NC}\n"
else
    printf "Tests normaux       : ${RED}ECHEC${NC}\n"
fi

if [ "$alloc" -eq 0 ]; then
    printf "Tests avec malloc   : ${GREEN}OK${NC}\n"
else
    printf "Tests avec malloc   : ${RED}ECHEC${NC}\n"
fi

# ==========================================
# CODE DE SORTIE
# ==========================================

if [ "$normal" -ne 0 ] || [ "$alloc" -ne 0 ]; then
    exit 1
fi

printf "\n${GREEN}Tous les tests sont passes !${NC}\n"

exit 0
