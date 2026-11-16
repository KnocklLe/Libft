/*
** test.h - socle commun du banc de tests Libft.
**
** Fournit :
**  - les couleurs ([OK] vert, [KO] rouge) ;
**  - une memoire partagee (g_bac) qui survit aux crashs des processus fils ;
**  - les verifications V_* qui notent la LIGNE du test, l'appel, l'attendu,
**    l'obtenu et une explication en francais ;
**  - le suivi de malloc/free (taille exacte, fuites, echecs forces) quand
**    l'executable est lie avec -Wl,--wrap=malloc -Wl,--wrap=free ;
**  - la "memoire electrique" (page interdite juste apres un buffer) ;
**  - la capture de ce qui est ecrit dans un descripteur de fichier.
**
** Ce dossier n'est PAS a rendre : il n'est pas a la norme.
*/
#ifndef TEST_H
# define TEST_H

# include "../libft.h"
# include <ctype.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <signal.h>
# include <stdarg.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/mman.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

/* ========================================================================== */
/*                                 COULEURS                                   */
/* ========================================================================== */

# define ROUGE "\033[1;31m"
# define VERT "\033[1;32m"
# define JAUNE "\033[1;33m"
# define BLEU "\033[1;34m"
# define MAGENTA "\033[1;35m"
# define CYAN "\033[0;36m"
# define GRIS "\033[0;90m"
# define GRAS "\033[1m"
# define NC "\033[0m"

# define TAG_OK VERT "[OK]" NC
# define TAG_KO ROUGE "[KO]" NC

/* ========================================================================== */
/*                     MEMOIRE PARTAGEE ENTRE PERE ET FILS                    */
/* ========================================================================== */

# define JOURNAL_MAX 60000
# define MAX_AFFICHES 12

typedef struct s_bac
{
	unsigned long	tests;
	unsigned long	erreurs;
	unsigned long	affiches;
	unsigned long	total_tests;
	unsigned long	total_erreurs;
	int				ouvert;
	int				ligne;
	char			fichier[64];
	char			fonction[64];
	char			source[32];
	char			appel[512];
	char			liste_ko[4096];
	size_t			taille_journal;
	char			journal[JOURNAL_MAX];
}	t_bac;

extern t_bac	*g_bac;

/* Initialise la memoire partagee (a appeler une fois au debut du main). */
void		bac_init(void);
/* Vide le journal et les compteurs du groupe courant. */
void		bac_reset_groupe(const char *fonction, const char *source);
/* Ajoute du texte au journal (affiche plus tard, sous la ligne [OK]/[KO]). */
void		journal(const char *fmt, ...);
/* Ajoute "nom" a la liste des fonctions KO du bilan (sans doublon). */
void		bac_noter_ko(const char *nom);

/* ========================================================================== */
/*                              VERIFICATIONS                                 */
/* ========================================================================== */

/*
** essai() memorise l'appel en cours et sa ligne : si la fonction testee
** crashe, le pere affiche exactement quel appel a crashe et ou.
*/
void		essai(int ligne, const char *fmt, ...);
/* Comme essai(), mais seulement si le texte contient "ft_". */
void		essai_expr(int ligne, const char *texte);

/*
** verif() : coeur de toutes les verifications.
** ok          : resultat de la verification (0 = echec)
** explication : phrase en francais (peut etre NULL)
** fmt, ...    : details (attendu / obtenu). Peut contenir des '\n'.
** Renvoie ok.
*/
int			verif(int ligne, int ok, const char *explication,
				const char *fmt, ...);

int			v_int(int ligne, long long obtenu, long long attendu,
				const char *expl);
int			v_uint(int ligne, unsigned long long obtenu,
				unsigned long long attendu, const char *expl);
int			v_bool(int ligne, long long obtenu, long long attendu,
				const char *expl);
int			v_signe(int ligne, long long obtenu, long long attendu,
				const char *expl);
int			v_str(int ligne, const char *obtenu, const char *attendu,
				const char *expl);
int			v_ptr(int ligne, const void *obtenu, const void *attendu,
				const void *base, const char *expl);
int			v_mem(int ligne, const void *obtenu, const void *attendu,
				size_t n, const char *expl);

/*
** Macros a utiliser dans les tests. Si l'expression testee contient un
** appel ft_..., elle devient "l'appel en cours" (affiche en cas d'erreur).
** Exemple : V_INT(ft_strlen("abc"), 3, "ft_strlen doit compter 3");
*/
# define ESSAI(...) essai(__LINE__, __VA_ARGS__)
# define V_OK(c, e) (essai_expr(__LINE__, #c), \
	verif(__LINE__, !!(c), e, "condition non respectee : %s", #c))
# define V_INT(o, a, e) (essai_expr(__LINE__, #o), \
	v_int(__LINE__, (long long)(o), (long long)(a), e))
