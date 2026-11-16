/*
** fr_part2.c - tests portes depuis francinette pour la PARTIE 2 :
** ft_substr, ft_strjoin, ft_strtrim, ft_split, ft_itoa, ft_strmapi,
** ft_striteri, ft_putchar_fd, ft_putstr_fd, ft_putendl_fd, ft_putnbr_fd.
**
** Sources : fsoares (tests "maison" de francinette), Tripouille
** (libftTester), alelievr (libft-unit-test), war-machine.
** Les tests alelievr "*_null*" (crash attendu sur NULL) et "*_speed*"
** (benchmarks) ne sont pas portes : ils ne font pas partie du sujet.
*/
#include "test.h"

/* ========================================================================== */
/*                               EXPLICATIONS                                 */
/* ========================================================================== */

#define EX_SUBSTR "ft_substr doit renvoyer une nouvelle chaine allouee avec au \
plus len caracteres de s, a partir de l'indice start"
#define EX_SUBSTR_LOIN "quand start >= strlen(s), ft_substr doit renvoyer une \
chaine vide allouee (pas NULL, pas de lecture hors de s)"
#define EX_SUBSTR_TROP "quand len depasse la fin de s, ft_substr s'arrete au \
'\\0' de s et n'alloue que strlen(resultat) + 1 octets"
#define EX_STRJOIN "ft_strjoin doit renvoyer une nouvelle chaine allouee \
contenant s1 puis s2"
#define EX_STRJOIN_VIDE "ft_strjoin doit fonctionner quand s1 et/ou s2 sont \
des chaines vides"
#define EX_STRTRIM "ft_strtrim doit retirer les caracteres de set au debut \
et a la fin de s1 (jamais au milieu)"
#define EX_STRTRIM_VIDE "si s1 ne contient que des caracteres de set (ou est \
vide), ft_strtrim doit renvoyer une chaine vide allouee"
#define EX_STRTRIM_SET "si set est vide, ft_strtrim doit renvoyer une copie \
de s1"
#define EX_SPLIT "ft_split doit decouper s selon le separateur c, sans mot \
vide, et terminer le tableau par NULL"
#define EX_SPLIT_VIDE "si s est vide ou ne contient que des separateurs, \
ft_split doit renvoyer un tableau qui contient juste NULL"
#define EX_SPLIT_ZERO "avec c = '\\0', il n'y a aucun separateur : le \
resultat est s entier (ou un tableau vide si s est vide)"
#define EX_ITOA "ft_itoa doit renvoyer l'entier ecrit en base 10 dans une \
chaine allouee de la taille exacte (avec '-' si negatif)"
#define EX_ITOA_MIN "ft_itoa(INT_MIN) : -2147483648 ne tient pas en positif \
dans un int, il faut un long ou un traitement special"
#define EX_STRMAPI "ft_strmapi doit renvoyer une nouvelle chaine allouee ou \
chaque caractere vaut f(indice, s[indice])"
#define EX_STRITERI "ft_striteri doit appeler f(indice, &s[indice]) sur \
chaque caractere de s, dans l'ordre, en modifiant s sur place"
#define EX_PUTCHAR "ft_putchar_fd doit ecrire exactement le caractere c sur \
le descripteur fd (avec write)"
#define EX_PUTSTR "ft_putstr_fd doit ecrire tous les caracteres de s (jusqu'au \
'\\0' exclu) sur fd, sans rien ajouter"
#define EX_PUTENDL "ft_putendl_fd doit ecrire s puis UN seul '\\n' sur fd"
#define EX_PUTNBR "ft_putnbr_fd doit ecrire n en base 10 sur fd (avec '-' si \
negatif, INT_MIN compris)"
#define EX_FUITE_PUT "les fonctions put*_fd ne doivent laisser aucune \
memoire allouee"

#define AL_STRING_1 "the cake is a lie !\0I'm hidden lol\r\n"
#define AL_TRIM " \n\t"
#define WM_LOREM "lorem ipsum dolor sit amet, consectetur adipiscing elit. \
Sed non risus. Suspendisse lectus tortor, dignissim sit amet, adipiscing \
nec, ultricies sed, dolor. Cras elementum ultricies diam. Maecenas ligula \
massa, varius a, semper congue, euismod non, mi."

/* ========================================================================== */
/*                               OUTILS COMMUNS                               */
/* ========================================================================== */

/* Libere un tableau de chaines termine par NULL (resultat de ft_split). */
static void	liberer_tableau(void *p)
{
	char	**t;
	size_t	i;

	t = p;
	if (!t)
		return ;
	i = 0;
	while (t[i] && i < 100000)
		free(t[i++]);
	free(t);
}

/*
** Verifie un appel qui renvoie une chaine allouee, quand l'appel ne peut
** pas etre ecrit en dur (boucles, variables) : f() fait l'appel, desc le
** decrit. proteger = 1 => chaque malloc est aussi force a NULL.
*/
static void	alloc_cas(int ligne, char *(*f)(void), const char *desc,
	const char *attendu, const char *expl, int proteger)
{
	char	*r;
	size_t	n;
	size_t	i;

	essai(ligne, "%s", desc);
	suivi_debut(0);
	r = f();
	suivi_fin();
	t_alloc_str_verif(ligne, r, attendu, expl);
	if (!proteger || !g_malloc_dispo)
		return ;
	suivi_debut(0);
	r = f();
	suivi_fin();
	n = suivi_nb_malloc();
	free(r);
	i = 1;
	while (i <= n)
	{
		essai(ligne, "%s  [malloc numero %zu/%zu force a NULL]", desc, i, n);
		suivi_debut(i);
		r = f();
		suivi_fin();
		t_protection_verif(ligne, i, n, r);
		free(r);
		i++;
	}
}

/* Capture ce que f() ecrit sur le VRAI descripteur num (1 ou 2). */
static void	capture_std(int ligne, int num, void (*f)(void), const char *desc,
	const char *attendu, size_t n, const char *expl)
{
	int	tmp;
	int	sauv;

	tmp = fd_temporaire();
	sauv = dup(num);
	dup2(tmp, num);
	essai(ligne, "%s", desc);
	f();
	dup2(sauv, num);
	close(sauv);
	v_fd(ligne, tmp, attendu, n, expl);
}

/* Arguments partages avec les petites fonctions d'appel ci-dessous. */
static int			g_n;
static int			g_de;
static int			g_a;
static int			g_num;

static char	*appel_itoa(void)
{
	return (ft_itoa(g_n));
}

