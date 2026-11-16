TESTS LIBFT V20 - LE GROS MAIN
==============================

UN SEUL PROGRAMME QUI FAIT TOUT
  1. NORMINETTE  : tous les .c / .h de la racine (pas le dossier test/).
                   Pour chaque erreur : ligne, colonne, code, message de
                   norminette ET explication en francais.
  2. TESTS DE BASE (les anciens tests, ~1,66 million de verifications) :
                   comparaison libc / references, aleatoire reproductible,
                   cas limites, sorties fd exactes, listes, malloc en echec.
  3. FRANCINETTE : les tests des 4 testeurs utilises par francinette
                   (https://github.com/xicodomingues/francinette) portes en C :
                   fsoares, Tripouille (libftTester), alelievr
                   (libft-unit-test) et war-machine.

AFFICHAGE
  [OK] en vert  : tout est bon.
  [KO] en rouge : erreur. Sous la ligne, pour chaque erreur :
      x test/fr_part2.c:310 [fsoares] ft_substr("", 0, 0)   <- fichier:ligne
          attendu : ""                                      <- du test
          obtenu  : "#"
        -> explication de ce que la fonction aurait du faire
  Les CRASHS sont attrapes (chaque groupe tourne dans son propre processus) :
  segfault, bus error, abort (double free...), boucle infinie (TIMEOUT).
  Le message donne l'appel qui a crashe, la ligne du test, la cause probable
  ET LA LIGNE DANS TON CODE :
      x CRASH [fsoares] SIGSEGV (Segmentation fault)
          pendant : ft_strtrim("", "")
          crash dans ton code : ft_strtrim.c:8 (ft_strtrim)
          (TIMEOUT : "ta fonction tournait ici : ft_itoa.c:11 (ft_itoa)")
  Pour une FUITE, la ligne du malloc jamais libere est donnee :
          bloc de 42 octet(s) alloue par malloc dans ton code : ft_strjoin.c:11
  (Linux : la libft est compilee avec -g et addr2line est utilise.)
  Pour un simple mauvais resultat il n'y a pas de "ligne fautive" : regarde
  attendu / obtenu et le test a fichier:ligne.

  Pendant le suivi de malloc, chaque bloc alloue est rempli de '#' :
  si tu vois des '#' dans "obtenu", tu as oublie le '\0' final.

UTILISATION
  sh test/run_tests.sh                 tout (make re + compilation + tests)
  sh test/run_tests.sh ft_split        seulement francinette pour ft_split
  sh test/run_tests.sh --norme         seulement la norminette
  sh test/run_tests.sh --base          seulement les tests de base
  sh test/run_tests.sh --francinette   seulement francinette
  sh test/run_tests.sh --sans-norme    tout sauf la norminette
  TEST_TIMEOUT=30 sh test/run_tests.sh delai avant TIMEOUT (defaut 10 s)
  make test                            (regle existante du Makefile)

FICHIERS
  test.h        API commune (verifications V_*, T_*, suivi malloc...)
  framework.c   implementation : memoire partagee, fork, malloc, fd...
  main.c        tests de base + le main qui enchaine tout + bilan
  norminette.c  lancement et analyse de norminette (+ explications FR)
  fr_part1.c    francinette : partie 1 (is*, to*, mem*, str*, atoi...)
  fr_part2.c    francinette : partie 2 (substr, join, trim, split, itoa...)
  fr_bonus.c    francinette : bonus (ft_lst*)
  run_tests.sh  compile la libft puis le banc, puis l'execute
  text_prototypes.sh   verifie les prototypes de libft.h

SUIVI DE MALLOC (Linux)
  Le banc est lie avec -Wl,--wrap=malloc -Wl,--wrap=free (editeur de liens
  GNU). Il verifie alors : taille EXACTE de chaque malloc, absence de fuites,
  et protection : chaque malloc est force a NULL tour a tour, la fonction
  doit renvoyer NULL et liberer ce qu'elle avait deja alloue.
  Sur macOS (pas de --wrap) le script compile sans : ces controles sont
  sautes (message jaune [--]).

CE QUI N'EST PAS TESTE (volontairement)
  - Les tests alelievr "*_null*" (crash attendu quand on passe NULL) : le
    sujet ne demande rien pour NULL.
  - Les tests de vitesse d'alelievr.
  - Les fonctions qui ne sont pas dans la libft actuelle (strcpy, strcat,
    memccpy, strsplit...).

IMPORTANT
  Le dossier test/ n'est PAS a la norme : NE PAS le rendre sur le depot
  d'evaluation (vogsphere), sinon la norminette de la moulinette echoue.
  Aucun flag de standard (-std=...) n'est utilise.
