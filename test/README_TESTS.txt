TESTS LIBFT V19.3 - VERSION COMPLETE
===================================

FICHIERS
  main.c       : tests des 43 fonctions, comparaison libc/references,
                 tests aleatoires reproductibles, cas limites, sortie fd exacte,
                 listes, injections d'echecs d'allocation.
  run_tests.sh : lance deux executables distincts, sans modifier le depot.

UTILISATION
  1. Extraire ces fichiers a la racine de la Libft (avec le Makefile, libft.h).
  2. Lancer : sh run_tests.sh
  3. Lire chaque [FAIL] et le resume final de chaque executable.

OU MANUELLEMENT
  make re
  cc -Wall -Wextra -Werror main.c libft.a -o test_libft
  ./test_libft

  cc -Wall -Wextra -Werror -DTEST_WRAP_MALLOC main.c libft.a \
    -Wl,--wrap=malloc -Wl,--wrap=free -o test_libft_malloc
  ./test_libft_malloc

POURQUOI DEUX MODES ?
  Le mode normal applique toutes les validations du sujet.
  Le mode malloc ajoute une interception, a l'edition des liens, des appels
  a malloc/free par la bibliotheque. Il simule un echec a chaque allocation,
  controle les retours NULL, les fuites de noeuds, les contenus liberes et
  les nettoyages de ft_split et ft_lstmap.
  L'option -Wl,--wrap fonctionne avec GNU ld (Linux). Elle NE change PAS
  les options requises pour construire la Libft elle-meme.

VERIFICATION MEMOIRE OPTIONNELLE
  Il est aussi possible d'utiliser Valgrind :
    valgrind --leak-check=full --show-leak-kinds=all ./test_libft
  ou AddressSanitizer apres reconstruction de la libft avec les flags
  d'instrumentation. Ce sont des outils de diagnostic hors evaluation,
  et non des exigences supplementaires du sujet.

IMPORTANT
  Aucun -std=c99, -std=c11 ni autre flag de standard C n'est utilise.
  Le fichier main.c est UNIQUEMENT un test : NE PAS l'ajouter aux sources
  soumises ou aux fichiers archives dans libft.a.
  Ce banc ne peut pas prouver une equivalence pour TOUTES les entrees
  mathematiquement possibles. Il evite deliberement les comportements
  indéfinis (NULL pour strlen, chevauchement pour memcpy, atoi hors plage).
  ft_strncmp/ft_memcmp comparent le SIGNE du resultat lorsque la manpage
  ne garantit pas la valeur numerique exacte du delta.

VALIDATION DE CETTE LIVRAISON
  main.c compile avec cc -Wall -Wextra -Werror.
  Le mode normal : 1 663 258 verifications, 0 erreur sur une reference.
  Le mode injection : 1 663 298 verifications, 0 erreur sur une reference
  adaptee pour permettre l'injection. Le banc d'injection signale bien
  une version de reference volontairement fuyante de ft_split.
  La bibliotheque du GitHub n'a PAS pu etre clonee dans ce conteneur ;
  executer ce banc dans son depot pour obtenir ses resultats reels.