static void	itoa_cas(int ligne, int n, int proteger)
{
	char	attendu[32];
	char	desc[64];

	g_n = n;
	snprintf(attendu, sizeof(attendu), "%d", n);
	snprintf(desc, sizeof(desc), "ft_itoa(%d)", n);
	alloc_cas(ligne, appel_itoa, desc, attendu,
		(n == INT_MIN) ? EX_ITOA_MIN : EX_ITOA, proteger);
}

/* ft_putchar_fd(c, g_num) pour c de g_de a g_a */
static void	appel_putchar_plage(void)
{
	int	i;

	i = g_de;
	while (i <= g_a)
	{
		ft_putchar_fd(i, g_num);
		i++;
	}
}

static void	putchar_plage_cas(int ligne, int de, int a, int num)
{
	char	attendu[256];
	char	desc[96];
	int		i;

	i = de;
	while (i <= a)
	{
		attendu[i - de] = (char)i;
		i++;
	}
	g_de = de;
	g_a = a;
	g_num = num;
	snprintf(desc, sizeof(desc), "ft_putchar_fd(c, %d) pour c = %d .. %d",
		num, de, a);
	capture_std(ligne, num, appel_putchar_plage, desc, attendu,
		(size_t)(a - de + 1), EX_PUTCHAR);
}

static void	appel_putnbr(void)
{
	ft_putnbr_fd(g_n, g_num);
}

/* ft_putnbr_fd(n, num) : num = 1 ou 2 (vrai descripteur) ou 0 (fichier) */
static void	putnbr_cas(int ligne, int n, int num)
{
	char	attendu[32];
	char	desc[64];
	int		fd;

	snprintf(attendu, sizeof(attendu), "%d", n);
	if (num == 1 || num == 2)
	{
		g_n = n;
		g_num = num;
		snprintf(desc, sizeof(desc), "ft_putnbr_fd(%d, %d)", n, num);
		capture_std(ligne, num, appel_putnbr, desc, attendu, strlen(attendu),
			EX_PUTNBR);
		return ;
	}
	fd = fd_temporaire();
	essai(ligne, "ft_putnbr_fd(%d, fd)", n);
	ft_putnbr_fd(n, fd);
	v_fd(ligne, fd, attendu, strlen(attendu), EX_PUTNBR);
}

/*
** Verifie le resultat de ft_split(s, c) : chaque mot, le NULL final, la
** taille exacte du tableau et des mots, l'absence de fuite, puis la
** protection de chaque malloc.
*/
static void	split_cas(int ligne, const char *s, char c, const char **att,
	const char *expl)
{
	char	**r;
	size_t	n;
	size_t	i;
	size_t	nm;
	int		forme_ok;

	n = 0;
	while (att[n])
		n++;
	essai(ligne, "ft_split(%s, %s)", esc(s), esc_c(c));
	suivi_debut(0);
	r = ft_split(s, c);
	suivi_fin();
	if (!verif(ligne, r != NULL, expl,
			"attendu : un tableau de %zu mot(s) termine par NULL\n"
			"obtenu  : NULL", n))
		return ;
	forme_ok = 1;
	i = 0;
	while (i < n)
	{
		if (!verif(ligne, r[i] != NULL, expl,
				"attendu : mot[%zu] = %s\nobtenu  : NULL (le tableau "
				"s'arrete trop tot : %zu mot(s) au lieu de %zu)",
				i, esc(att[i]), i, n))
		{
			forme_ok = 0;
			break ;
		}
		v_str(ligne, r[i], att[i], expl);
		i++;
	}
	if (forme_ok && !verif(ligne, r[n] == NULL, expl,
			"attendu : NULL apres %zu mot(s)\nobtenu  : mot en trop %s",
			n, esc(r[n])))
		forme_ok = 0;
	if (forme_ok && g_malloc_dispo)
	{
		t_taille_verif(ligne, r, sizeof(char *) * (n + 1),
			"le tableau de ft_split (nombre de mots + 1 pointeurs)");
		i = 0;
		while (i < n)
		{
			t_taille_verif(ligne, r[i], strlen(att[i]) + 1,
				"un mot de ft_split (strlen(mot) + 1)");
			i++;
		}
	}
	liberer_tableau(r);
	if (!g_malloc_dispo)
		return ;
	t_fuites_verif(ligne, "ft_split");
	suivi_debut(0);
	r = ft_split(s, c);
	suivi_fin();
	nm = suivi_nb_malloc();
	liberer_tableau(r);
	i = 1;
	while (i <= nm)
	{
		essai(ligne, "ft_split(%s, %s)  [malloc numero %zu/%zu force a NULL]",
			esc(s), esc_c(c), i, nm);
		suivi_debut(i);
		r = ft_split(s, c);
		suivi_fin();
		t_protection_verif(ligne, i, nm, r);
		liberer_tableau(r);
		i++;
	}
}

#define SPLIT(s, c, e, ...) split_cas(__LINE__, (s), (c), \
	(const char *[]){__VA_ARGS__}, (e))

/* ========================================================================== */
/*                                 FT_SUBSTR                                  */
/* ========================================================================== */

static void	fs_substr(void)
{
	T_ALLOC_STR(ft_substr("", 0, 0), "", EX_SUBSTR);
	T_ALLOC_STR(ft_substr("", 0, 1), "", EX_SUBSTR_TROP);
	T_ALLOC_STR(ft_substr("", 1, 1), "", EX_SUBSTR_LOIN);
	T_ALLOC_STR(ft_substr("hola", UINT_MAX, 0), "", EX_SUBSTR_LOIN);
	T_ALLOC_STR(ft_substr("hola", 0, SIZE_MAX), "hola", EX_SUBSTR_TROP);
	T_ALLOC_STR(ft_substr("hola", UINT_MAX, SIZE_MAX), "", EX_SUBSTR_LOIN);
	T_ALLOC_STR(ft_substr("hola", 0, 0), "", EX_SUBSTR);
	T_ALLOC_STR(ft_substr("hola", 0, 1), "h", EX_SUBSTR);
	T_ALLOC_STR(ft_substr("hola", 0, 3), "hol", EX_SUBSTR);
	T_ALLOC_STR(ft_substr("hola", 0, 4), "hola", EX_SUBSTR);
	T_ALLOC_STR(ft_substr("hola", 0, 5), "hola", EX_SUBSTR_TROP);
	T_ALLOC_STR(ft_substr("hola", 2, 0), "", EX_SUBSTR);
	T_ALLOC_STR(ft_substr("hola", 2, 1), "l", EX_SUBSTR);
	T_ALLOC_STR(ft_substr("hola", 2, 2), "la", EX_SUBSTR);
	T_ALLOC_STR(ft_substr("hola", 2, 3), "la", EX_SUBSTR_TROP);
	T_ALLOC_STR(ft_substr("hola", 2, 30), "la", EX_SUBSTR_TROP);
	T_ALLOC_STR(ft_substr("hola", 3, 0), "", EX_SUBSTR);
	T_ALLOC_STR(ft_substr("hola", 3, 1), "a", EX_SUBSTR);
	T_ALLOC_STR(ft_substr("hola", 3, 2), "a", EX_SUBSTR_TROP);
	T_ALLOC_STR(ft_substr("hola", 4, 0), "", EX_SUBSTR_LOIN);
	T_ALLOC_STR(ft_substr("hola", 4, 1), "", EX_SUBSTR_LOIN);
	T_ALLOC_STR(ft_substr("hola", 4, 20), "", EX_SUBSTR_LOIN);
	T_ALLOC_STR(ft_substr("hola", 5, 2), "", EX_SUBSTR_LOIN);
}

