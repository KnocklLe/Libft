
#!/bin/sh
# Tests Libft v19.3 : execution standard + injection d'echecs malloc.
# Le seul standard de compilation utilise est celui par defaut de cc.

set -u

# Se placer a la racine du projet (parent du dossier test).
cd "$(dirname "$0")/.." || exit 2

printf '\n===== 1/3 : construire la libft selon son Makefile =====\n'
if ! make re; then
    printf 'ECHEC : make re\n' >&2
    exit 2
fi

printf '\n===== 2/3 : compiler et executer les tests normaux =====\n'
if ! cc -Wall -Wextra -Werror test/main.c libft.a -o test/test_libft; then
    printf 'ECHEC : compilation des tests normaux\n' >&2
    exit 2
fi

normal=0
./test/test_libft || normal=1

printf '\n===== 3/3 : tester les echecs successifs de malloc =====\n'
printf 'Utilise une option de l editeur de liens GNU, uniquement pour le test.\n'

if ! cc -Wall -Wextra -Werror -DTEST_WRAP_MALLOC test/main.c libft.a \
    -Wl,--wrap=malloc -Wl,--wrap=free -o test/test_libft_malloc; then
    printf 'ECHEC : compilation des tests malloc (linker GNU requis)\n' >&2
    exit 2
fi

alloc=0
./test/test_libft_malloc || alloc=1

printf '\n===== BILAN =====\n'
printf 'Tests normaux       : %s\n' "$(if [ "$normal" -eq 0 ]; then echo OK; else echo ECHEC; fi)"
printf 'Tests avec malloc   : %s\n' "$(if [ "$alloc" -eq 0 ]; then echo OK; else echo ECHEC; fi)"

if [ "$normal" -ne 0 ] || [ "$alloc" -ne 0 ]; then
    exit 1
fi

exit 0