# define V_UINT(o, a, e) (essai_expr(__LINE__, #o), \
	v_uint(__LINE__, (unsigned long long)(o), (unsigned long long)(a), e))
# define V_BOOL(o, a, e) (essai_expr(__LINE__, #o), \
	v_bool(__LINE__, (long long)(o), (long long)(a), e))
# define V_SIGNE(o, a, e) (essai_expr(__LINE__, #o), \
	v_signe(__LINE__, (long long)(o), (long long)(a), e))
# define V_STR(o, a, e) (essai_expr(__LINE__, #o), \
	v_str(__LINE__, (o), (a), e))
# define V_PTR(o, a, base, e) (essai_expr(__LINE__, #o), \
	v_ptr(__LINE__, (const void *)(o), (const void *)(a), \
	(const void *)(base), e))
# define V_MEM(o, a, n, e) (essai_expr(__LINE__, #o), \
	v_mem(__LINE__, (o), (a), (n), e))

/* ========================================================================== */
/*                        AFFICHAGE / ECHAPPEMENT                             */
/* ========================================================================== */

/* Chaine entre guillemets, caracteres speciaux echappes, NULL -> NULL.
** Buffers tournants : jusqu'a 8 appels dans un meme printf. */
const char	*esc(const char *s);
/* Idem pour n octets bruts (peut contenir des '\0'). */
const char	*esc_n(const void *m, size_t n);
/* Un caractere : 'a', '\n', '\x80'... */
const char	*esc_c(int c);

/* ========================================================================== */
/*                       SUIVI DE MALLOC / FREE                               */
/* ========================================================================== */

/*
** Disponible seulement si compile avec -DTEST_WRAP_MALLOC et lie avec
** -Wl,--wrap=malloc -Wl,--wrap=free (Linux). Sinon g_malloc_dispo vaut 0
** et les macros T_* ne verifient que le contenu.
*/
extern const int	g_malloc_dispo;
extern int			g_trace_active;
extern size_t		g_total_alloc;
extern size_t		g_nombres_echecs;

/* Remet le suivi a zero et l'active. echec_au = numero du malloc qui doit
** renvoyer NULL (0 = aucun). Pendant le suivi, chaque bloc alloue est
** rempli de '#' pour detecter un '\0' final oublie. */
void		suivi_debut(size_t echec_au);
void		suivi_fin(void);
/* Nombre de blocs alloues pendant le suivi et pas encore liberes. */
size_t		suivi_vivantes(void);
/* Nombre d'appels a malloc pendant le suivi. */
size_t		suivi_nb_malloc(void);
/* Taille demandee a malloc pour p (-1 si p n'a pas ete suivi). */
long long	suivi_taille(const void *p);
/* Taille demandee au dernier appel de malloc (meme s'il a echoue). */
size_t		suivi_derniere_taille(void);
/* malloc/free qui ne passent pas par le suivi. */
void		*malloc_reel(size_t taille);
void		free_reel(void *p);

/* Noms historiques utilises par les tests de main.c */
# define preparer_injection(n) suivi_debut(n)
# define allocations_restantes() suivi_vivantes()

# ifdef TEST_WRAP_MALLOC

void		*__real_malloc(size_t taille);
void		__real_free(void *ptr);
# endif

/* Verifications utilisees par les macros T_* (ne pas appeler directement). */
void		t_alloc_str_verif(int ligne, char *res, const char *attendu,
				const char *expl);
void		t_protection_verif(int ligne, size_t i, size_t n, void *res);
void		t_taille_verif(int ligne, const void *p, size_t attendue,
				const char *quoi);
void		t_fuites_verif(int ligne, const char *quoi);

/*
** T_ALLOC_STR(appel, attendu, expl)
** Pour une fonction qui renvoie une chaine allouee (strdup, substr, ...) :
**  1. contenu identique a "attendu" ;
**  2. malloc de exactement strlen(attendu) + 1 octets ;
**  3. aucune fuite ;
**  4. protection : chaque malloc est force a NULL tour a tour, la fonction
**     doit alors renvoyer NULL sans rien laisser alloue.
*/
# define T_ALLOC_STR(appel, attendu, expl) do { \
	char *_r_; \
	essai(__LINE__, "%s", #appel); \
	suivi_debut(0); \
	_r_ = (appel); \
	suivi_fin(); \
	t_alloc_str_verif(__LINE__, _r_, (attendu), (expl)); \
	T_PROTECTION(appel, free); \
} while (0)