static void	tr_substr(void)
{
	char	*str;

	T_ALLOC_STR(ft_substr("tripouille", 0, 42000), "tripouille",
		EX_SUBSTR_TROP);
	T_ALLOC_STR(ft_substr("tripouille", 1, 1), "r", EX_SUBSTR);
	T_ALLOC_STR(ft_substr("tripouille", 100, 1), "", EX_SUBSTR_LOIN);
	str = strdup("1");
	T_ALLOC_STR(ft_substr(str, 42, 42000000), "", EX_SUBSTR_LOIN);
	free(str);
	str = strdup("0123456789");
	T_ALLOC_STR(ft_substr(str, 9, 10), "9", EX_SUBSTR_TROP);
	free(str);
	T_ALLOC_STR(ft_substr("42", 0, 0), "", EX_SUBSTR);
	T_ALLOC_STR(ft_substr("BONJOUR LES HARICOTS !", 8, 14), "LES HARICOTS !",
		EX_SUBSTR);
	T_ALLOC_STR(ft_substr("test", 1, 2), "es", EX_SUBSTR);
}

static void	al_substr_electrique(void)
{
	char	*str;

	str = alloc_electrique(10);
	strcpy(str, "YOLO !!!!");
	free(ft_substr(str, 0, 10));
}

static void	al_substr(void)
{
	char	str[] = "i just want this part #############";

	T_ALLOC_STR(ft_substr(str, 0, 22), "i just want this part ", EX_SUBSTR);
	T_ALLOC_STR(ft_substr(str, 5, 20), "t want this part ###", EX_SUBSTR);
	T_ALLOC_STR(ft_substr("", 5, 0), "", EX_SUBSTR_LOIN);
	T_ALLOC_STR(ft_substr(str, 5, 10), "t want thi",
		"ft_substr doit allouer exactement len + 1 octets ici");
	T_ALLOC_STR(ft_substr("all of this !", 0, 13), "all of this !",
		"ft_substr doit fonctionner pour la chaine entiere");
	T_SANS_CRASH(al_substr_electrique, "ft_substr(\"YOLO !!!!\", 0, 10) "
		"sur un buffer de 10 octets suivi d'une page interdite",
		"ft_substr lit au-dela du '\\0' de s (il ne faut pas lire plus loin "
		"que la fin de la chaine)");
	T_ALLOC_STR(ft_substr("01234", 10, 10), "", EX_SUBSTR_LOIN);
}

static void	wm_substr(void)
{
	char	str[] = "lorem ipsum dolor sit amet";

	T_ALLOC_STR(ft_substr(str, 0, 10), "lorem ipsu", EX_SUBSTR);
	T_ALLOC_STR(ft_substr(str, 7, 10), "psum dolor", EX_SUBSTR);
	T_ALLOC_STR(ft_substr(str, 7, 0), "", EX_SUBSTR);
	T_ALLOC_STR(ft_substr(str, 0, 0), "", EX_SUBSTR);
	T_ALLOC_STR(ft_substr(str, 400, 20), "", EX_SUBSTR_LOIN);
}

/* ========================================================================== */
/*                                 FT_STRJOIN                                 */
/* ========================================================================== */

static void	fs_strjoin(void)
{
	T_ALLOC_STR(ft_strjoin("", ""), "", EX_STRJOIN_VIDE);
	T_ALLOC_STR(ft_strjoin("abc", ""), "abc", EX_STRJOIN_VIDE);
	T_ALLOC_STR(ft_strjoin("", "abc"), "abc", EX_STRJOIN_VIDE);
	T_ALLOC_STR(ft_strjoin("abcd", "efghi"), "abcdefghi", EX_STRJOIN);
}

static void	tr_strjoin(void)
{
	T_ALLOC_STR(ft_strjoin("tripouille", "42"), "tripouille42", EX_STRJOIN);
	T_ALLOC_STR(ft_strjoin("", "42"), "42", EX_STRJOIN_VIDE);
	T_ALLOC_STR(ft_strjoin("42", ""), "42", EX_STRJOIN_VIDE);
	T_ALLOC_STR(ft_strjoin("", ""), "", EX_STRJOIN_VIDE);
}

static void	al_strjoin(void)
{
	char	*tmp;
	char	*s1;
	char	*s2;

	tmp = ft_strjoin("my favorite animal is", " ");
	V_STR(tmp, "my favorite animal is ", EX_STRJOIN);
	if (tmp)
		T_ALLOC_STR(ft_strjoin(tmp, "the nyancat"),
			"my favorite animal is the nyancat",
			"ft_strjoin doit pouvoir recevoir le resultat d'un autre "
			"ft_strjoin");
	free(tmp);
	s1 = "my favorite animal is ";
	s2 = s1 + 20;
	T_ALLOC_STR(ft_strjoin(s2, s1), "s my favorite animal is ",
		"ft_strjoin doit marcher quand s1 et s2 se chevauchent en memoire");
	T_ALLOC_STR(ft_strjoin("where is my ", "malloc ???"),
		"where is my malloc ???",
		"ft_strjoin doit mettre un '\\0' a la fin (malloc ne remet pas a 0)");
}

static void	wm_strjoin(void)
{
	char	s1[] = "lorem ipsum";
	char	s2[] = "dolor sit amet";

	T_ALLOC_STR(ft_strjoin(s1, s2), "lorem ipsumdolor sit amet", EX_STRJOIN);
	s1[0] = '\0';
	T_ALLOC_STR(ft_strjoin(s1, s2), "dolor sit amet", EX_STRJOIN_VIDE);
	strcpy(s1, "lorem ipsum");
	s2[0] = '\0';
	T_ALLOC_STR(ft_strjoin(s1, s2), "lorem ipsum", EX_STRJOIN_VIDE);
	s1[0] = '\0';
	T_ALLOC_STR(ft_strjoin(s1, s2), "", EX_STRJOIN_VIDE);
}

/* ========================================================================== */
/*                                 FT_STRTRIM                                 */
/* ========================================================================== */

static void	fs_strtrim(void)
{
	T_ALLOC_STR(ft_strtrim("", ""), "", EX_STRTRIM_VIDE);
	T_ALLOC_STR(ft_strtrim("abcd", ""), "abcd", EX_STRTRIM_SET);
	T_ALLOC_STR(ft_strtrim("", "cdef"), "", EX_STRTRIM_VIDE);
	T_ALLOC_STR(ft_strtrim(" . abcd", " "), ". abcd", EX_STRTRIM);
	T_ALLOC_STR(ft_strtrim("ab cd  f    ", " "), "ab cd  f", EX_STRTRIM);
	T_ALLOC_STR(ft_strtrim("xxxz  test with x and z and x .  zx  xx z",
			"z x"), "test with x and z and x .", EX_STRTRIM);
	T_ALLOC_STR(ft_strtrim("   abxfg  ", "x"), "   abxfg  ", EX_STRTRIM);
	T_ALLOC_STR(ft_strtrim(".teste, bla ,.,.", ",."), "teste, bla ",
		EX_STRTRIM);
}

static void	tr_strtrim(void)
{
	T_ALLOC_STR(ft_strtrim("   xxxtripouille", " x"), "tripouille",
		EX_STRTRIM);
	T_ALLOC_STR(ft_strtrim("tripouille   xxx", " x"), "tripouille",
		EX_STRTRIM);
	T_ALLOC_STR(ft_strtrim("   xxxtripouille   xxx", " x"), "tripouille",
		EX_STRTRIM);
	T_ALLOC_STR(ft_strtrim("   xxx   xxx", " x"), "", EX_STRTRIM_VIDE);
	T_ALLOC_STR(ft_strtrim("", "123"), "", EX_STRTRIM_VIDE);
	T_ALLOC_STR(ft_strtrim("123", ""), "123", EX_STRTRIM_SET);
	T_ALLOC_STR(ft_strtrim("", ""), "", EX_STRTRIM_VIDE);
	T_ALLOC_STR(ft_strtrim("abcdba", "acb"), "d", EX_STRTRIM);
}

static void	al_strtrim(void)
{
	T_ALLOC_STR(ft_strtrim("   \t  \n\n \t\t  \n\n\nHello \t  Please\n Trim me "
			"!\n   \n \n \t\t\n  ", AL_TRIM), "Hello \t  Please\n Trim me !",
		EX_STRTRIM);
	T_ALLOC_STR(ft_strtrim("   \t  \n\n \t\t  \n\n\nHello \t  Please\n Trim me "
			"!", AL_TRIM), "Hello \t  Please\n Trim me !", EX_STRTRIM);
	T_ALLOC_STR(ft_strtrim("Hello \t  Please\n Trim me !", AL_TRIM),
		"Hello \t  Please\n Trim me !",
		"si rien n'est a retirer, ft_strtrim renvoie une copie de s1");
	T_ALLOC_STR(ft_strtrim("", AL_TRIM), "", EX_STRTRIM_VIDE);
	T_ALLOC_STR(ft_strtrim("  \t \t \n   \n\n\n\t", AL_TRIM), "",
		EX_STRTRIM_VIDE);
	T_ALLOC_STR(ft_strtrim("\t   \n\n\n  \n\n\t    Hello \t  Please\n Trim me !"
			"\t\t\t\n  \t\t\t\t  ", AL_TRIM), "Hello \t  Please\n Trim me !",
		"ft_strtrim doit allouer exactement strlen(resultat) + 1 octets");
}

static void	wm_strtrim(void)
{
	char	s1[] = "lorem \n ipsum \t dolor \n sit \t amet";
	char	s2[] = "lorem ipsum dolor sit amet";
	char	s3[] = " lorem ipsum dolor sit amet";
	char	s5[] = "          ";

	T_ALLOC_STR(ft_strtrim(s1, " "), "lorem \n ipsum \t dolor \n sit \t amet",
		EX_STRTRIM);
	T_ALLOC_STR(ft_strtrim(s2, "te"), "lorem ipsum dolor sit am", EX_STRTRIM);
	T_ALLOC_STR(ft_strtrim(s3, "l "), "orem ipsum dolor sit amet",
		EX_STRTRIM);
	T_ALLOC_STR(ft_strtrim(s2, "tel"), "orem ipsum dolor sit am", EX_STRTRIM);
	T_ALLOC_STR(ft_strtrim(s5, " "), "", EX_STRTRIM_VIDE);
}

/* ========================================================================== */
/*                                  FT_SPLIT                                  */
/* ========================================================================== */

static void	fs_split(void)
{
	SPLIT("hello!", ' ', EX_SPLIT, "hello!", NULL);
	SPLIT("xxxxxxxxhello!", 'x', EX_SPLIT, "hello!", NULL);
	SPLIT("hello!zzzzzzzz", 'z', EX_SPLIT, "hello!", NULL);
	SPLIT("\11\11\11\11hello!\11\11\11\11", '\11', EX_SPLIT, "hello!", NULL);
	SPLIT("", 'a', EX_SPLIT_VIDE, NULL);
	SPLIT("ggggggggggg", 'g', EX_SPLIT_VIDE, NULL);
	SPLIT("^^^1^^2a,^^^^3^^^^--h^^^^", '^', EX_SPLIT,
		"1", "2a,", "3", "--h", NULL);
	SPLIT("nonempty", '\0', EX_SPLIT_ZERO, "nonempty", NULL);
}

static void	tr_split_electrique(void)
{
	char	*s;

	s = alloc_electrique(1);
	s[0] = '\0';
	liberer_tableau(ft_split(s, 0));
}