/*
** T_PROTECTION(appel, liberer)
** Force chaque malloc de "appel" a renvoyer NULL (un par un). "liberer" est
** une fonction qui prend le resultat (void *) et le libere.
*/
# define T_PROTECTION(appel, liberer) do { \
	size_t _n_; \
	size_t _i_; \
	void *_p_; \
	if (!g_malloc_dispo) \
		break ; \
	essai(__LINE__, "%s", #appel); \
	suivi_debut(0); \
	_p_ = (void *)(appel); \
	suivi_fin(); \
	_n_ = suivi_nb_malloc(); \
	if (_p_) \
		liberer(_p_); \
	for (_i_ = 1; _i_ <= _n_; _i_++) { \
		essai(__LINE__, "%s  [malloc numero %zu/%zu force a NULL]", \
			#appel, _i_, _n_); \
		suivi_debut(_i_); \
		_p_ = (void *)(appel); \
		suivi_fin(); \
		t_protection_verif(__LINE__, _i_, _n_, _p_); \
		if (_p_) \
			liberer(_p_); \
	} \
} while (0)

/* Verifie la taille exacte du bloc p (alloue pendant le dernier suivi). */
# define T_TAILLE(p, attendue, quoi) t_taille_verif(__LINE__, (p), \
	(attendue), (quoi))
/* Verifie qu'il ne reste aucun bloc suivi non libere. */
# define T_FUITES(quoi) t_fuites_verif(__LINE__, (quoi))

/* ========================================================================== */
/*                           MEMOIRE ELECTRIQUE                               */
/* ========================================================================== */

/* [p, p + taille) utilisable ; l'octet p[taille] est dans une page
** interdite : lire ou ecrire un octet de trop => SIGSEGV. */
void		*alloc_electrique(size_t taille);
/* Pareil mais la page interdite est juste AVANT p (p[-1] interdit). */
void		*alloc_electrique_avant(size_t taille);
/* Rend la zone [p, p + taille) lecture seule (ecrire dedans => SIGSEGV). */
void		lecture_seule(void *p, size_t taille);

/*
** sous_test : execute f() dans un processus fils. Si f() crashe (lecture
** hors buffer, ecriture interdite...) ou boucle, c'est une erreur notee a
** la ligne du test, avec l'explication. Renvoie 1 si pas de crash.
*/
int			sous_test(int ligne, void (*f)(void), const char *quoi,
				const char *expl);
# define T_SANS_CRASH(f, quoi, expl) sous_test(__LINE__, (f), (quoi), (expl))

/* ========================================================================== */
/*                          DESCRIPTEURS DE FICHIER                           */
/* ========================================================================== */

/* Fichier temporaire (deja supprime du disque), ouvert en lecture/ecriture. */
int			fd_temporaire(void);
/* Relit tout le fichier fd depuis le debut, compare aux n octets attendus,
** puis ferme fd. */
int			v_fd(int ligne, int fd, const char *attendu, size_t n,
				const char *expl);

/* L'appel doit utiliser la variable "fd". Ex :
** T_FD(ft_putnbr_fd(-42, fd), "-42", 3, "..."); */
# define T_FD(appel, attendu, n, expl) do { \
	int fd; \
	fd = fd_temporaire(); \
	essai(__LINE__, "%s", #appel); \
	appel; \
	v_fd(__LINE__, fd, (attendu), (n), (expl)); \
} while (0)

/* Redirige le VRAI descripteur num (1 = sortie, 2 = erreur) le temps de
** l'appel. Ex : T_FD_STD(2, ft_putstr_fd("abc", 2), "abc", 3, "..."); */
# define T_FD_STD(num, appel, attendu, n, expl) do { \
	int _tmp_; \
	int _sauv_; \
	_tmp_ = fd_temporaire(); \
	_sauv_ = dup(num); \
	dup2(_tmp_, (num)); \
	essai(__LINE__, "%s", #appel); \
	appel; \
	dup2(_sauv_, (num)); \
	close(_sauv_); \
	v_fd(__LINE__, _tmp_, (attendu), (n), (expl)); \
} while (0)

/* ========================================================================== */
/*                               LISTES                                       */
/* ========================================================================== */