static void	tr_split(void)
{
	char	*splitme;

	SPLIT("  tripouille  42  ", ' ', EX_SPLIT, "tripouille", "42", NULL);
	SPLIT("tripouille", 0, EX_SPLIT_ZERO, "tripouille", NULL);
	SPLIT("     ", ' ', EX_SPLIT_VIDE, NULL);
	SPLIT("", 0, EX_SPLIT_ZERO, NULL);
	T_SANS_CRASH(tr_split_electrique, "ft_split(\"\", 0) sur un buffer d'un "
		"seul octet suivi d'une page interdite",
		"ft_split lit au-dela du '\\0' de s (lecture invalide)");
	SPLIT("chinimala", ' ', EX_SPLIT, "chinimala", NULL);
	SPLIT("", ' ', EX_SPLIT_VIDE, NULL);
	splitme = strdup("Tripouille");
	SPLIT(splitme, ' ', EX_SPLIT, "Tripouille", NULL);
	free(splitme);
	splitme = strdup("Tripouille ");
	SPLIT(splitme, ' ', EX_SPLIT, "Tripouille", NULL);
	free(splitme);
	splitme = strdup(" Tripouille");
	SPLIT(splitme, ' ', EX_SPLIT, "Tripouille", NULL);
	free(splitme);
	splitme = strdup(" Tripouille ");
	SPLIT(splitme, ' ', EX_SPLIT, "Tripouille", NULL);
	free(splitme);
	splitme = strdup("--1-2--3---4----5-----42");
	SPLIT(splitme, '-', EX_SPLIT, "1", "2", "3", "4", "5", "42", NULL);
	free(splitme);
}

static void	al_split(void)
{
	SPLIT("      split       this for   me  !       ", ' ', EX_SPLIT,
		"split", "this", "for", "me", "!", NULL);
	SPLIT("                  ", ' ', EX_SPLIT_VIDE, NULL);
	SPLIT("                  olol", ' ', EX_SPLIT, "olol", NULL);
	SPLIT("olol                     ", ' ', EX_SPLIT, "olol", NULL);
	SPLIT("", '\65', EX_SPLIT_VIDE, NULL);
	SPLIT("0 0 0 0 0 0 0 0 0", ' ', EX_SPLIT,
		"0", "0", "0", "0", "0", "0", "0", "0", "0", NULL);
	SPLIT("split  ||this|for|me|||||!|", '|', EX_SPLIT,
		"split  ", "this", "for", "me", "!", NULL);
	SPLIT("\0aa\0bbb", '\0', EX_SPLIT_ZERO, NULL);
}

static void	wm_split(void)
{
	SPLIT("          ", ' ', EX_SPLIT_VIDE, NULL);
	SPLIT("lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed non "
		"risus. Suspendisse", ' ', EX_SPLIT,
		"lorem", "ipsum", "dolor", "sit", "amet,", "consectetur",
		"adipiscing", "elit.", "Sed", "non", "risus.", "Suspendisse", NULL);
	SPLIT("   lorem   ipsum dolor     sit amet, consectetur   adipiscing elit."
		" Sed non risus. Suspendisse   ", ' ', EX_SPLIT,
		"lorem", "ipsum", "dolor", "sit", "amet,", "consectetur",
		"adipiscing", "elit.", "Sed", "non", "risus.", "Suspendisse", NULL);
	SPLIT(WM_LOREM, 'i', EX_SPLIT,
		"lorem ", "psum dolor s", "t amet, consectetur ad", "p", "sc",
		"ng el", "t. Sed non r", "sus. Suspend", "sse lectus tortor, d",
		"gn", "ss", "m s", "t amet, ad", "p", "sc", "ng nec, ultr", "c",
		"es sed, dolor. Cras elementum ultr", "c", "es d", "am. Maecenas l",
		"gula massa, var", "us a, semper congue, eu", "smod non, m", ".",
		NULL);
	SPLIT(WM_LOREM, 'z', EX_SPLIT, WM_LOREM, NULL);
	SPLIT("", 'z', EX_SPLIT_VIDE, NULL);
}

/* ========================================================================== */
/*                                  FT_ITOA                                   */
/* ========================================================================== */

static void	fs_itoa(void)
{
	int	i;

	itoa_cas(__LINE__, 0, 1);
	itoa_cas(__LINE__, 1000034, 1);
	itoa_cas(__LINE__, -10004, 1);
	itoa_cas(__LINE__, INT_MAX, 1);
	itoa_cas(__LINE__, INT_MIN, 1);
	srand(42);
	i = 0;
	while (i < 1000)
	{
		itoa_cas(__LINE__, rand() - RAND_MAX / 2, 0);
		i++;
	}
}

static void	tr_itoa(void)
{
	itoa_cas(__LINE__, INT_MAX, 1);
	itoa_cas(__LINE__, INT_MIN, 1);
	itoa_cas(__LINE__, 0, 1);
	itoa_cas(__LINE__, 1, 1);
	itoa_cas(__LINE__, -1, 1);
	itoa_cas(__LINE__, 42, 1);
}

static void	al_itoa(void)
{
	int	i;

	itoa_cas(__LINE__, -623, 1);
	itoa_cas(__LINE__, 156, 1);
	itoa_cas(__LINE__, -0, 1);
	srand(42);
	i = 0;
	while (i < 1000)
	{
		itoa_cas(__LINE__, rand(), 0);
		i++;
	}
	itoa_cas(__LINE__, INT_MAX, 1);
	itoa_cas(__LINE__, -5859, 1);
	itoa_cas(__LINE__, 0, 1);
	itoa_cas(__LINE__, INT_MIN, 1);
	itoa_cas(__LINE__, -1234, 1);
}

static void	wm_itoa(void)
{
	itoa_cas(__LINE__, 0, 1);
	itoa_cas(__LINE__, 9, 1);
	itoa_cas(__LINE__, -9, 1);
	itoa_cas(__LINE__, 10, 1);
	itoa_cas(__LINE__, -10, 1);
	itoa_cas(__LINE__, 8124, 1);
	itoa_cas(__LINE__, -9874, 1);
	itoa_cas(__LINE__, 543000, 1);
	itoa_cas(__LINE__, -2147483647 - 1, 1);
	itoa_cas(__LINE__, 2147483647, 1);
}

/* ========================================================================== */
/*                                 FT_STRMAPI                                 */
/* ========================================================================== */

static char	fs_mapi_add(unsigned int i, char c)
{
	return ((char)(i + c));
}

static char	fs_mapi_zero(unsigned int i, char c)
{
	(void)i;
	(void)c;
	return ('0');
}

static void	fs_strmapi(void)
{
	T_ALLOC_STR(ft_strmapi("", fs_mapi_add), "", EX_STRMAPI);
	T_ALLOC_STR(ft_strmapi("abcd0 ", fs_mapi_add), "aceg4%", EX_STRMAPI);
	T_ALLOC_STR(ft_strmapi("abcdfsdfs", fs_mapi_zero), "000000000",
		EX_STRMAPI);
}

static char	tr_add_one(unsigned int i, char c)
{
	return ((char)(i + c));
}