/* Noeud cree avec malloc (sans ft_lstnew), next = NULL. */
t_list		*lst_noeud(void *content);
/* Liste de n noeuds dont les contenus sont les n arguments (non copies). */
t_list		*lst_creer(int n, ...);
/* Texte de la liste (contenus vus comme des chaines) : ["a", "b"] */
const char	*lst_texte(const t_list *l);
/* Libere les noeuds (pas les contenus). */
void		lst_liberer_noeuds(t_list *l);
/* Libere les noeuds ET les contenus (avec free). */
void		lst_liberer_tout(t_list *l);

/* ========================================================================== */
/*                       ISOLATION DES GROUPES DE TESTS                       */
/* ========================================================================== */

/* Delai (secondes) avant de declarer une boucle infinie. */
int			delai_test(void);
/*
** Execute f() dans un fils. Renvoie 0 si tout s'est bien passe, le numero
** du signal si le fils a crashe, SIGALRM si temps depasse, -1 si le fils
** s'est arrete avec exit() != 0.
*/
int			executer_isole(void (*f)(void), int delai);
/* Explication en francais d'un signal (SIGSEGV, SIGBUS, SIGABRT...). */
const char	*nom_signal(int sig);
const char	*explication_signal(int sig);
/* Ajoute au journal le compte rendu d'un crash (appel en cours, ligne). */
void		journal_crash(int sig);

/* ========================================================================== */
/*                     TESTS PORTES DEPUIS FRANCINETTE                        */
/* ========================================================================== */

typedef struct s_test_fr
{
	const char	*fonction;
	const char	*source;
	void		(*test)(void);
}	t_test_fr;

/* Tables terminees par {NULL, NULL, NULL} */
extern const t_test_fr	g_fr_part1[];
extern const t_test_fr	g_fr_part2[];
extern const t_test_fr	g_fr_bonus[];

/* ========================================================================== */
/*                               NORMINETTE                                   */
/* ========================================================================== */

/*
** Lance norminette sur les .c et .h du dossier "dossier" (hors test/),
** affiche [OK]/[KO] par fichier et, pour chaque erreur : ligne, colonne,
** code, message et explication en francais.
** Renvoie le nombre de fichiers KO, ou -1 si norminette est introuvable.
*/
int			norminette_verifier(const char *dossier, int *nb_fichiers);

/* ========================================================================== */
/*          NOM DU FICHIER DE TEST (ajoute automatiquement aux erreurs)       */
/* ========================================================================== */

/*
** Chaque appel aux fonctions ci-dessous memorise __FILE__ juste avant :
** les erreurs affichent donc "test/fr_part1.c:123" et pas seulement 123.
** (Une macro ne se re-developpe pas elle-meme : l'appel interne est bien
** celui de la vraie fonction.)
*/
extern const char	*g_test_fichier;

# ifndef TEST_FRAMEWORK
#  define essai(...) (g_test_fichier = __FILE__, essai(__VA_ARGS__))
#  define essai_expr(...) (g_test_fichier = __FILE__, essai_expr(__VA_ARGS__))
#  define verif(...) (g_test_fichier = __FILE__, verif(__VA_ARGS__))
#  define v_int(...) (g_test_fichier = __FILE__, v_int(__VA_ARGS__))
#  define v_uint(...) (g_test_fichier = __FILE__, v_uint(__VA_ARGS__))
#  define v_bool(...) (g_test_fichier = __FILE__, v_bool(__VA_ARGS__))
#  define v_signe(...) (g_test_fichier = __FILE__, v_signe(__VA_ARGS__))
#  define v_str(...) (g_test_fichier = __FILE__, v_str(__VA_ARGS__))
#  define v_ptr(...) (g_test_fichier = __FILE__, v_ptr(__VA_ARGS__))
#  define v_mem(...) (g_test_fichier = __FILE__, v_mem(__VA_ARGS__))
#  define v_fd(...) (g_test_fichier = __FILE__, v_fd(__VA_ARGS__))
#  define sous_test(...) (g_test_fichier = __FILE__, sous_test(__VA_ARGS__))
#  define t_alloc_str_verif(...) (g_test_fichier = __FILE__, \
	t_alloc_str_verif(__VA_ARGS__))
#  define t_protection_verif(...) (g_test_fichier = __FILE__, \
	t_protection_verif(__VA_ARGS__))
#  define t_taille_verif(...) (g_test_fichier = __FILE__, \
	t_taille_verif(__VA_ARGS__))
#  define t_fuites_verif(...) (g_test_fichier = __FILE__, \
	t_fuites_verif(__VA_ARGS__))
# endif

#endif