static void	tr_strmapi(void)
{
	T_ALLOC_STR(ft_strmapi("1234", tr_add_one), "1357", EX_STRMAPI);
	T_ALLOC_STR(ft_strmapi("", tr_add_one), "", EX_STRMAPI);
}

static char	al_f_strmapi(unsigned int i, char c)
{
	return ((char)(c + i));
}

static void	al_strmapi(void)
{
	const char	*b;
	char		attendu[0xF0];
	size_t		i;

	b = "override this !";
	i = 0;
	while (b[i])
	{
		attendu[i] = al_f_strmapi((unsigned int)i, b[i]);
		i++;
	}
	attendu[i] = '\0';
	T_ALLOC_STR(ft_strmapi(b, al_f_strmapi), attendu, EX_STRMAPI);
	T_ALLOC_STR(ft_strmapi("override this !", al_f_strmapi), attendu,
		"ft_strmapi doit allouer strlen(s) + 1 octets et mettre le '\\0' "
		"final (malloc ne remet pas a 0)");
}

static int	g_wm_index[11];
static int	g_wm_mauvais;

static void	wm_noter_index(unsigned int i)
{
	if (i > 10 || g_wm_index[i] == 1)
		g_wm_mauvais++;
	else
		g_wm_index[i] = 1;
}

static char	wm_mapi(unsigned int i, char c)
{
	wm_noter_index(i);
	if (c >= 'a' && c <= 'z')
		return (c - 32);
	if (c >= 'A' && c <= 'Z')
		return (c + 32);
	return (c);
}

static int	wm_nb_index(void)
{
	int	i;
	int	n;

	i = 0;
	n = 0;
	while (i < 11)
		n += g_wm_index[i++];
	return (n);
}

static void	wm_strmapi(void)
{
	char	str[12];
	char	*r;

	strcpy(str, "LoReM iPsUm");
	memset(g_wm_index, 0, sizeof(g_wm_index));
	g_wm_mauvais = 0;
	ESSAI("ft_strmapi(\"LoReM iPsUm\", mapi)");
	suivi_debut(0);
	r = ft_strmapi(str, wm_mapi);
	suivi_fin();
	V_INT(g_wm_mauvais, 0, "f doit etre appelee une seule fois par indice, "
		"de 0 a strlen(s) - 1 (indice faux ou repete)");
	V_INT(wm_nb_index(), 11, "f doit etre appelee sur CHAQUE caractere de s");
	V_OK(r != str, "ft_strmapi doit renvoyer une NOUVELLE chaine, pas s");
	t_alloc_str_verif(__LINE__, r, "lOrEm IpSuM", EX_STRMAPI);
}

/* ========================================================================== */
/*                                FT_STRITERI                                 */
/* ========================================================================== */

static void	fs_iteri_add(unsigned int i, char *c)
{
	*c = (char)(i + *c);
}

static void	fs_iteri_zero(unsigned int i, char *c)
{
	*c = (char)(i - i + '0');
}

static void	fs_iteri_cas(int ligne, const char *src,
	void (*f)(unsigned int, char *), const char *nom, const char *attendu)
{
	char	buffer[20];

	strcpy(buffer, src);
	essai(ligne, "ft_striteri(%s, %s)", esc(src), nom);
	suivi_debut(0);
	ft_striteri(buffer, f);
	suivi_fin();
	v_str(ligne, buffer, attendu, EX_STRITERI);
	if (g_malloc_dispo)
		t_fuites_verif(ligne, "ft_striteri");
}

static void	fs_striteri(void)
{
	fs_iteri_cas(__LINE__, "", fs_iteri_add, "{(i, c) => i + c}", "");
	fs_iteri_cas(__LINE__, "abcd0 ", fs_iteri_add, "{(i, c) => i + c}",
		"aceg4%");
	fs_iteri_cas(__LINE__, "abcdfsdfs", fs_iteri_zero, "{(i, c) => '0'}",
		"000000000");
}

static void	tr_iter(unsigned int i, char *s)
{
	*s += i;
}

static void	tr_striteri(void)
{
	char	s1[] = "";
	char	s2[] = "0";
	char	s3[] = "0000000000";

	ft_striteri(s1, tr_iter);
	ESSAI("ft_striteri(\"\", iter)");
	V_STR(s1, "", EX_STRITERI);
	ESSAI("ft_striteri(\"0\", iter)");
	ft_striteri(s2, tr_iter);
	V_STR(s2, "0", EX_STRITERI);
	ESSAI("ft_striteri(\"0000000000\", iter)");
	ft_striteri(s3, tr_iter);
	V_STR(s3, "0123456789", EX_STRITERI);
}

static void	al_f_striteri(unsigned int i, char *s)
{
	*s = (char)(i + '0');
}

static void	al_striteri_electrique(void)
{
	char	*b;

	b = alloc_electrique(10);
	strcpy(b, "123456789");
	ft_striteri(b, al_f_striteri);
}

static void	al_striteri(void)
{
	char	b[] = "override this !";
	char	attendu[0xF0];
	size_t	i;
	size_t	taille;

	taille = strlen(b);
	i = 0;
	while (i < taille)
	{
		al_f_striteri((unsigned int)i, attendu + i);
		i++;
	}
	attendu[taille] = '\0';
	ESSAI("ft_striteri(\"override this !\", {(i, s) => *s = i + '0'})");
	ft_striteri(b, al_f_striteri);
	V_STR(b, attendu, EX_STRITERI);
	T_SANS_CRASH(al_striteri_electrique, "ft_striteri(\"123456789\", f) sur "
		"un buffer de 10 octets suivi d'une page interdite",
		"ft_striteri lit au-dela du '\\0' de s");
}

static void	wm_iter(unsigned int i, char *c)
{
	wm_noter_index(i);
	if (*c >= 'a' && *c <= 'z')
		*c = *c - 32;
	else if (*c >= 'A' && *c <= 'Z')
		*c = *c + 32;
}

static void	wm_striteri(void)
{
	char	str[12];

	strcpy(str, "LoReM iPsUm");
	memset(g_wm_index, 0, sizeof(g_wm_index));
	g_wm_mauvais = 0;
	ESSAI("ft_striteri(\"LoReM iPsUm\", iter)");
	ft_striteri(str, wm_iter);
	V_INT(g_wm_mauvais, 0, "f doit etre appelee une seule fois par indice, "
		"de 0 a strlen(s) - 1 (indice faux ou repete)");
	V_INT(wm_nb_index(), 11, "f doit etre appelee sur CHAQUE caractere de s");
	V_STR(str, "lOrEm IpSuM", EX_STRITERI);
}

/* ========================================================================== */
/*                               FT_PUTCHAR_FD                                */
/* ========================================================================== */

static void	fs_putchar_fd(void)
{
	int	fd;

	fd = fd_temporaire();
	ESSAI("ft_putchar_fd('a', fd); 'x'; 'y'; 'z'; '\\n' (meme fichier)");
	suivi_debut(0);
	ft_putchar_fd('a', fd);
	ft_putchar_fd('x', fd);
	ft_putchar_fd('y', fd);
	ft_putchar_fd('z', fd);
	ft_putchar_fd('\n', fd);
	suivi_fin();
	if (g_malloc_dispo)
		T_FUITES(EX_FUITE_PUT);
	v_fd(__LINE__, fd, "axyz\n", 5, EX_PUTCHAR);
}

static void	tr_putchar_fd(void)
{
	T_FD(ft_putchar_fd('a', fd), "a", 1, EX_PUTCHAR);
}

static void	al_putchar_fd(void)
{
	T_FD_STD(2, ft_putchar_fd('o', 2), "o", 1, EX_PUTCHAR);
	putchar_plage_cas(__LINE__, 0, 127, 2);
}

static void	wm_putchar_fd(void)
{
	putchar_plage_cas(__LINE__, 0, 47, 2);
	putchar_plage_cas(__LINE__, '0', '9', 1);
	putchar_plage_cas(__LINE__, 58, 64, 2);
	putchar_plage_cas(__LINE__, 'A', 'Z', 1);
	putchar_plage_cas(__LINE__, 91, 96, 2);
	putchar_plage_cas(__LINE__, 'a', 'z', 1);
	putchar_plage_cas(__LINE__, 123, 127, 2);
}

/* ========================================================================== */
/*                               FT_PUTSTR_FD                                 */
/* ========================================================================== */

#define AL_ASCII "string \x01 of \x63 non \x0a ascii \x12 chars\x1d"
#define AL_UNICODE "\xe3\x82\x88\xe3\x81\x8f\xe3\x82\x84\xe3\x81\xa3\xe3\x81\x9f"

static void	fs_putstr_fd(void)
{
	int	fd;

	fd = fd_temporaire();
	ESSAI("ft_putstr_fd(\"abcdef\", fd); \"\\n1234\"; \"\\t567\"; \"\"; "
		"\"\\nend!\" (meme fichier)");
	suivi_debut(0);
	ft_putstr_fd("abcdef", fd);
	ft_putstr_fd("\n1234", fd);
	ft_putstr_fd("\t567", fd);
	ft_putstr_fd("", fd);
	ft_putstr_fd("\nend!", fd);
	suivi_fin();
	if (g_malloc_dispo)
		T_FUITES(EX_FUITE_PUT);
	v_fd(__LINE__, fd, "abcdef\n1234\t567\nend!", 20, EX_PUTSTR);
}

static void	tr_putstr_fd(void)
{
	T_FD(ft_putstr_fd("42", fd), "42", 2, EX_PUTSTR);
}

static void	al_putstr_fd(void)
{
	T_FD_STD(2, ft_putstr_fd(AL_STRING_1, 2), "the cake is a lie !", 19,
		EX_PUTSTR);
	T_FD_STD(2, ft_putstr_fd(AL_ASCII, 2), AL_ASCII, strlen(AL_ASCII),
		"ft_putstr_fd doit ecrire aussi les caracteres non imprimables");
	T_FD_STD(2, ft_putstr_fd(AL_UNICODE, 2), AL_UNICODE, strlen(AL_UNICODE),
		"ft_putstr_fd doit ecrire les octets non ASCII (UTF-8) tels quels");
}

static void	wm_putstr_fd(void)
{
	T_FD_STD(2, ft_putstr_fd("lorem ipsum dolor sit amet", 2),
		"lorem ipsum dolor sit amet", 26, EX_PUTSTR);
	T_FD_STD(1, ft_putstr_fd("  lorem\nipsum\rdolor\tsit amet  ", 1),
		"  lorem\nipsum\rdolor\tsit amet  ", 30, EX_PUTSTR);
	T_FD_STD(2, ft_putstr_fd("", 2), "", 0, EX_PUTSTR);
	T_FD_STD(1, ft_putstr_fd("lorem ipsum do\0lor sit amet", 1),
		"lorem ipsum do", 14, "ft_putstr_fd doit s'arreter au premier '\\0'");
}

/* ========================================================================== */
/*                               FT_PUTENDL_FD                                */
/* ========================================================================== */

static void	fs_putendl_fd(void)
{
	int	fd;

	fd = fd_temporaire();
	ESSAI("ft_putendl_fd(\"\", fd); \"abcdef\"; \"1234\"; \"567\"; \"end!\" "
		"(meme fichier)");
	suivi_debut(0);
	ft_putendl_fd("", fd);
	ft_putendl_fd("abcdef", fd);
	ft_putendl_fd("1234", fd);
	ft_putendl_fd("567", fd);
	ft_putendl_fd("end!", fd);
	suivi_fin();
	if (g_malloc_dispo)
		T_FUITES(EX_FUITE_PUT);
	v_fd(__LINE__, fd, "\nabcdef\n1234\n567\nend!\n",
		strlen("\nabcdef\n1234\n567\nend!\n"), EX_PUTENDL);
}

static void	tr_putendl_fd(void)
{
	T_FD(ft_putendl_fd("42", fd), "42\n", 3, EX_PUTENDL);
}

static void	al_putendl_fd(void)
{
	T_FD_STD(2, ft_putendl_fd(AL_STRING_1, 2), "the cake is a lie !\n", 20,
		EX_PUTENDL);
	T_FD_STD(2, ft_putendl_fd(AL_ASCII, 2), AL_ASCII "\n",
		strlen(AL_ASCII) + 1,
		"ft_putendl_fd doit ecrire aussi les caracteres non imprimables");
	T_FD_STD(2, ft_putendl_fd(AL_UNICODE, 2), AL_UNICODE "\n",
		strlen(AL_UNICODE) + 1,
		"ft_putendl_fd doit ecrire les octets non ASCII (UTF-8) tels quels");
}

static void	wm_putendl_fd(void)
{
	T_FD_STD(2, ft_putendl_fd("lorem ipsum dolor sit amet", 2),
		"lorem ipsum dolor sit amet\n", 27, EX_PUTENDL);
	T_FD_STD(1, ft_putendl_fd("  lorem\nipsum\rdolor\tsit amet  ", 1),
		"  lorem\nipsum\rdolor\tsit amet  \n", 31, EX_PUTENDL);
	T_FD_STD(2, ft_putendl_fd("", 2), "\n", 1, EX_PUTENDL);
	T_FD_STD(1, ft_putendl_fd("lorem ipsum do\0lor sit amet", 1),
		"lorem ipsum do\n", 15,
		"ft_putendl_fd doit s'arreter au premier '\\0' puis ecrire '\\n'");
}

/* ========================================================================== */
/*                               FT_PUTNBR_FD                                 */
/* ========================================================================== */

static void	fs_putnbr_fd(void)
{
	int		fd;
	char	attendu[100];

	fd = fd_temporaire();
	ESSAI("ft_putnbr_fd(n, fd) + ft_putendl_fd(\"\", fd) pour n = 0, "
		"10000043, -10000043, INT_MAX, INT_MIN (meme fichier)");
	suivi_debut(0);
	ft_putnbr_fd(0, fd);
	ft_putendl_fd("", fd);
	ft_putnbr_fd(10000043, fd);
	ft_putendl_fd("", fd);
	ft_putnbr_fd(-10000043, fd);
	ft_putendl_fd("", fd);
	ft_putnbr_fd(INT_MAX, fd);
	ft_putendl_fd("", fd);
	ft_putnbr_fd(INT_MIN, fd);
	ft_putendl_fd("", fd);
	suivi_fin();
	if (g_malloc_dispo)
		T_FUITES(EX_FUITE_PUT);
	snprintf(attendu, sizeof(attendu), "0\n10000043\n-10000043\n%d\n%d\n",
		INT_MAX, INT_MIN);
	v_fd(__LINE__, fd, attendu, strlen(attendu), EX_PUTNBR);
}

static void	tr_putnbr_fd(void)
{
	putnbr_cas(__LINE__, 0, 0);
	putnbr_cas(__LINE__, 10, 0);
	putnbr_cas(__LINE__, INT_MAX, 0);
	putnbr_cas(__LINE__, INT_MIN, 0);
	putnbr_cas(__LINE__, -42, 0);
}

static void	al_putnbr_fd(void)
{
	int		i;
	int		fd;

	putnbr_cas(__LINE__, 0, 2);
	putnbr_cas(__LINE__, INT_MAX, 2);
	putnbr_cas(__LINE__, INT_MIN, 2);
	srand(42);
	i = 0;
	while (i < 4)
	{
		putnbr_cas(__LINE__, rand(), 2);
		i++;
	}
	if (!g_malloc_dispo)
		return ;
	fd = fd_temporaire();
	ESSAI("ft_putnbr_fd(INT_MIN, fd) avec malloc force a NULL");
	suivi_debut(1);
	ft_putnbr_fd(INT_MIN, fd);
	suivi_fin();
	V_INT(suivi_nb_malloc(), 0, "ft_putnbr_fd ne doit pas utiliser malloc "
		"(pas de ft_itoa) : ecris les chiffres directement");
	v_fd(__LINE__, fd, "-2147483648", 11, "ft_putnbr_fd doit marcher meme "
		"si malloc echoue (il n'a pas besoin d'allouer)");
}

static void	wm_putnbr_fd(void)
{
	putnbr_cas(__LINE__, 0, 2);
	putnbr_cas(__LINE__, 5, 1);
	putnbr_cas(__LINE__, -5, 2);
	putnbr_cas(__LINE__, 42, 1);
	putnbr_cas(__LINE__, -57, 2);
	putnbr_cas(__LINE__, 164189, 1);
	putnbr_cas(__LINE__, -987441, 2);
	putnbr_cas(__LINE__, 2147483647, 1);
	putnbr_cas(__LINE__, -2147483647 - 1, 2);
}

/* ========================================================================== */
/*                                   TABLE                                    */
/* ========================================================================== */

const t_test_fr	g_fr_part2[] = {
	{"ft_substr", "fsoares", fs_substr},
	{"ft_substr", "Tripouille", tr_substr},
	{"ft_substr", "alelievr", al_substr},
	{"ft_substr", "war-machine", wm_substr},
	{"ft_strjoin", "fsoares", fs_strjoin},
	{"ft_strjoin", "Tripouille", tr_strjoin},
	{"ft_strjoin", "alelievr", al_strjoin},
	{"ft_strjoin", "war-machine", wm_strjoin},
	{"ft_strtrim", "fsoares", fs_strtrim},
	{"ft_strtrim", "Tripouille", tr_strtrim},
	{"ft_strtrim", "alelievr", al_strtrim},
	{"ft_strtrim", "war-machine", wm_strtrim},
	{"ft_split", "fsoares", fs_split},
	{"ft_split", "Tripouille", tr_split},
	{"ft_split", "alelievr", al_split},
	{"ft_split", "war-machine", wm_split},
	{"ft_itoa", "fsoares", fs_itoa},
	{"ft_itoa", "Tripouille", tr_itoa},
	{"ft_itoa", "alelievr", al_itoa},
	{"ft_itoa", "war-machine", wm_itoa},
	{"ft_strmapi", "fsoares", fs_strmapi},
	{"ft_strmapi", "Tripouille", tr_strmapi},
	{"ft_strmapi", "alelievr", al_strmapi},
	{"ft_strmapi", "war-machine", wm_strmapi},
	{"ft_striteri", "fsoares", fs_striteri},
	{"ft_striteri", "Tripouille", tr_striteri},
	{"ft_striteri", "alelievr", al_striteri},
	{"ft_striteri", "war-machine", wm_striteri},
	{"ft_putchar_fd", "fsoares", fs_putchar_fd},
	{"ft_putchar_fd", "Tripouille", tr_putchar_fd},
	{"ft_putchar_fd", "alelievr", al_putchar_fd},
	{"ft_putchar_fd", "war-machine", wm_putchar_fd},
	{"ft_putstr_fd", "fsoares", fs_putstr_fd},
	{"ft_putstr_fd", "Tripouille", tr_putstr_fd},
	{"ft_putstr_fd", "alelievr", al_putstr_fd},
	{"ft_putstr_fd", "war-machine", wm_putstr_fd},
	{"ft_putendl_fd", "fsoares", fs_putendl_fd},
	{"ft_putendl_fd", "Tripouille", tr_putendl_fd},
	{"ft_putendl_fd", "alelievr", al_putendl_fd},
	{"ft_putendl_fd", "war-machine", wm_putendl_fd},
	{"ft_putnbr_fd", "fsoares", fs_putnbr_fd},
	{"ft_putnbr_fd", "Tripouille", tr_putnbr_fd},
	{"ft_putnbr_fd", "alelievr", al_putnbr_fd},
	{"ft_putnbr_fd", "war-machine", wm_putnbr_fd},
	{NULL, NULL, NULL}
};
