/*
** fr_part1.c - Tests de la PARTIE 1 portes depuis francinette :
**   fsoares     (francinette, "my own tests")
**   Tripouille  (libftTester)
**   alelievr    (libft-unit-test)   : sans les tests *_null / *_speed
**   war-machine (libft-war-machine) : sorties attendues des .output
**
** Chaque groupe (fonction, source) est execute dans un processus fils :
** un crash est rapporte avec la ligne et l'appel en cours.
** Les tests n'ecrivent rien sur la sortie standard.
*/
#include "test.h"

#define REP 1000
#define MEM_SIZE 0x100
#define BSIZE 0xF00
#define BFSIZE 0xF0000

#define STRING_1 "the cake is a lie !\0I'm hidden lol\r\n"
#define STRING_2 "there is no stars in the sky"
#define STRING_3 "test basic !"
#define STRING_4 "phrase differente pour le test"

/* ========================================================================== */
/*                              EXPLICATIONS                                  */
/* ========================================================================== */

#define EX_ISALPHA "ft_isalpha doit renvoyer non-zero pour 'a'-'z' et 'A'-'Z' \
uniquement, 0 pour tout le reste (chiffres, 128-255, EOF)"
#define EX_ISDIGIT "ft_isdigit doit renvoyer non-zero pour '0'-'9' uniquement"
#define EX_ISALNUM "ft_isalnum doit renvoyer non-zero pour les lettres \
('a'-'z', 'A'-'Z') et les chiffres ('0'-'9') uniquement"
#define EX_ISASCII "ft_isascii doit renvoyer non-zero pour 0 a 127 \
uniquement (0 pour -1 et pour 128 et plus)"
#define EX_ISPRINT "ft_isprint doit renvoyer non-zero pour 32 (' ') a 126 \
('~') uniquement : 0-31 et 127 ne sont pas imprimables"
#define EX_TOUPPER "ft_toupper ne doit transformer que 'a'-'z' en 'A'-'Z' \
et renvoyer c inchange sinon (y compris EOF et 128-255)"
#define EX_TOLOWER "ft_tolower ne doit transformer que 'A'-'Z' en 'a'-'z' \
et renvoyer c inchange sinon (y compris EOF et 128-255)"
#define EX_STRLEN "ft_strlen doit compter les octets avant le premier '\\0'"
#define EX_MEMSET "ft_memset doit ecrire (unsigned char)c dans les len \
premiers octets, et ne rien toucher apres"
#define EX_MEMSET_RET "ft_memset doit renvoyer le pointeur b recu en parametre"
#define EX_BZERO "ft_bzero doit mettre a 0 exactement les n premiers octets \
(rien du tout si n = 0)"
#define EX_MEMCPY "ft_memcpy doit copier exactement n octets de src vers \
dest (y compris les '\\0'), sans toucher au reste"
#define EX_MEMCPY_RET "ft_memcpy doit renvoyer dest"
#define EX_MEMMOVE "ft_memmove doit copier n octets correctement MEME si \
src et dest se chevauchent (copier de la fin vers le debut si dest > src)"
#define EX_MEMMOVE_RET "ft_memmove doit renvoyer dest"
#define EX_MEMCHR "ft_memchr doit chercher (unsigned char)c dans les n \
premiers octets, sans s'arreter au '\\0', et renvoyer son adresse ou NULL"
#define EX_MEMCMP "ft_memcmp doit comparer n octets en unsigned char (sans \
s'arreter au '\\0') : < 0, 0 ou > 0 selon la premiere difference"
#define EX_STRLCPY "ft_strlcpy doit copier au plus size - 1 caracteres, \
toujours finir par '\\0' si size > 0 et renvoyer strlen(src)"
#define EX_STRLCPY_RET "ft_strlcpy doit renvoyer strlen(src), meme si la \
copie est tronquee"
#define EX_STRLCAT "ft_strlcat doit ajouter src a la fin de dst sans que le \
total depasse size - 1 caracteres, puis mettre un '\\0'"
#define EX_STRLCAT_RET "ft_strlcat doit renvoyer strlen(dst initial) + \
strlen(src), ou size + strlen(src) si size <= strlen(dst)"
#define EX_STRCHR "ft_strchr doit renvoyer la PREMIERE occurrence de (char)c, \
l'adresse du '\\0' final si c == '\\0', ou NULL"
#define EX_STRRCHR "ft_strrchr doit renvoyer la DERNIERE occurrence de \
(char)c, l'adresse du '\\0' final si c == '\\0', ou NULL"
#define EX_STRNCMP "ft_strncmp doit comparer au plus n caracteres, en \
unsigned char, et s'arreter au premier '\\0'"
#define EX_STRNSTR "ft_strnstr doit trouver little EN ENTIER dans les len \
premiers caracteres de big (renvoyer big si little est vide, sinon NULL)"
#define EX_ATOI "ft_atoi doit sauter les espaces (' ' \\t \\n \\v \\f \\r), \
accepter UN seul signe '+' ou '-', puis lire les chiffres jusqu'au premier \
non-chiffre"
#define EX_CALLOC "ft_calloc doit allouer nmemb * size octets, tous mis a 0"
#define EX_CALLOC_NULL "ft_calloc ne doit pas renvoyer NULL quand malloc \
reussit"
#define EX_STRDUP "ft_strdup doit renvoyer une copie allouee \
(malloc(strlen(s) + 1)) terminee par '\\0'"

#define EX_ELEC_LIT "ta fonction lit au-dela de la fin du buffer (l'octet \
qui suit est dans une page interdite) ou ecrit dans une zone en lecture \
seule"
#define EX_ELEC_ECRIT "ta fonction ecrit au-dela des n octets demandes \
(l'octet qui suit est dans une page interdite)"

/* ========================================================================== */
/*                       REFERENCES ET OUTILS LOCAUX                          */
/* ========================================================================== */

typedef int		(*t_fct_int)(int);

static volatile size_t	g_puits;

static size_t	ref_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	len;
	size_t	i;

	len = strlen(src);
	if (size == 0)
		return (len);
	i = 0;
	while (i + 1 < size && src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (len);
}

static size_t	ref_strlcat(char *dst, const char *src, size_t size)
{
	size_t	d;
	size_t	s;
	size_t	i;

	d = 0;
	while (d < size && dst[d])
		d++;
	s = strlen(src);
	if (d == size)
		return (size + s);
	i = 0;
	while (src[i] && d + i + 1 < size)
	{
		dst[d + i] = src[i];
		i++;
	}
	dst[d + i] = '\0';
	return (d + s);
}

/* strnstr de FreeBSD / macOS (reference utilisee par fsoares/alelievr) */
static char	*ref_strnstr(const char *s, const char *find, size_t slen)
{
	char	c;
	char	sc;
	size_t	len;

	c = *find++;
	if (c != '\0')
	{
		len = strlen(find);
		do
		{
			do
			{
				if (slen-- < 1)
					return (NULL);
				sc = *s++;
				if (sc == '\0')
					return (NULL);
			} while (sc != c);
			if (len > slen)
				return (NULL);
		} while (strncmp(s, find, len) != 0);
		s--;
	}
	return ((char *)s);
}

static int	ref_memcmp(const void *a, const void *b, size_t n)
{
	const unsigned char	*s1;
	const unsigned char	*s2;
	size_t				i;

	s1 = a;
	s2 = b;
	i = 0;
	while (i < n)
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
		i++;
	}
	return (0);
}

static int	ref_isalpha(int c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

static int	ref_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

static int	ref_isascii(int c)
{
	return (c >= 0 && c <= 127);
}

static int	ref_isprint(int c)
{
	return (c >= 32 && c <= 126);
}

static int	ref_isascii_libc(int c)
{
	return (isascii(c));
}

/* fsoares : chaine aleatoire de caracteres imprimables */
static char	*rand_str(char *dest, int len)
{
	int	i;

	dest[len - 1] = '\0';
	i = 0;
	while (i < len - 1)
	{
		dest[i] = (char)(rand() % ('~' - ' ') + ' ');
		i++;
	}
	return (dest);
}

/* fsoares : octets aleatoires */
static char	*rand_bytes(char *dest, int len)
{
	int	i;

	dest[len - 1] = '\0';
	i = 0;
	while (i < len - 1)
	{
		dest[i] = (char)(rand() % 0x100);
		i++;
	}
	return (dest);
}

/* fsoares reset / reset_with */
static void	reset(void *m1, void *m2, size_t size)
{
	memset(m1, 0x11, size);
	memset(m2, 0x11, size);
}

static void	reset_with(char *m1, char *m2, const char *content, size_t size)
{
	reset(m1, m2, size);
	strcpy(m1, content);
	strcpy(m2, content);
}

/*
** motif("0*32 1*16") -> 32 fois '0' puis 16 fois '1'.
** Utilise pour ecrire les sorties attendues de war-machine.
*/
static const char	*motif(const char *spec)
{
	static char	bufs[4][700];
	static int	k;
	char		*out;
	size_t		pos;
	char		c;
	long		n;
	char		*fin;

	out = bufs[k++ % 4];
	pos = 0;
	while (*spec)
	{
		c = *spec++;
		n = 1;
		if (*spec == '*')
		{
			n = strtol(spec + 1, &fin, 10);
			spec = fin;
		}
		while (n-- > 0 && pos < sizeof(bufs[0]) - 1)
			out[pos++] = c;
		while (*spec == ' ')
			spec++;
	}
	out[pos] = '\0';
	return (out);
}

/* Compare !ft(c) et !ref(c) pour c dans [debut, fin] (+ EOF si eof). */
static void	ctype_bool(int ligne, const char *nom, t_fct_int ft,
		t_fct_int ref, int debut, int fin, int eof, const char *ex)
{
	int	c;
	int	r;

	c = debut;
	while (c <= fin)
	{
		essai(ligne, "%s(%s)", nom, esc_c(c));
		r = ft(c);
		v_bool(ligne, r, ref(c), ex);
		c++;
	}
	if (eof)
	{
		essai(ligne, "%s(EOF)", nom);
		r = ft(EOF);
		v_bool(ligne, r, ref(EOF), ex);
	}
}

/* Compare ft(c) == ref(c) (toupper / tolower). */
static void	ctype_val(int ligne, const char *nom, t_fct_int ft,
		t_fct_int ref, int debut, int fin, int eof, const char *ex)
{
	int	c;
	int	r;

	c = debut;
	while (c <= fin)
	{
		essai(ligne, "%s(%s)", nom, esc_c(c));
		r = ft(c);
		v_int(ligne, r, ref(c), ex);
		c++;
	}
	if (eof)
	{
		essai(ligne, "%s(EOF)", nom);
		r = ft(EOF);
		v_int(ligne, r, ref(EOF), ex);
	}
}

/* war-machine : for (i = debut; i <= fin; i++) ft_print_result(f(i)) */
static void	wm_bool_range(int ligne, const char *nom, t_fct_int ft,
		int debut, int fin, const char *attendu, const char *ex)
{
	char	obtenu[300];
	int		i;

	essai(ligne, "for (i = %d; i <= %d; i++) write(%s(i) ? \"1\" : \"0\")",
		debut, fin, nom);
	i = debut;
	while (i <= fin)
	{
		obtenu[i - debut] = ft(i) ? '1' : '0';
		i++;
	}
	obtenu[fin - debut + 1] = '\0';
	v_str(ligne, obtenu, attendu, ex);
}

/* war-machine : for (i = debut; i <= fin; i++) { c = f(i); write(c) }
** attendu == NULL : chaque caractere doit rester inchange. */
static void	wm_char_range(int ligne, const char *nom, t_fct_int ft,
		int debut, int fin, const char *attendu, const char *ex)
{
	char	obtenu[300];
	char	ref[300];
	int		i;

	essai(ligne, "for (i = %d; i <= %d; i++) write(%s(i))", debut, fin, nom);
	i = debut;
	while (i <= fin)
	{
		obtenu[i - debut] = (char)ft(i);
		ref[i - debut] = attendu ? attendu[i - debut] : (char)i;
		i++;
	}
	v_mem(ligne, obtenu, ref, (size_t)(fin - debut + 1), ex);
}

#define CTYPE_BOOL(f, ref, d, fin, eof, ex) \
	ctype_bool(__LINE__, #f, f, ref, d, fin, eof, ex)
#define CTYPE_VAL(f, ref, d, fin, eof, ex) \
	ctype_val(__LINE__, #f, f, ref, d, fin, eof, ex)
#define WM_BOOL(f, d, fin, spec, ex) \
	wm_bool_range(__LINE__, #f, f, d, fin, motif(spec), ex)
#define WM_CHAR(f, d, fin, att, ex) \
	wm_char_range(__LINE__, #f, f, d, fin, att, ex)
/* Note l'appel (texte + ligne) avant de l'executer : en cas d'erreur ou de
** crash, c'est lui qui est affiche. */
#define CALL(x) (essai(__LINE__, "%s", #x), (x))

/* ========================================================================== */
/*                       ft_isalpha ... ft_tolower                            */
/* ========================================================================== */

/* ---------- fsoares : 0..255 + EOF compares a la libc ---------- */

static void	fs_isalpha(void)
{
	CTYPE_BOOL(ft_isalpha, isalpha, 0, 0xff, 1, EX_ISALPHA);
}

static void	fs_isdigit(void)
{
	CTYPE_BOOL(ft_isdigit, isdigit, 0, 0xff, 1, EX_ISDIGIT);
}

static void	fs_isalnum(void)
{
	CTYPE_BOOL(ft_isalnum, isalnum, 0, 0xff, 1, EX_ISALNUM);
}

static void	fs_isascii(void)
{
	CTYPE_BOOL(ft_isascii, ref_isascii_libc, 0, 0xff, 1, EX_ISASCII);
}

static void	fs_isprint(void)
{
	CTYPE_BOOL(ft_isprint, isprint, 0, 0xff, 1, EX_ISPRINT);
}

static void	fs_toupper(void)
{
	CTYPE_VAL(ft_toupper, toupper, 0, 0xff, 1, EX_TOUPPER);
}

static void	fs_tolower(void)
{
	CTYPE_VAL(ft_tolower, tolower, 0, 0xff, 1, EX_TOLOWER);
}

/* ---------- Tripouille : bornes de chaque intervalle ---------- */

static void	tr_isalpha(void)
{
	V_BOOL(ft_isalpha('a' - 1), 0, EX_ISALPHA);
	V_BOOL(ft_isalpha('a'), 1, EX_ISALPHA);
	V_BOOL(ft_isalpha('z' + 1), 0, EX_ISALPHA);
	V_BOOL(ft_isalpha('z'), 1, EX_ISALPHA);
	V_BOOL(ft_isalpha('A' - 1), 0, EX_ISALPHA);
	V_BOOL(ft_isalpha('A'), 1, EX_ISALPHA);
	V_BOOL(ft_isalpha('Z' + 1), 0, EX_ISALPHA);
	V_BOOL(ft_isalpha('Z'), 1, EX_ISALPHA);
}

static void	tr_isdigit(void)
{
	V_BOOL(ft_isdigit('0' - 1), 0, EX_ISDIGIT);
	V_BOOL(ft_isdigit('0'), 1, EX_ISDIGIT);
	V_BOOL(ft_isdigit('9' + 1), 0, EX_ISDIGIT);
	V_BOOL(ft_isdigit('9'), 1, EX_ISDIGIT);
}

static void	tr_isalnum(void)
{
	V_BOOL(ft_isalnum('a' - 1), 0, EX_ISALNUM);
	V_BOOL(ft_isalnum('a'), 1, EX_ISALNUM);
	V_BOOL(ft_isalnum('z' + 1), 0, EX_ISALNUM);
	V_BOOL(ft_isalnum('z'), 1, EX_ISALNUM);
	V_BOOL(ft_isalnum('A' - 1), 0, EX_ISALNUM);
	V_BOOL(ft_isalnum('A'), 1, EX_ISALNUM);
	V_BOOL(ft_isalnum('Z' + 1), 0, EX_ISALNUM);
	V_BOOL(ft_isalnum('Z'), 1, EX_ISALNUM);
	V_BOOL(ft_isalnum('0' - 1), 0, EX_ISALNUM);
	V_BOOL(ft_isalnum('0'), 1, EX_ISALNUM);
	V_BOOL(ft_isalnum('9' + 1), 0, EX_ISALNUM);
	V_BOOL(ft_isalnum('9'), 1, EX_ISALNUM);
}

static void	tr_isascii(void)
{
	V_BOOL(ft_isascii(-1), 0, EX_ISASCII);
	V_BOOL(ft_isascii(0), 1, EX_ISASCII);
	V_BOOL(ft_isascii(128), 0, EX_ISASCII);
	V_BOOL(ft_isascii(127), 1, EX_ISASCII);
}

static void	tr_isprint(void)
{
	V_BOOL(ft_isprint(' ' - 1), 0, EX_ISPRINT);
	V_BOOL(ft_isprint(' '), 1, EX_ISPRINT);
	V_BOOL(ft_isprint('~' + 1), 0, EX_ISPRINT);
	V_BOOL(ft_isprint('~'), 1, EX_ISPRINT);
}

static void	tr_toupper(void)
{
	V_INT(ft_toupper('a' - 1), 'a' - 1, EX_TOUPPER);
	V_INT(ft_toupper('a'), 'A', EX_TOUPPER);
	V_INT(ft_toupper('z' + 1), 'z' + 1, EX_TOUPPER);
	V_INT(ft_toupper('z'), 'Z', EX_TOUPPER);
}

static void	tr_tolower(void)
{
	V_INT(ft_tolower('A' - 1), 'A' - 1, EX_TOLOWER);
	V_INT(ft_tolower('A'), 'a', EX_TOLOWER);
	V_INT(ft_tolower('Z' + 1), 'Z' + 1, EX_TOLOWER);
	V_INT(ft_tolower('Z'), 'z', EX_TOLOWER);
}

/* ---------- alelievr : -1..529 (hors 0..255 : attendu 0) ---------- */

static void	al_isalpha(void)
{
	CTYPE_BOOL(ft_isalpha, ref_isalpha, -1, 529, 0,
		"your isalpha just doesn't work : " EX_ISALPHA);
}

static void	al_isdigit(void)
{
	CTYPE_BOOL(ft_isdigit, ref_isdigit, -1, 529, 0, EX_ISDIGIT);
}

static void	al_isalnum(void)
{
	CTYPE_BOOL(ft_isalnum, isalnum, 0, 0xff, 0, EX_ISALNUM);
}

static void	al_isascii(void)
{
	CTYPE_BOOL(ft_isascii, ref_isascii, -1, 529, 0, EX_ISASCII);
}

static void	al_isprint(void)
{
	CTYPE_BOOL(ft_isprint, ref_isprint, -1, 529, 0, EX_ISPRINT);
}

static void	al_toupper(void)
{
	CTYPE_VAL(ft_toupper, toupper, 0, 0xff, 0, EX_TOUPPER);
}

static void	al_tolower(void)
{
	CTYPE_VAL(ft_tolower, tolower, 0, 0xff, 0, EX_TOLOWER);
}

/* ---------- war-machine : sorties test01..test0N ---------- */

static void	wm_isalpha(void)
{
	WM_BOOL(ft_isalpha, 0, 47, "0*48", EX_ISALPHA);
	WM_BOOL(ft_isalpha, '0', '9', "0*10", EX_ISALPHA);
	WM_BOOL(ft_isalpha, 58, 64, "0*7", EX_ISALPHA);
	WM_BOOL(ft_isalpha, 'A', 'Z', "1*26", EX_ISALPHA);
	WM_BOOL(ft_isalpha, 91, 96, "0*6", EX_ISALPHA);
	WM_BOOL(ft_isalpha, 'a', 'z', "1*26", EX_ISALPHA);
	WM_BOOL(ft_isalpha, 123, 127, "0*5", EX_ISALPHA);
}

static void	wm_isdigit(void)
{
	WM_BOOL(ft_isdigit, 0, 47, "0*48", EX_ISDIGIT);
	WM_BOOL(ft_isdigit, '0', '9', "1*10", EX_ISDIGIT);
	WM_BOOL(ft_isdigit, 58, 64, "0*7", EX_ISDIGIT);
	WM_BOOL(ft_isdigit, 'A', 'Z', "0*26", EX_ISDIGIT);
	WM_BOOL(ft_isdigit, 91, 96, "0*6", EX_ISDIGIT);
	WM_BOOL(ft_isdigit, 'a', 'z', "0*26", EX_ISDIGIT);
	WM_BOOL(ft_isdigit, 123, 127, "0*5", EX_ISDIGIT);
}

static void	wm_isalnum(void)
{
	WM_BOOL(ft_isalnum, 0, 47, "0*48", EX_ISALNUM);
	WM_BOOL(ft_isalnum, '0', '9', "1*10", EX_ISALNUM);
	WM_BOOL(ft_isalnum, 58, 64, "0*7", EX_ISALNUM);
	WM_BOOL(ft_isalnum, 'A', 'Z', "1*26", EX_ISALNUM);
	WM_BOOL(ft_isalnum, 91, 96, "0*6", EX_ISALNUM);
	WM_BOOL(ft_isalnum, 'a', 'z', "1*26", EX_ISALNUM);
	WM_BOOL(ft_isalnum, 123, 127, "0*5", EX_ISALNUM);
}

static void	wm_isascii(void)
{
	WM_BOOL(ft_isascii, 0, 47, "1*48", EX_ISASCII);
	WM_BOOL(ft_isascii, '0', '9', "1*10", EX_ISASCII);
	WM_BOOL(ft_isascii, 58, 64, "1*7", EX_ISASCII);
	WM_BOOL(ft_isascii, 'A', 'Z', "1*26", EX_ISASCII);
	WM_BOOL(ft_isascii, 91, 96, "1*6", EX_ISASCII);
	WM_BOOL(ft_isascii, 'a', 'z', "1*26", EX_ISASCII);
	WM_BOOL(ft_isascii, 123, 127, "1*5", EX_ISASCII);
	WM_BOOL(ft_isascii, 128, 138, "0*11", EX_ISASCII);
}

static void	wm_isprint(void)
{
	WM_BOOL(ft_isprint, 0, 47, "0*32 1*16", EX_ISPRINT);
	WM_BOOL(ft_isprint, '0', '9', "1*10", EX_ISPRINT);
	WM_BOOL(ft_isprint, 58, 64, "1*7", EX_ISPRINT);
	WM_BOOL(ft_isprint, 'A', 'Z', "1*26", EX_ISPRINT);
	WM_BOOL(ft_isprint, 91, 96, "1*6", EX_ISPRINT);
	WM_BOOL(ft_isprint, 'a', 'z', "1*26", EX_ISPRINT);
	WM_BOOL(ft_isprint, 123, 127, "1*4 0*1", EX_ISPRINT);
}

static void	wm_toupper(void)
{
	WM_CHAR(ft_toupper, 0, 47, NULL, EX_TOUPPER);
	WM_CHAR(ft_toupper, '0', '9', NULL, EX_TOUPPER);
	WM_CHAR(ft_toupper, 58, 64, NULL, EX_TOUPPER);
	WM_CHAR(ft_toupper, 'A', 'Z', NULL, EX_TOUPPER);
	WM_CHAR(ft_toupper, 91, 96, NULL, EX_TOUPPER);
	WM_CHAR(ft_toupper, 'a', 'z', "ABCDEFGHIJKLMNOPQRSTUVWXYZ", EX_TOUPPER);
	WM_CHAR(ft_toupper, 123, 127, NULL, EX_TOUPPER);
}

static void	wm_tolower(void)
{
	WM_CHAR(ft_tolower, 0, 47, NULL, EX_TOLOWER);
	WM_CHAR(ft_tolower, '0', '9', NULL, EX_TOLOWER);
	WM_CHAR(ft_tolower, 58, 64, NULL, EX_TOLOWER);
	WM_CHAR(ft_tolower, 'A', 'Z', "abcdefghijklmnopqrstuvwxyz", EX_TOLOWER);
	WM_CHAR(ft_tolower, 91, 96, NULL, EX_TOLOWER);
	WM_CHAR(ft_tolower, 'a', 'z', NULL, EX_TOLOWER);
	WM_CHAR(ft_tolower, 123, 127, NULL, EX_TOLOWER);
}

/* ========================================================================== */
/*                               ft_strlen                                    */
/* ========================================================================== */

static void	strlen_un(int ligne, const char *s, const char *ex)
{
	size_t	r;

	essai(ligne, "ft_strlen(%s)", esc(s));
	r = ft_strlen(s);
	v_uint(ligne, r, strlen(s), ex);
}

static void	fs_strlen(void)
{
	char	str[100];
	int		n;

	strlen_un(__LINE__, "", EX_STRLEN);
	str[0] = (char)EOF;
	str[1] = '\0';
	strlen_un(__LINE__, str, EX_STRLEN);
	srand(42);
	n = 0;
	while (n < REP)
	{
		rand_str(str, rand() % 98 + 1);
		strlen_un(__LINE__, str, EX_STRLEN);
		n++;
	}
}

static void	tr_strlen(void)
{
	V_UINT(ft_strlen("123"), 3, EX_STRLEN);
	V_UINT(ft_strlen(""), 0, EX_STRLEN);
}

static void	el_strlen(void)
{
	char	*s;

	s = alloc_electrique(37);
	strcpy(s, "be carefull with electrical memory !");
	lecture_seule(s, 37);
	g_puits = ft_strlen(s);
}

static void	al_strlen(void)
{
	char		buff[0x100];
	const char	*s;
	int			i;

	strlen_un(__LINE__, "sais-tu compter ?",
		"your strlen doesn't work with basic test");
	srand(42);
	i = 0;
	while (i < REP)
	{
		rand_bytes(buff, sizeof(buff));
		strlen_un(__LINE__, buff + rand() % sizeof(buff),
			"your strlen doesn't work with random strings");
		i++;
	}
	strlen_un(__LINE__, "01234567, AAAAAA, abc\xba e, ......, end of string !",
		"ft_strlen ne marche pas (mauvaise optimisation ?)");
	strlen_un(__LINE__, "\xe2\x99\xab\xe2\x99\xaa.\xc4\xb1l\xc4\xb1l\xc4\xb1ll"
		"|\xcc\xb2\xcc\x85\xcc\x85\xe2\x97\x8f\xcc\xb2\xcc\x85\xcc\x85|.\xe2"
		"\x99\xab\xe2\x99\xaa",
		"ft_strlen doit compter les OCTETS d'une chaine unicode (UTF-8)");
	V_UINT(ft_strlen(""), 0, "ft_strlen(\"\") doit renvoyer 0");
	strlen_un(__LINE__, "HAHAHAHA \0 TA FAIL XD",
		"ft_strlen doit s'arreter au premier '\\0'");
	s = "YOLO";
	if ((unsigned long)s & 7)
		s++;
	strlen_un(__LINE__, s,
		"ft_strlen doit marcher sur une adresse non alignee sur 8");
	T_SANS_CRASH(el_strlen, "ft_strlen(s) avec s = 37 octets suivis "
		"d'une page interdite", EX_ELEC_LIT);
}

static void	wm_strlen(void)
{
	V_UINT(ft_strlen("Hello !"), 7, EX_STRLEN);
	V_UINT(ft_strlen("1"), 1, EX_STRLEN);
	V_UINT(ft_strlen("lorem\tipsum\tdolor\nsit\namet\n"), 27, EX_STRLEN);
	V_UINT(ft_strlen(""), 0, EX_STRLEN);
	V_UINT(ft_strlen("\n\n\f\r\t"), 5, EX_STRLEN);
	V_UINT(ft_strlen("   "), 3, EX_STRLEN);
}

/* ========================================================================== */
/*                               ft_memset                                    */
/* ========================================================================== */

static void	fs_memset_un(int ligne, int c, size_t size)
{
	char	m[MEM_SIZE];
	char	ms[MEM_SIZE];
	void	*r;

	reset(m, ms, MEM_SIZE);
	memset(ms, c, size);
	essai(ligne, "ft_memset(ptr, %d, %zu)", c, size);
	r = ft_memset(m, c, size);
	v_ptr(ligne, r, m, m, EX_MEMSET_RET);
	v_mem(ligne, m, ms, MEM_SIZE, EX_MEMSET);
}

static void	fs_memset(void)
{
	int		m2[MEM_SIZE];
	int		ms2[MEM_SIZE];
	void	*r;

	fs_memset_un(__LINE__, 'x', 0);
	fs_memset_un(__LINE__, 'g', 12);
	fs_memset_un(__LINE__, 300, 12);
	fs_memset_un(__LINE__, 257, 19);
	reset(m2, ms2, MEM_SIZE);
	memset(ms2 + 1, 143462, 4);
	ESSAI("ft_memset(int_tab + 1, 143462, 4)");
	r = ft_memset(m2 + 1, 143462, 4);
	V_PTR(r, m2 + 1, m2, EX_MEMSET_RET);
	V_MEM(m2, ms2, 0x10, "ft_memset doit ecrire (unsigned char)c octet par "
		"octet, meme dans un tableau d'int");
}

static void	tr_memset(void)
{
	char	tab[100];
	char	att[100];

	memset(tab, 0, 100);
	memset(att, 0, 100);
	CALL(ft_memset(tab, 'A', 0));
	V_MEM(tab, att, 100, "ft_memset(tab, 'A', 0) ne doit rien modifier");
	CALL(ft_memset(tab, 'A', 42));
	memset(att, 'A', 42);
	V_MEM(tab, att, 100, "ft_memset(tab, 'A', 42) doit remplir 42 octets, "
		"tab[42] doit rester a 0");
}

static void	el_memset(void)
{
	char	*buff;

	buff = alloc_electrique(10);
	memset(buff, 0, 10);
	ft_memset(buff, '\xff', 10);
}

static void	al_memset(void)
{
	char	b1[BSIZE];
	char	b2[BSIZE];
	char	vide[1];
	char	*f1;
	char	*f2;
	void	*r;

	memset(b1, 'B', BSIZE);
	memset(b2, 'B', BSIZE);
	memset(b1, 'A', 20);
	CALL(ft_memset(b2, 'A', 20));
	V_MEM(b2, b1, BSIZE, "basic memset test (fill a buffer with 'A')");
	memset(b1, 'B', BSIZE);
	memset(b2, 'B', BSIZE);
	memset(b1, '\200', 22);
	CALL(ft_memset(b2, '\200', 22));
	V_MEM(b2, b1, BSIZE, "ft_memset doit convertir c en unsigned char");
	memset(b1, 'B', BSIZE);
	r = CALL(ft_memset(b1, 'A', 18));
	V_PTR(r, b1, b1, "your memset return address is false");
	r = CALL(ft_memset(vide, 'A', 0));
	V_PTR(r, vide, vide, "ft_memset doit renvoyer b meme quand len = 0");
	f1 = malloc(BFSIZE + 1);
	f2 = malloc(BFSIZE + 1);
	if (f1 && f2)
	{
		memset(f1, '\5', BFSIZE);
		CALL(ft_memset(f2, '\5', BFSIZE));
		V_MEM(f2, f1, BFSIZE, "ft_memset doit marcher sur un gros buffer "
			"(0xF0000 octets)");
	}
	free(f1);
	free(f2);
	memset(b1, 0, BSIZE);
	memset(b2, 0, BSIZE);
	CALL(ft_memset(b1, '\xff', 0));
	V_MEM(b1, b2, BSIZE, "ft_memset ne doit rien changer quand len = 0");
	T_SANS_CRASH(el_memset, "ft_memset(buf, 0xff, 10) avec buf = 10 octets "
		"suivis d'une page interdite", EX_ELEC_ECRIT);
}

static void	wm_memset(void)
{
	char	*mem;
	void	*r;

	mem = malloc(15);
	if (!mem)
		return ;
	memset(mem, 'j', 15);
	r = CALL(ft_memset(mem, 'c', 5));
	V_PTR(r, mem, mem, "mem's adress was not returned");
	V_MEM(mem, "cccccjjjjjjjjjj", 15, EX_MEMSET);
	memset(mem, 'j', 15);
	r = CALL(ft_memset(mem, 'c', 14));
	V_PTR(r, mem, mem, "mem's adress was not returned");
	V_MEM(mem, "ccccccccccccccj", 15, EX_MEMSET);
	memset(mem, 'j', 15);
	r = CALL(ft_memset(mem, '\n', 6));
	V_PTR(r, mem, mem, "mem's adress was not returned");
	V_MEM(mem, "\n\n\n\n\n\njjjjjjjjj", 15, EX_MEMSET);
	memset(mem, 'j', 15);
	r = CALL(ft_memset(mem, '\0', 1));
	V_PTR(r, mem, mem, "mem's adress was not returned");
	V_MEM(mem, "\0jjjjjjjjjjjjjj", 15, EX_MEMSET);
	free(mem);
}

/* ========================================================================== */
/*                                ft_bzero                                    */
/* ========================================================================== */

static void	fs_bzero_un(int ligne, size_t size)
{
	char	m[MEM_SIZE];
	char	ms[MEM_SIZE];

	reset(m, ms, MEM_SIZE);
	bzero(ms, size);
	essai(ligne, "ft_bzero(ptr, %zu)", size);
	ft_bzero(m, size);
	v_mem(ligne, m, ms, MEM_SIZE, EX_BZERO);
}

static void	fs_bzero(void)
{
	fs_bzero_un(__LINE__, 0);
	fs_bzero_un(__LINE__, 12);
}

static void	tr_bzero(void)
{
	char	tab[100];
	char	att[100];

	memset(tab, 'A', 100);
	memset(att, 'A', 100);
	CALL(ft_bzero(tab, 0));
	V_MEM(tab, att, 100, "ft_bzero(tab, 0) ne doit rien modifier");
	CALL(ft_bzero(tab, 42));
	memset(att, 0, 42);
	V_MEM(tab, att, 100, "ft_bzero(tab, 42) : 42 zeros puis tab[42] == 'A'");
	memset(tab, 'A', 100);
	tab[0] = 0;
	CALL(ft_bzero(tab, 42));
	V_MEM(tab, att, 100, "ft_bzero(tab, 42) avec tab[0] deja a 0 : 42 zeros");
}

static void	el_bzero(void)
{
	char	*buff;

	buff = alloc_electrique(10);
	memset(buff, '\x1', 10);
	ft_bzero(buff, 10);
}

static void	al_bzero(void)
{
	char	str[BSIZE];
	char	str2[BSIZE];

	memset(str, 'a', BSIZE);
	memset(str2, 'a', BSIZE);
	bzero(str, 20);
	CALL(ft_bzero(str2, 20));
	V_MEM(str2, str, BSIZE, "bzero with normal params fail !");
	memset(str, '\x1', BSIZE);
	memset(str2, '\x1', BSIZE);
	CALL(ft_bzero(str2, 0));
	V_MEM(str2, str, BSIZE, "ft_bzero ne doit rien changer quand n = 0");
	T_SANS_CRASH(el_bzero, "ft_bzero(buf, 10) avec buf = 10 octets suivis "
		"d'une page interdite", EX_ELEC_ECRIT);
}

static void	wm_bzero(void)
{
	char	*mem;

	mem = malloc(5);
	if (!mem)
		return ;
	memset(mem, 'e', 5);
	CALL(ft_bzero(mem, 5));
	V_MEM(mem, "\0\0\0\0\0", 5, EX_BZERO);
	memset(mem, 'e', 5);
	CALL(ft_bzero(mem, 0));
	V_MEM(mem, "eeeee", 5, EX_BZERO);
	free(mem);
}

/* ========================================================================== */
/*                               ft_memcpy                                    */
/* ========================================================================== */

static void	fs_memcpy_un(int ligne, int n)
{
	char	dest[MEM_SIZE];
	char	dest_std[MEM_SIZE];
	char	src[MEM_SIZE];
	char	src_std[MEM_SIZE];
	void	*r;

	reset(dest, dest_std, MEM_SIZE);
	reset(src, src_std, MEM_SIZE);
	rand_str(src, rand() % 55 + 1);
	memcpy(src_std, src, 100);
	essai(ligne, "ft_memcpy(dest, %s, %d)", esc(src), n);
	r = ft_memcpy(dest, src, n);
	memcpy(dest_std, src_std, n);
	v_ptr(ligne, r, dest, dest, EX_MEMCPY_RET);
	v_mem(ligne, dest, dest_std, MEM_SIZE, EX_MEMCPY);
}

static void	fs_memcpy(void)
{
	int	n;

	srand(42);
	fs_memcpy_un(__LINE__, 0);
	n = 0;
	while (n < REP)
	{
		fs_memcpy_un(__LINE__, rand() % 70);
		n++;
	}
}

static void	tr_memcpy(void)
{
	char	dest[100];
	char	att[100];
	char	src[2];
	void	*rtn;

	memset(dest, 'A', 100);
	memset(att, 'A', 100);
	CALL(ft_memcpy(dest, "coucou", 0));
	V_MEM(dest, att, 100, "ft_memcpy(dest, \"coucou\", 0) ne doit rien copier");
	rtn = CALL(ft_memcpy(dest, NULL, 0));
	V_PTR(rtn, dest, dest, "ft_memcpy(dest, NULL, 0) doit renvoyer dest sans "
		"rien copier");
	V_MEM(dest, att, 100, "ft_memcpy(dest, NULL, 0) ne doit rien modifier");
	src[0] = 0;
	src[1] = 0;
	CALL(ft_memcpy(dest, src, 2));
	att[0] = 0;
	att[1] = 0;
	V_MEM(dest, att, 100, "ft_memcpy doit copier les '\\0' : 2 zeros puis "
		"dest[2] == 'A'");
}

static char	*g_el_src;
static char	*g_el_dst;

static void	el_memcpy(void)
{
	g_el_src = alloc_electrique(10);
	g_el_dst = alloc_electrique(10);
	strcpy(g_el_src, "NYANCATSH");
	lecture_seule(g_el_src, 10);
	ft_memcpy(g_el_dst, g_el_src, 10);
}

struct s_al_struct
{
	const char	*s;
	void		*p;
	long		l;
	int			i;
};

static void	al_memcpy(void)
{
	char				src[] = "test basic du memcpy !";
	char				b1[22];
	char				b2[22];
	char				vide[1];
	char				src3[] = STRING_3;
	char				c1[] = STRING_1;
	char				c2[] = STRING_1;
	char				z[] = STRING_4;
	char				buff1[0xF00];
	char				buff2[0xF00];
	struct s_al_struct	st;
	void				*r;

	memcpy(b1, src, 22);
	CALL(ft_memcpy(b2, src, 22));
	V_MEM(b2, b1, 22, "your memcpy doesn't work with basic params");
	r = CALL(ft_memcpy(b1, src, 22));
	V_PTR(r, b1, b1, "your memcpy's return is false");
	r = CALL(ft_memcpy(vide, src, 0));
	V_PTR(r, vide, vide, "ft_memcpy doit renvoyer dest meme quand n = 0");
	CALL(ft_memcpy(z, src, 0));
	V_MEM(z, STRING_4, sizeof(z), "your memcpy does not work when call "
		"with 0 (rien ne doit etre copie)");
	memcpy(c1, src3, strlen(src3));
	CALL(ft_memcpy(c2, src3, strlen(src3)));
	V_MEM(c2, c1, sizeof(c1), "your memcpy does not work with basic params");
	st.s = "nyancat\xc2\xae inside";
	st.p = (void *)0xdeadbeef;
	st.l = 0x42424242424242L;
	st.i = 0x2A2;
	memset(buff1, 0, sizeof(buff1));
	memset(buff2, 0, sizeof(buff2));
	memcpy(buff1, &st, sizeof(st));
	CALL(ft_memcpy(buff2, &st, sizeof(st)));
	V_MEM(buff2, buff1, sizeof(st), "ft_memcpy doit copier une structure "
		"octet par octet");
	T_SANS_CRASH(el_memcpy, "ft_memcpy(dst, src, 10) avec dst et src de 10 "
		"octets suivis d'une page interdite", EX_ELEC_LIT);
}

static void	wm_memcpy(void)
{
	char	*mem;
	void	*r;

	mem = malloc(30);
	if (!mem)
		return ;
	memset(mem, 'j', 30);
	r = CALL(ft_memcpy(mem, "zyxwvutsrqponmlkjihgfedcba", 14));
	V_PTR(r, mem, mem, "dest's adress was not returned");
	V_MEM(mem, "zyxwvutsrqponmjjjjjjjjjjjjjjjj", 30, EX_MEMCPY);
	memset(mem, 'j', 30);
	r = CALL(ft_memcpy(mem, "zyxwvutst", 0));
	V_PTR(r, mem, mem, "dest's adress was not returned");
	V_MEM(mem, "jjjjjjjjjjjjjjjjjjjjjjjjjjjjjj", 30, EX_MEMCPY);
	memset(mem, 'j', 30);
	r = CALL(ft_memcpy(mem, "zy\0xw\0vu\0\0tsr", 11));
	V_PTR(r, mem, mem, "dest's adress was not returned");
	V_MEM(mem, "zy\0xw\0vu\0\0tjjjjjjjjjjjjjjjjjjj", 30,
		"ft_memcpy doit copier les '\\0' (ne pas s'arreter au premier)");
	free(mem);
}

/* ========================================================================== */
/*                               ft_memmove                                   */
/* ========================================================================== */

static void	fs_memmove_un(int ligne, int od, int os, const char *val, int n)
{
	char	a[MEM_SIZE + 10];
	char	b[MEM_SIZE + 10];
	void	*r;

	reset(a, b, sizeof(a));
	strcpy(a + os, val);
	strcpy(b + os, val);
	essai(ligne, "ft_memmove(buf + %d, buf + %d /* %s */, %d)",
		od, os, esc(val), n);
	r = ft_memmove(a + od, a + os, n);
	memmove(b + od, b + os, n);
	v_ptr(ligne, r, a + od, a, EX_MEMMOVE_RET);
	v_mem(ligne, a, b, sizeof(a), EX_MEMMOVE);
}

static void	fs_memmove(void)
{
	fs_memmove_un(__LINE__, 0, 2, "123456", 4);
	fs_memmove_un(__LINE__, 2, 0, "123456", 4);
	fs_memmove_un(__LINE__, 0, 0, "123456", 4);
	fs_memmove_un(__LINE__, 2, 0, "123456", 0);
}

static void	tr_memmove(void)
{
	char	s[] = {65, 66, 67, 68, 69, 0, 45};
	char	s0[] = {0, 0, 0, 0, 0, 0, 0};
	char	scpy[] = {65, 66, 67, 68, 69, 0, 45};
	char	sres[] = {67, 68, 67, 68, 69, 0, 45};
	char	sres2[] = {67, 67, 68, 68, 69, 0, 45};
	void	*r;

	r = CALL(ft_memmove(s0, s, 7));
	V_PTR(r, s0, s0, EX_MEMMOVE_RET);
	V_MEM(s0, s, 7, "ft_memmove(s0, s, 7) : copie simple, '\\0' compris");
	r = CALL(ft_memmove(s, s + 2, 0));
	V_OK(r != NULL, "ft_memmove(s, s + 2, 0) doit renvoyer dest (pas NULL)");
	V_MEM(s, scpy, 7, "ft_memmove avec n = 0 ne doit rien modifier");
	r = CALL(ft_memmove(s, s + 2, 2));
	V_PTR(r, s, s, EX_MEMMOVE_RET);
	V_MEM(s, sres, 7, "ft_memmove(s, s + 2, 2) : chevauchement vers l'avant");
	r = CALL(ft_memmove(sres + 1, sres, 2));
	V_PTR(r, sres + 1, sres, EX_MEMMOVE_RET);
	V_MEM(sres, sres2, 7, "ft_memmove(s + 1, s, 2) : chevauchement vers "
		"l'arriere, il faut copier en partant de la fin");
}

static char	*g_mm_dst;
static char	*g_mm_src;

static void	el_memmove(void)
{
	g_mm_dst = alloc_electrique(10);
	g_mm_src = alloc_electrique(10);
	memset(g_mm_src, 'A', 10);
	lecture_seule(g_mm_src, 10);
	ft_memmove(g_mm_dst, g_mm_src, 10);
	g_mm_dst = alloc_electrique_avant(10);
	g_mm_src = alloc_electrique_avant(10);
	memset(g_mm_src, 'A', 10);
	lecture_seule(g_mm_src, 10);
	ft_memmove(g_mm_dst, g_mm_src, 10);
}

static void	al_memmove_overlap(int ligne, int sens)
{
	char		d1[0xF0];
	char		d2[0xF0];
	const char	*data;
	int			size;

	data = "thi\xc3\x9f \xc3\x9f\xde\xad\xbe\xef" "tri\xc3\xb1g will be "
		"\xc3\xb8v\xc3\xa9rlap\xc3\xa9" "d !\r\n";
	size = 0xF0 - 0xF;
	memset(d1, 'A', sizeof(d1));
	memset(d2, 'A', sizeof(d2));
	memcpy(d1, data, strlen(data));
	memcpy(d2, data, strlen(data));
	if (sens)
	{
		essai(ligne, "ft_memmove(buf + 3, buf, %d)", size);
		memmove(d1 + 3, d1, size);
		ft_memmove(d2 + 3, d2, size);
	}
	else
	{
		essai(ligne, "ft_memmove(buf, buf + 3, %d)", size);
		memmove(d1, d1 + 3, size);
		ft_memmove(d2, d2 + 3, size);
	}
	v_mem(ligne, d2, d1, sizeof(d1), EX_MEMMOVE);
}

static void	al_memmove(void)
{
	const char		*src;
	char			d1[0xF0];
	char			d2[0xF0];
	char			vide[1];
	unsigned long	l1;
	unsigned long	l2;
	unsigned long	ls;
	char			dat1[] = STRING_1;
	char			dat2[] = STRING_1;
	const char		bin[] = "thi\xffs i\xfas \0a g\xde\xadood \0nyan\0cat\0 !\r\n";
	char			*hd;
	char			*hs;
	size_t			hsize;
	void			*r;

	src = "this is a good nyancat !\r\n";
	memmove(d1, src, strlen(src));
	CALL(ft_memmove(d2, src, strlen(src)));
	V_MEM(d2, d1, strlen(src), "your memmove does not work with basic input");
	src = "thanks to @apellicc for this test !\r\n";
	r = CALL(ft_memmove(d1, src, strlen(src)));
	V_PTR(r, d1, d1, "your memmove's return is false");
	r = CALL(ft_memmove(vide, vide, 0));
	V_PTR(r, vide, vide, "ft_memmove doit renvoyer dest meme quand n = 0");
	memmove(d1, bin, 33);
	CALL(ft_memmove(d2, bin, 33));
	V_MEM(d2, d1, 33, "your memmove does not work with \\0 and others "
		"unsigned char codes");
	ls = 0xdeadbeef;
	memmove(&l1, &ls, sizeof(ls));
	CALL(ft_memmove(&l2, &ls, sizeof(ls)));
	V_MEM(&l2, &l1, sizeof(ls), "your memmove does not work with integers "
		"copy");
	al_memmove_overlap(__LINE__, 1);
	al_memmove_overlap(__LINE__, 0);
	hsize = 128 * 1024 * 1024;
	hd = malloc(hsize);
	hs = malloc(hsize);
	if (hd && hs)
	{
		memset(hs, 'A', hsize);
		ESSAI("ft_memmove(dst, data, 128 Mo)");
		r = ft_memmove(hd, hs, hsize);
		V_PTR(r, hd, hd, EX_MEMMOVE_RET);
		V_OK(hd[0] == 'A' && hd[hsize / 2] == 'A' && hd[hsize - 1] == 'A',
			"your memmove does not work with a 128Mo data input !");
	}
	free(hd);
	free(hs);
	T_SANS_CRASH(el_memmove, "ft_memmove(dst, src, 10) avec des buffers de "
		"10 octets entoures de pages interdites",
		"ta fonction lit trop d'octets ou lit/ecrit AVANT le debut des "
		"buffers");
	memmove(dat1, dat1, strlen(STRING_1));
	r = CALL(ft_memmove(dat2, dat2, strlen(STRING_1)));
	V_PTR(r, dat2, dat2, EX_MEMMOVE_RET);
	V_MEM(dat2, dat1, sizeof(dat1), "ft_memmove(s, s, n) (meme pointeur) "
		"ne doit rien abimer");
	if (g_malloc_dispo)
	{
		suivi_debut(1);
		r = CALL(ft_memmove(d1, dat1, sizeof(dat1)));
		suivi_fin();
		V_PTR(r, d1, d1, "your memmove use malloc ? why ? (ft_memmove a "
			"renvoye autre chose que dest quand malloc echoue)");
		V_UINT(suivi_nb_malloc(), 0, "ft_memmove n'a pas besoin de malloc : "
			"il suffit de choisir le sens de copie");
	}
}

static void	wm_memmove(void)
{
	char	src[] = "lorem ipsum dolor sit amet";
	char	*dest;
	void	*r;

	dest = src + 1;
	r = CALL(ft_memmove(dest, "consectetur", 5));
	V_PTR(r, dest, src, "dest's adress was not returned");
	V_MEM(dest, "conseipsum dolor sit a", 22, EX_MEMMOVE);
	strcpy(src, "lorem ipsum dolor sit amet");
	r = CALL(ft_memmove(dest, "con\0sec\0\0te\0tur", 10));
	V_PTR(r, dest, src, "dest's adress was not returned");
	V_MEM(dest, "con\0sec\0\0t dolor sit a", 22,
		"ft_memmove doit copier les '\\0' (ne pas s'arreter au premier)");
	strcpy(src, "lorem ipsum dolor sit amet");
	r = CALL(ft_memmove(dest, src, 8));
	V_PTR(r, dest, src, "dest's adress was not returned");
	V_MEM(dest, "lorem ipum dolor sit a", 22, "ft_memmove(src + 1, src, 8) : "
		"dest apres src qui se chevauchent, copier depuis la fin");
	strcpy(src, "lorem ipsum dolor sit amet");
	r = CALL(ft_memmove(src, dest, 8));
	V_PTR(r, src, src, "dest's adress was not returned");
	V_MEM(dest, "rem ipssum dolor sit a", 22, "ft_memmove(src, src + 1, 8) : "
		"chevauchement vers l'avant");
	strcpy(src, "lorem ipsum dolor sit amet");
	r = CALL(ft_memmove(src, dest, 0));
	V_PTR(r, src, src, "dest's adress was not returned");
	V_MEM(dest, "orem ipsum dolor sit a", 22, "ft_memmove avec n = 0 ne doit "
		"rien modifier");
}

/* ========================================================================== */
/*                               ft_memchr                                    */
/* ========================================================================== */

static void	memchr_un(int ligne, const void *s, int c, size_t n, size_t aff,
		const char *ex)
{
	void	*r;

	essai(ligne, "ft_memchr(%s, %d, %zu)", esc_n(s, aff), c, n);
	r = ft_memchr(s, c, n);
	v_ptr(ligne, r, memchr(s, c, n), s, ex);
}

static void	fs_memchr(void)
{
	char	str[MEM_SIZE];
	int		i;

	srand(42);
	memchr_un(__LINE__, rand_bytes(str, 0x31), 123, 0, 0x30, EX_MEMCHR);
	i = 0;
	while (i < REP)
	{
		rand_bytes(str, 0x31);
		memchr_un(__LINE__, str, rand() % 0x400, rand() % 0x30, 0x30,
			EX_MEMCHR);
		i++;
	}
}

static void	tr_memchr(void)
{
	char	s[] = {0, 1, 2, 3, 4, 5};

	V_PTR(ft_memchr(s, 0, 0), NULL, s, "ft_memchr avec n = 0 doit renvoyer "
		"NULL");
	V_PTR(ft_memchr(s, 0, 1), s, s, "ft_memchr doit pouvoir trouver un 0");
	V_PTR(ft_memchr(s, 2, 3), s + 2, s, EX_MEMCHR);
	V_PTR(ft_memchr(s, 6, 6), NULL, s, EX_MEMCHR);
	V_PTR(ft_memchr(s, 2 + 256, 3), s + 2, s, "ft_memchr doit convertir c en "
		"unsigned char (2 + 256 -> 2)");
}

static char	*g_mc_src;

static void	el_memchr(void)
{
	g_mc_src = alloc_electrique(15);
	strcpy(g_mc_src, ":(){ :|: & };:");
	lecture_seule(g_mc_src, 15);
	g_puits = (size_t)ft_memchr(g_mc_src, '\xde', 15);
}

static void	al_memchr(void)
{
	const char	*s1;
	const char	*s2;
	char		far[] = "iawhf\200wf208awfh\xa" "kufi21f4\x42w7fwa1f";
	const char	*s3;

	s1 = "/|\x12\xff\x09\x42\042\42|\\";
	memchr_un(__LINE__, s1, '\x42', 10, 10, "your memchr does not work with "
		"basic input");
	s2 = "/|\x12\xff\x09\x42\200" "2\42|\\";
	memchr_un(__LINE__, s2, '\200', 10, 10, "your memchr does not cast in "
		"unsigned the memory");
	memchr_un(__LINE__, s1, '\x42', 2, 10, "ft_memchr ne doit chercher que "
		"dans les n premiers octets");
	memchr_un(__LINE__, s1, '\xde', 10, 10, "ft_memchr doit renvoyer NULL si "
		"c est absent");
	memchr_un(__LINE__, far, '\x42', sizeof(far), sizeof(far),
		"your memchr does not work on basic input");
	T_SANS_CRASH(el_memchr, "ft_memchr(s, 0xde, 15) avec s = 15 octets "
		"suivis d'une page interdite", EX_ELEC_LIT);
	s3 = "/|\x12\xff\x09\0\x42\042\0\42|\\";
	memchr_un(__LINE__, s3, '\0', 12, 12, "your memchr failed to find a \\0");
}

static void	wm_memchr(void)
{
	const char	*b;
	const char	*bn;
	int			tab[7];

	b = "bonjour";
	bn = "bonjourno";
	V_PTR(ft_memchr(b, 'b', 4), b, b, EX_MEMCHR);
	V_PTR(ft_memchr(b, 'o', 7), b + 1, b, EX_MEMCHR);
	V_PTR(ft_memchr(bn, 'n', 2), NULL, bn, EX_MEMCHR);
	V_PTR(ft_memchr(b, 'j', 6), b + 3, b, EX_MEMCHR);
	V_PTR(ft_memchr(b, 's', 7), NULL, b, EX_MEMCHR);
	tab[0] = -49;
	tab[1] = 49;
	tab[2] = 1;
	tab[3] = -1;
	tab[4] = 0;
	tab[5] = -2;
	tab[6] = 2;
	V_PTR(ft_memchr(tab, -1, 7), memchr(tab, -1, 7), tab,
		"ft_memchr doit comparer en unsigned char : -1 devient 0xff");
}

/* ========================================================================== */
/*                               ft_memcmp                                    */
/* ========================================================================== */

static int	memcmp_un(int ligne, const void *s1, const void *s2, size_t n,
		const char *ex)
{
	int	r;

	essai(ligne, "ft_memcmp(%s, %s, %zu)", esc_n(s1, n), esc_n(s2, n), n);
	r = ft_memcmp(s1, s2, n);
	return (v_signe(ligne, r, ref_memcmp(s1, s2, n), ex));
}

static void	fs_memcmp(void)
{
	char	s1[20];
	char	s2[20];

	memcmp_un(__LINE__, "teste", "teste", 0, EX_MEMCMP);
	memcmp_un(__LINE__, "teste", "teste", 1, EX_MEMCMP);
	memcmp_un(__LINE__, "teste", "teste", 5, EX_MEMCMP);
	memcmp_un(__LINE__, "teste", "teste", 6, EX_MEMCMP);
	memcmp_un(__LINE__, "teste", "testex", 6, EX_MEMCMP);
	memset(s1, 0, sizeof(s1));
	memset(s2, 0, sizeof(s2));
	strcpy(s1, "teste");
	strcpy(s2, "test");
	memcmp_un(__LINE__, s1, s2, 10, EX_MEMCMP);
	memcmp_un(__LINE__, s2, s1, 10, EX_MEMCMP);
	strcpy(s1, "abcdef");
	strcpy(s2, "abc\xfdxx");
	memcmp_un(__LINE__, s1, s2, 5, "ft_memcmp doit comparer en unsigned "
		"char ('d' < 0xfd)");
	s1[3] = 0;
	s2[3] = 0;
	memcmp_un(__LINE__, s1, s2, 7, "You are stopping at the '\\0' : "
		"ft_memcmp doit comparer les n octets, meme apres un '\\0'");
}

static void	tr_memcmp(void)
{
	char	s[] = {(char)0x80, 0, 127, 0};
	char	scpy[] = {(char)0x80, 0, 127, 0};
	char	s2[] = {0, 0, 127, 0};
	char	s3[] = {0, 0, 42, 0};

	V_SIGNE(ft_memcmp(s, scpy, 4), 0, EX_MEMCMP);
	V_SIGNE(ft_memcmp(s, s2, 0), 0, "ft_memcmp avec n = 0 doit renvoyer 0");
	V_SIGNE(ft_memcmp(s, s2, 1), 1, "ft_memcmp doit comparer en unsigned "
		"char : 0x80 > 0");
	V_SIGNE(ft_memcmp(s2, s, 1), -1, "ft_memcmp doit comparer en unsigned "
		"char : 0 < 0x80");
	V_SIGNE(ft_memcmp(s2, s3, 4), 1, EX_MEMCMP);
}

static char	*g_cmp1;
static char	*g_cmp2;

static void	el_memcmp(void)
{
	g_cmp1 = alloc_electrique(10);
	g_cmp2 = alloc_electrique(10);
	strcpy(g_cmp1, ".........");
	strcpy(g_cmp2, ".........");
	lecture_seule(g_cmp1, 10);
	lecture_seule(g_cmp2, 10);
	g_puits = (size_t)ft_memcmp(g_cmp1, g_cmp2, 10);
}

static void	al_memcmp(void)
{
	char	u1[8] = "\xff\xaa\xde\200";
	char	u2[8] = "\xff\xaa\xde\0";
	char	b2a[16] = "\xff\xaa\xde\x12";

	memcmp_un(__LINE__, "\xff\xaa\xde\x12MACOSX", "\xff\xaa\xde\x12MACOSX", 10,
		"your memcmp does not work with basic input");
	memcmp_un(__LINE__, "\xff\xaa\xde\x12WXYZ", "\xff\xaa\xde\x12MACOSX", 7,
		"your memcmp does not work with basic input");
	memcmp_un(__LINE__, b2a, "\xff\xaa\xde\x12MACOSAAAAA", 4,
		"your memcmp does not work with basic input");
	memcmp_un(__LINE__, "\xff\xaa\xde\xffMACOSX\xff", "\xff\xaa\xde\x02\0\0\0",
		8, "your memcmp does not work with basic input");
	memcmp_un(__LINE__, "atoms\0\0\0\0", "atoms\0abc", 8,
		"your memcmp stop at \\0 : il faut comparer les n octets");
	memcmp_un(__LINE__, u1, u2, 8, "your memcmp does not cast the memory in "
		"unsigned char");
	memcmp_un(__LINE__, "\xff\0\0\xaa\0\xde\xffMACOSX\xff",
		"\xff\0\0\xaa\0\xde\x00MBS", 9,
		"your memcmp does not work with basic input");
	T_SANS_CRASH(el_memcmp, "ft_memcmp(b1, b2, 10) avec des buffers de 10 "
		"octets suivis d'une page interdite", EX_ELEC_LIT);
}

static void	wm_memcmp(void)
{
	char	vide[5];

	memset(vide, 0, sizeof(vide));
	V_SIGNE(ft_memcmp("salut", "salut", 5), 0, EX_MEMCMP);
	V_SIGNE(ft_memcmp("t\200", "t\0", 2), 1, "ft_memcmp doit comparer en "
		"unsigned char : '\\200' > '\\0'");
	V_SIGNE(ft_memcmp("testss", "test", 5), 1, EX_MEMCMP);
	V_SIGNE(ft_memcmp("test", "tEst", 4), 1, EX_MEMCMP);
	V_SIGNE(ft_memcmp(vide, "test", 4), -1, EX_MEMCMP);
	V_SIGNE(ft_memcmp("test", vide, 4), 1, EX_MEMCMP);
	V_SIGNE(ft_memcmp("abcdefghij", "abcdefgxyz", 7), 0, EX_MEMCMP);
	V_SIGNE(ft_memcmp("abcdefgh", "abcdwxyz", 6), -1, EX_MEMCMP);
	V_SIGNE(ft_memcmp("zyxbcdefgh", "abcdefgxyz", 0), 0, "ft_memcmp avec "
		"n = 0 doit renvoyer 0");
}

/* ========================================================================== */
/*                               ft_strlcpy                                   */
/* ========================================================================== */

/* Compare ft_strlcpy a la reference sur deux buffers identiques. */
static void	lcpy_cmp(int ligne, const char *src, size_t size, char *b1,
		char *b2, size_t buf, const char *ex)
{
	size_t	r1;
	size_t	r2;

	essai(ligne, "ft_strlcpy(dest, %s, %zu)", esc(src), size);
	r2 = ft_strlcpy(b2, src, size);
	r1 = ref_strlcpy(b1, src, size);
	v_uint(ligne, r2, r1, EX_STRLCPY_RET);
	v_mem(ligne, b2, b1, buf, ex);
}

static void	fs_strlcpy_un(int ligne, const char *src, size_t n)
{
	char	dest[MEM_SIZE];
	char	dest_std[MEM_SIZE];

	reset(dest, dest_std, MEM_SIZE);
	lcpy_cmp(ligne, src, n, dest_std, dest, MEM_SIZE, EX_STRLCPY);
}

static void	fs_strlcpy(void)
{
	fs_strlcpy_un(__LINE__, "aaa", 0);
	fs_strlcpy_un(__LINE__, "aaa", 2);
	fs_strlcpy_un(__LINE__, "aaa", 3);
	fs_strlcpy_un(__LINE__, "aaa", 4);
	fs_strlcpy_un(__LINE__, "aasdjj;s;sa", 100);
}

static void	tr_strlcpy(void)
{
	char	src[] = "coucou";
	char	dest[10];

	memset(dest, 'A', 10);
	V_UINT(ft_strlcpy(dest, src, 0), 6, EX_STRLCPY_RET);
	V_MEM(dest, "AAAAAAAAAA", 10, "ft_strlcpy avec size = 0 ne doit rien "
		"ecrire");
	V_UINT(ft_strlcpy(dest, src, 1), 6, EX_STRLCPY_RET);
	V_MEM(dest, "\0AAAAAAAAA", 10, "ft_strlcpy avec size = 1 : juste '\\0'");
	V_UINT(ft_strlcpy(dest, src, 2), 6, EX_STRLCPY_RET);
	V_MEM(dest, "c\0AAAAAAAA", 10, "ft_strlcpy avec size = 2 : 'c' puis '\\0'");
	V_UINT(ft_strlcpy(dest, src, -1), 6, EX_STRLCPY_RET);
	V_MEM(dest, "coucou\0AAA", 10, "ft_strlcpy avec size = SIZE_MAX : copie "
		"complete, rien apres le '\\0'");
	memset(dest, 'A', 10);
	V_UINT(ft_strlcpy(dest, src, 6), 6, EX_STRLCPY_RET);
	V_MEM(dest, "couco\0AAAA", 10, "ft_strlcpy avec size = 6 : 5 caracteres "
		"puis '\\0'");
	memset(dest, 'A', 10);
	V_UINT(ft_strlcpy(dest, src, 7), 6, EX_STRLCPY_RET);
	V_MEM(dest, "coucou\0AAA", 10, EX_STRLCPY);
	memset(dest, 'A', 10);
	V_UINT(ft_strlcpy(dest, src, 8), 6, EX_STRLCPY_RET);
	V_MEM(dest, "coucou\0AAA", 10, EX_STRLCPY);
	memset(dest, 'A', 10);
	V_UINT(ft_strlcpy(dest, "", 42), 0, EX_STRLCPY_RET);
	V_MEM(dest, "\0AAAAAAAAA", 10, "ft_strlcpy d'une chaine vide : juste "
		"'\\0'");
	memset(dest, 0, 10);
	V_UINT(ft_strlcpy(dest, "1", 0), 1, EX_STRLCPY_RET);
	V_MEM(dest, "\0\0\0\0\0\0\0\0\0\0", 10, "ft_strlcpy avec size = 0 ne doit "
		"rien ecrire");
}

static void	al_strlcpy(void)
{
	char	b1[BSIZE];
	char	b2[BSIZE];

	memset(b1, 'A', sizeof(b1) - 1);
	memset(b2, 'A', sizeof(b2) - 1);
	b1[sizeof(b1) - 1] = 0;
	b2[sizeof(b2) - 1] = 0;
	lcpy_cmp(__LINE__, STRING_1, sizeof(b1), b1, b2, BSIZE,
		"your strlcpy does not works with basic input");
	lcpy_cmp(__LINE__, STRING_1, sizeof(b1), b1, b2, BSIZE,
		"your strlcpy does not return the good value");
	memset(b1, 'A', 20);
	memset(b2, 'A', 20);
	lcpy_cmp(__LINE__, "BBBB", sizeof(b1), b1, b2, 20,
		"your strlcpy overflow the dest");
	memset(b1, 'A', 20);
	memset(b2, 'A', 20);
	lcpy_cmp(__LINE__, "hello !", 2, b1, b2, 20,
		"your strlcpy does not work when size < strlen(src)");
	memset(b1, 'A', 20);
	memset(b2, 'A', 20);
	lcpy_cmp(__LINE__, "", sizeof(b1), b1, b2, 20,
		"your strlcpy does not works with 0-length string");
	memset(b1, 'A', 20);
	memset(b2, 'A', 20);
	lcpy_cmp(__LINE__, "BBBB", 0, b1, b2, 20,
		"your strlcpy copies while destsize is zero, or does not return the "
		"size of the string it tried to create");
}

static void	wm_strlcpy_un(int ligne, const char *src, size_t size,
		size_t ret, const char *att)
{
	char	*dest;
	size_t	r;

	dest = malloc(15);
	if (!dest)
		return ;
	memset(dest, 0, 15);
	memset(dest, 'r', 6);
	essai(ligne, "ft_strlcpy(\"rrrrrr\", %s, %zu)", esc(src), size);
	r = ft_strlcpy(dest, src, size);
	v_uint(ligne, r, ret, EX_STRLCPY_RET);
	v_mem(ligne, dest, att, 15, EX_STRLCPY);
	free(dest);
}

static void	wm_strlcpy(void)
{
	wm_strlcpy_un(__LINE__, "lorem", 15, 5,
		"lorem\0\0\0\0\0\0\0\0\0\0");
	wm_strlcpy_un(__LINE__, "", 15, 0,
		"\0rrrrr\0\0\0\0\0\0\0\0\0");
	wm_strlcpy_un(__LINE__, "lorem ipsum", 3, 11,
		"lo\0rrr\0\0\0\0\0\0\0\0\0");
	wm_strlcpy_un(__LINE__, "lorem ipsum dolor sit amet", 15, 26,
		"lorem ipsum do\0");
	wm_strlcpy_un(__LINE__, "lorem ipsum dolor sit amet", 0, 26,
		"rrrrrr\0\0\0\0\0\0\0\0\0");
}

/* ========================================================================== */
/*                               ft_strlcat                                   */
/* ========================================================================== */

static void	lcat_cmp(int ligne, char *b1, char *b2, const char *src,
		size_t size, size_t buf, const char *ex)
{
	size_t	r1;
	size_t	r2;

	essai(ligne, "ft_strlcat(%s, %s, %zu)", esc(b2), esc(src), size);
	r2 = ft_strlcat(b2, src, size);
	r1 = ref_strlcat(b1, src, size);
	v_uint(ligne, r2, r1, EX_STRLCAT_RET);
	v_mem(ligne, b2, b1, buf, ex);
}

static void	fs_strlcat_un(int ligne, const char *orig, const char *src, int n)
{
	char	dest[MEM_SIZE];
	char	dest_std[MEM_SIZE];

	reset_with(dest, dest_std, orig, MEM_SIZE);
	lcat_cmp(ligne, dest_std, dest, src, n, (n / 16 + 1) * 16, EX_STRLCAT);
}

static void	fs_strlcat(void)
{
	int	i;

	i = 0;
	while (i < 8)
	{
		fs_strlcat_un(__LINE__, "pqrstuvwxyz", "abcd", i);
		i++;
	}
	fs_strlcat_un(__LINE__, "pqrstuvwxyz", "abcd", 20);
	i = 10;
	while (i < 18)
	{
		fs_strlcat_un(__LINE__, "pqrs", "abcdefghi", i);
		i++;
	}
}

static void	tr_strlcat(void)
{
	char		dest[30];
	const char	*src;

	src = "AAAAAAAAA";
	memset(dest, 0, 30);
	dest[0] = 'B';
	V_UINT(ft_strlcat(dest, src, 0), 9, EX_STRLCAT_RET);
	V_STR(dest, "B", "ft_strlcat avec size = 0 ne doit rien ecrire");
	dest[0] = 'B';
	V_UINT(ft_strlcat(dest, src, 1), 10, EX_STRLCAT_RET);
	V_STR(dest, "B", "ft_strlcat avec size <= strlen(dst) ne doit rien "
		"ecrire");
	memset(dest, 'B', 4);
	V_UINT(ft_strlcat(dest, src, 3), 12, EX_STRLCAT_RET);
	V_STR(dest, "BBBB", "ft_strlcat avec size <= strlen(dst) ne doit rien "
		"ecrire");
	V_UINT(ft_strlcat(dest, src, 6), 13, EX_STRLCAT_RET);
	V_STR(dest, "BBBBA", EX_STRLCAT);
	memset(dest, 'C', 5);
	V_UINT(ft_strlcat(dest, src, -1), 14, EX_STRLCAT_RET);
	V_STR(dest, "CCCCCAAAAAAAAA", EX_STRLCAT);
	memset(dest, 'C', 15);
	V_UINT(ft_strlcat(dest, src, 17), 24, EX_STRLCAT_RET);
	V_STR(dest, "CCCCCCCCCCCCCCCA", EX_STRLCAT);
	memset(dest, 0, 30);
	V_UINT(ft_strlcat(dest, src, 1), 9, EX_STRLCAT_RET);
	V_STR(dest, "", EX_STRLCAT);
	memset(dest, 0, 30);
	memset(dest, '1', 10);
	V_UINT(ft_strlcat(dest, src, 5), 14, EX_STRLCAT_RET);
	V_STR(dest, "1111111111", "ft_strlcat avec size <= strlen(dst) ne doit "
		"rien ecrire");
	memset(dest, 0, 30);
	memset(dest, '1', 10);
	V_UINT(ft_strlcat(dest, src, 5), 14, EX_STRLCAT_RET);
	V_STR(dest, "1111111111", EX_STRLCAT);
	memset(dest, 0, 30);
	memset(dest, '1', 10);
	V_UINT(ft_strlcat(dest, "", 15), 10, EX_STRLCAT_RET);
	V_STR(dest, "1111111111", EX_STRLCAT);
	memset(dest, 0, 30);
	V_UINT(ft_strlcat(dest, "", 42), 0, EX_STRLCAT_RET);
	V_STR(dest, "", EX_STRLCAT);
	memset(dest, 0, 30);
	V_UINT(ft_strlcat(dest, "", 0), 0, EX_STRLCAT_RET);
	V_STR(dest, "", EX_STRLCAT);
	memset(dest, 0, 30);
	V_UINT(ft_strlcat(dest, "123", 1), 3, EX_STRLCAT_RET);
	V_STR(dest, "", EX_STRLCAT);
	memset(dest, 0, 30);
	V_UINT(ft_strlcat(dest, "123", 2), 3, EX_STRLCAT_RET);
	V_STR(dest, "1", EX_STRLCAT);
	memset(dest, 0, 30);
	V_UINT(ft_strlcat(dest, "123", 3), 3, EX_STRLCAT_RET);
	V_STR(dest, "12", EX_STRLCAT);
	memset(dest, 0, 30);
	V_UINT(ft_strlcat(dest, "123", 4), 3, EX_STRLCAT_RET);
	V_STR(dest, "123", EX_STRLCAT);
	memset(dest, 0, 30);
	V_UINT(ft_strlcat(dest, "123", 0), 3, EX_STRLCAT_RET);
	V_STR(dest, "", EX_STRLCAT);
}

static char	*g_lc_buf;
static char	*g_lc_src;

static void	el_strlcat(void)
{
	g_lc_src = alloc_electrique(10);
	g_lc_buf = alloc_electrique(10);
	strcpy(g_lc_buf, "AB");
	strcpy(g_lc_src, "CDEFGHI");
	lecture_seule(g_lc_src, 10);
	g_puits = ft_strlcat(g_lc_buf, g_lc_src, 10);
}

static void	al_strlcat_cas(int ligne, const char *init, const char *src,
		size_t max, const char *ex)
{
	char	b1[0xF00];
	char	b2[0xF00];

	memset(b1, 0, sizeof(b1));
	memset(b2, 0, sizeof(b2));
	strcpy(b1, init);
	strcpy(b2, init);
	lcat_cmp(ligne, b1, b2, src, max, sizeof(b1), ex);
}

static void	al_strlcat(void)
{
	char	b1[0xF00];
	char	b2[0xF00];
	char	s1[4];
	char	s2[4];
	char	d1[20];
	char	d2[20];
	size_t	r1;
	size_t	r2;

	al_strlcat_cas(__LINE__, STRING_2, STRING_1, strlen(STRING_1) + 4,
		"your strlcat does not work with basic input");
	memset(s1, 0, 4);
	memset(s2, 0, 4);
	lcat_cmp(__LINE__, s1, s2, "thx to ntoniolo for this test !", 4, 4,
		"your strlcat does not work with basic input");
	al_strlcat_cas(__LINE__, STRING_2, STRING_1,
		strlen(STRING_1) + strlen(STRING_2),
		"your strlcat does not work with basic input");
	al_strlcat_cas(__LINE__, STRING_2, STRING_1, 1000,
		"your strlcat does not work with over length size");
	al_strlcat_cas(__LINE__, STRING_2, STRING_1, 0,
		"your strlcat does not work with a size of 0");
	al_strlcat_cas(__LINE__, STRING_2, "", strlen(STRING_2) + 1,
		"your strlcat does not work with an empty src");
	al_strlcat_cas(__LINE__, "", STRING_1, strlen(STRING_1) + 1,
		"your strlcat does not work with an empty dst");
	memset(b1, 0, sizeof(b1));
	memset(b2, 0, sizeof(b2));
	memcpy(b1, "\0AAAAAAAAAAAAAAAA", 18);
	memcpy(b2, "\0AAAAAAAAAAAAAAAA", 18);
	lcat_cmp(__LINE__, b1, b2, "n\0AA", 10, sizeof(b1),
		"your strlcat does not set a \\0 to the end");
	T_SANS_CRASH(el_strlcat, "ft_strlcat(\"AB\", \"CDEFGHI\", 10) avec des "
		"buffers de 10 octets suivis d'une page interdite", EX_ELEC_LIT);
	memset(d1, 'B', sizeof(d1));
	memset(d2, 'B', sizeof(d2));
	ref_strlcat(d1, "aaa", 20);
	ref_strlcat(d1, "aaa", 20);
	r1 = ref_strlcat(d1, "aaa", 20);
	ESSAI("3 x ft_strlcat(dst sans '\\0' (20 x 'B'), \"aaa\", 20)");
	ft_strlcat(d2, "aaa", 20);
	ft_strlcat(d2, "aaa", 20);
	r2 = ft_strlcat(d2, "aaa", 20);
	V_UINT(r2, r1, "your strlcat return value is false (dst sans '\\0' dans "
		"les size premiers octets : renvoyer size + strlen(src))");
	V_MEM(d2, d1, 20, "ft_strlcat ne doit rien ecrire si dst n'a pas de "
		"'\\0' dans les size premiers octets");
}

static void	wm_strlcat_un(int ligne, int cas, const char *src, size_t size,
		size_t ret, const char *att)
{
	char	*dest;
	size_t	r;

	dest = malloc(15);
	if (!dest)
		return ;
	memset(dest, 0, 15);
	memset(dest, 'r', 6);
	if (cas == 1 || cas == 3)
		dest[11] = 'a';
	if (cas == 3)
		dest[0] = '\0';
	if (cas == 4)
		dest[14] = 'a';
	if (cas == 5 || cas == 6 || cas == 8)
		dest[10] = 'a';
	if (cas == 7)
		memset(dest, 'r', 15);
	if (cas == 9)
		memset(dest, 'r', 14);
	essai(ligne, "ft_strlcat(%s, %s, %zu)", esc_n(dest, 15), esc(src), size);
	r = ft_strlcat(dest, src, size);
	v_uint(ligne, r, ret, EX_STRLCAT_RET);
	v_mem(ligne, dest, att, 15, EX_STRLCAT);
	free(dest);
}

static void	wm_strlcat(void)
{
	const char	*l;

	l = "lorem ipsum dolor sit amet";
	wm_strlcat_un(__LINE__, 1, "lorem", 15, 11, "rrrrrrlorem\0\0\0\0");
	wm_strlcat_un(__LINE__, 2, "", 15, 6, "rrrrrr\0\0\0\0\0\0\0\0\0");
	wm_strlcat_un(__LINE__, 3, "lorem ipsum", 15, 11,
		"lorem ipsum\0\0\0\0");
	wm_strlcat_un(__LINE__, 4, l, 15, 32, "rrrrrrlorem ip\0");
	wm_strlcat_un(__LINE__, 5, l, 0, 26, "rrrrrr\0\0\0\0a\0\0\0\0");
	wm_strlcat_un(__LINE__, 6, l, 1, 27, "rrrrrr\0\0\0\0a\0\0\0\0");
	wm_strlcat_un(__LINE__, 7, l, 5, 31, "rrrrrrrrrrrrrrr");
	wm_strlcat_un(__LINE__, 8, l, 6, 32, "rrrrrr\0\0\0\0a\0\0\0\0");
	wm_strlcat_un(__LINE__, 9, l, 15, 40, "rrrrrrrrrrrrrr\0");
}

/* ========================================================================== */
/*                          ft_strchr / ft_strrchr                            */
/* ========================================================================== */

static void	strchr_un(int ligne, const char *s, int c, const char *ex)
{
	char	*r;

	essai(ligne, "ft_strchr(%s, %s)", esc(s), esc_c(c));
	r = ft_strchr(s, c);
	v_ptr(ligne, r, strchr(s, c), s, ex);
}

static void	strrchr_un(int ligne, const char *s, int c, const char *ex)
{
	char	*r;

	essai(ligne, "ft_strrchr(%s, %s)", esc(s), esc_c(c));
	r = ft_strrchr(s, c);
	v_ptr(ligne, r, strrchr(s, c), s, ex);
}

static void	fs_strchr(void)
{
	char	s[] = "teste";

	strchr_un(__LINE__, s, 't', EX_STRCHR);
	strchr_un(__LINE__, s, 'e', EX_STRCHR);
	strchr_un(__LINE__, s, '\0', "ft_strchr(s, '\\0') doit renvoyer "
		"l'adresse du '\\0' final");
	strchr_un(__LINE__, s, 'a', EX_STRCHR);
	strchr_un(__LINE__, s, 'e' + 256, "ft_strchr doit convertir c en char "
		"('e' + 256 -> 'e')");
	strchr_un(__LINE__, s, 1024, "ft_strchr doit convertir c en char "
		"(1024 -> '\\0')");
}

static void	tr_strchr(void)
{
	char	s[] = "tripouille";

	V_PTR(ft_strchr(s, 't'), s, s, EX_STRCHR);
	V_PTR(ft_strchr(s, 'l'), s + 7, s, EX_STRCHR);
	V_PTR(ft_strchr(s, 'z'), NULL, s, EX_STRCHR);
	V_PTR(ft_strchr(s, 0), s + strlen(s), s, "ft_strchr(s, 0) doit renvoyer "
		"l'adresse du '\\0' final");
	V_PTR(ft_strchr(s, 't' + 256), s, s, "ft_strchr doit convertir c en char "
		"('t' + 256 -> 't')");
}

static char	*g_ch_src;

static void	el_strchr(void)
{
	g_ch_src = alloc_electrique(10);
	strcpy(g_ch_src, "123456789");
	lecture_seule(g_ch_src, 10);
	g_puits = (size_t)ft_strchr(g_ch_src, 'a');
}

static void	al_strchr(void)
{
	const char	*u;

	strchr_un(__LINE__, STRING_1, ' ', "your strchr does not work with basic "
		"input");
	strchr_un(__LINE__, STRING_1, 't', "your strchr does not work with the "
		"first char of input");
	strchr_un(__LINE__, STRING_1, '!', "your strchr does not work with the "
		"char before \\0");
	strchr_un(__LINE__, STRING_1, 'x', "your strchr does not work with not "
		"found char");
	u = "\xc4\xab\xc5\x93\xcb\x99\xcb\x80\xcb\x98\xc2\xaf\xcb\x87\xc2\xb8"
		"\xc2\xaf.\xc5\x93\xc2\xab\xe2\x80\x98\xe2\x80\x93\xe2\x80\x93\xe2"
		"\x84\xa2\xc2\xaa\xe2\x80\xa2\xc2\xa1\xc2\xb6\xc2\xa2\xcb\x9c\xcb\x80";
	strchr_un(__LINE__, u, 0x2013, "your strchr does not work with unicode "
		"(c doit etre converti en char)");
	strchr_un(__LINE__, "AA\0B", 'B', "your strchr find chars after \\0");
	strchr_un(__LINE__, "there is so \0ma\0ny \0 \\0 in t\0his stri\0ng !\0\0\0",
		'\0', "your strchr does not work with \\0");
	strchr_un(__LINE__, "", 'a', "your strchr does not work with an empty "
		"string");
	T_SANS_CRASH(el_strchr, "ft_strchr(\"123456789\", 'a') avec s suivi "
		"d'une page interdite", EX_ELEC_LIT);
}

static void	wm_strchr(void)
{
	char	str2[] = "bonjour";
	char	str3[] = "";

	V_PTR(ft_strchr(str2, 'b'), str2, str2, EX_STRCHR);
	V_PTR(ft_strchr(str2, 'o'), str2 + 1, str2, EX_STRCHR);
	V_PTR(ft_strchr(str2, 'j'), str2 + 3, str2, EX_STRCHR);
	V_PTR(ft_strchr(str2, 's'), NULL, str2, EX_STRCHR);
	V_PTR(ft_strchr(str2, '\0'), str2 + 7, str2, "ft_strchr(s, '\\0') doit "
		"renvoyer l'adresse du '\\0' final");
	V_PTR(ft_strchr(str3, '\0'), str3, str3, "ft_strchr(\"\", '\\0') doit "
		"renvoyer l'adresse de la chaine");
}

static void	fs_strrchr(void)
{
	strrchr_un(__LINE__, "teste", 'e', EX_STRRCHR);
	strrchr_un(__LINE__, "teste", '\0', "ft_strrchr(s, '\\0') doit renvoyer "
		"l'adresse du '\\0' final");
	strrchr_un(__LINE__, "xteste", 'x', EX_STRRCHR);
	strrchr_un(__LINE__, "teste", 'x', EX_STRRCHR);
	strrchr_un(__LINE__, "teste", 1024 + 'e', "ft_strrchr doit convertir c "
		"en char (1024 + 'e' -> 'e')");
	strrchr_un(__LINE__, "teste", 1024, "ft_strrchr doit convertir c en "
		"char (1024 -> '\\0')");
	strrchr_un(__LINE__, "pepe y cparlos", 'c', EX_STRRCHR);
}

static void	tr_strrchr(void)
{
	char	s[] = "tripouille";
	char	s2[] = "ltripouiel";
	char	s3[] = "";
	char	*empty;

	V_PTR(ft_strrchr(s, 't'), s, s, EX_STRRCHR);
	V_PTR(ft_strrchr(s, 'l'), s + 8, s, EX_STRRCHR);
	V_PTR(ft_strrchr(s2, 'l'), s2 + 9, s2, EX_STRRCHR);
	V_PTR(ft_strrchr(s, 'z'), NULL, s, EX_STRRCHR);
	V_PTR(ft_strrchr(s, 0), s + strlen(s), s, "ft_strrchr(s, 0) doit "
		"renvoyer l'adresse du '\\0' final");
	V_PTR(ft_strrchr(s, 't' + 256), s, s, "ft_strrchr doit convertir c en "
		"char ('t' + 256 -> 't')");
	empty = calloc(1, 1);
	if (empty)
		V_PTR(ft_strrchr(empty, 'V'), NULL, empty, "ft_strrchr sur une chaine "
			"vide doit renvoyer NULL (et ne pas lire avant la chaine)");
	free(empty);
	V_PTR(ft_strrchr(s3, 0), s3, s3, "ft_strrchr(\"\", 0) doit renvoyer "
		"l'adresse de la chaine");
}

static void	el_strrchr(void)
{
	g_ch_src = alloc_electrique(10);
	strcpy(g_ch_src, "123456789");
	lecture_seule(g_ch_src, 10);
	g_puits = (size_t)ft_strrchr(g_ch_src, 'a');
	g_ch_src = alloc_electrique_avant(10);
	strcpy(g_ch_src, "123456789");
	lecture_seule(g_ch_src, 10);
	g_puits = (size_t)ft_strrchr(g_ch_src, 'a');
}

static void	al_strrchr(void)
{
	const char	*u;

	strrchr_un(__LINE__, STRING_1, ' ', "your strrchr does not work with "
		"basic input");
	strrchr_un(__LINE__, "abbbbbbbb", 'a', "your strrchr does not work with "
		"basic input");
	strrchr_un(__LINE__, STRING_1, 'x', "your strrchr does not work with not "
		"found char");
	u = "\xc4\xab\xc5\x93\xcb\x99\xcb\x80\xcb\x98\xc2\xaf\xcb\x87\xc2\xb8"
		"\xc2\xaf.\xc5\x93\xc2\xab\xe2\x80\x98\xe2\x80\x93\xe2\x80\x93\xe2"
		"\x84\xa2\xc2\xaa\xe2\x80\xa2\xc2\xa1\xc2\xb6\xc2\xa2\xcb\x9c\xcb\x80";
	strrchr_un(__LINE__, u, 0x2013, "your strrchr does not work with "
		"unicode (c doit etre converti en char)");
	strrchr_un(__LINE__, "there is so \0ma\0ny \0 \\0 in t\0his stri\0ng !\0\0",
		'\0', "your strrchr does not work with \\0");
	strrchr_un(__LINE__, "", 'a', "your strrchr does not work with empty "
		"string");
	T_SANS_CRASH(el_strrchr, "ft_strrchr(\"123456789\", 'a') avec des pages "
		"interdites juste avant et juste apres s",
		"ta fonction lit avant le debut de la chaine ou apres son '\\0'");
}

static void	wm_strrchr(void)
{
	char	str2[] = "bonjour";
	char	str3[] = "bonjourno";
	char	str4[] = "";

	V_PTR(ft_strrchr(str2, 'b'), str2, str2, EX_STRRCHR);
	V_PTR(ft_strrchr(str2, 'o'), str2 + 4, str2, EX_STRRCHR);
	V_PTR(ft_strrchr(str3, 'o'), str3 + 8, str3, EX_STRRCHR);
	V_PTR(ft_strrchr(str2, 'j'), str2 + 3, str2, EX_STRRCHR);
	V_PTR(ft_strrchr(str2, 's'), NULL, str2, EX_STRRCHR);
	V_PTR(ft_strrchr(str2, '\0'), str2 + 7, str2, "ft_strrchr(s, '\\0') doit "
		"renvoyer l'adresse du '\\0' final");
	V_PTR(ft_strrchr(str4, '\0'), str4, str4, "ft_strrchr(\"\", '\\0') doit "
		"renvoyer l'adresse de la chaine");
	V_PTR(ft_strrchr(str2 + 2, 'b'), NULL, str2, "ft_strrchr ne doit pas "
		"chercher avant le debut de la chaine recue");
}

/* ========================================================================== */
/*                               ft_strncmp                                   */
/* ========================================================================== */

static int	strncmp_un(int ligne, const char *s1, const char *s2, size_t n,
		const char *ex)
{
	int	r;

	essai(ligne, "ft_strncmp(%s, %s, %zu)", esc(s1), esc(s2), n);
	r = ft_strncmp(s1, s2, n);
	return (v_signe(ligne, r, strncmp(s1, s2, n), ex));
}

static void	fs_strncmp(void)
{
	unsigned char	s1[10];
	unsigned char	s2[10];

	strncmp_un(__LINE__, "", "", 0, EX_STRNCMP);
	strncmp_un(__LINE__, "", "", 1, EX_STRNCMP);
	strncmp_un(__LINE__, "", "", 2, EX_STRNCMP);
	strncmp_un(__LINE__, "test", "", 0, EX_STRNCMP);
	strncmp_un(__LINE__, "test", "", 1, EX_STRNCMP);
	strncmp_un(__LINE__, "test", "", 2, EX_STRNCMP);
	strncmp_un(__LINE__, "", "test", 0, EX_STRNCMP);
	strncmp_un(__LINE__, "", "test", 1, EX_STRNCMP);
	strncmp_un(__LINE__, "", "test", 2, EX_STRNCMP);
	strncmp_un(__LINE__, "teste", "teste", 0, EX_STRNCMP);
	strncmp_un(__LINE__, "teste", "teste", 1, EX_STRNCMP);
	strncmp_un(__LINE__, "teste", "teste", 5, EX_STRNCMP);
	strncmp_un(__LINE__, "teste", "teste", 6, EX_STRNCMP);
	strncmp_un(__LINE__, "teste", "teste", 7, EX_STRNCMP);
	strncmp_un(__LINE__, "teste", "testex", 6, EX_STRNCMP);
	strncmp_un(__LINE__, "teste", "test", 10, EX_STRNCMP);
	strncmp_un(__LINE__, "test", "teste", 10, EX_STRNCMP);
	memset(s1, 0, sizeof(s1));
	memset(s2, 0, sizeof(s2));
	memcpy(s1, "abcdef", 6);
	memcpy(s2, "abc\xfdxx", 6);
	strncmp_un(__LINE__, (char *)s1, (char *)s2, 5, "ft_strncmp doit "
		"comparer en unsigned char ('d' < 0xfd)");
	s1[3] = 0;
	s2[3] = 0;
	strncmp_un(__LINE__, (char *)s1, (char *)s2, 7, "You are not stopping "
		"at the '\\0' : ft_strncmp doit s'arreter a la fin des chaines");
}

static void	tr_strncmp(void)
{
	V_SIGNE(ft_strncmp("t", "", 0), 0, "ft_strncmp avec n = 0 doit renvoyer 0");
	V_SIGNE(ft_strncmp("1234", "1235", 3), 0, EX_STRNCMP);
	V_SIGNE(ft_strncmp("1234", "1235", 4), -1, EX_STRNCMP);
	V_SIGNE(ft_strncmp("1234", "1235", -1), -1, EX_STRNCMP);
	V_SIGNE(ft_strncmp("", "", 42), 0, EX_STRNCMP);
	V_SIGNE(ft_strncmp("Tripouille", "Tripouille", 42), 0, EX_STRNCMP);
	V_SIGNE(ft_strncmp("Tripouille", "tripouille", 42), -1, EX_STRNCMP);
	V_SIGNE(ft_strncmp("Tripouille", "TriPouille", 42), 1, EX_STRNCMP);
	V_SIGNE(ft_strncmp("Tripouille", "TripouillE", 42), 1, EX_STRNCMP);
	V_SIGNE(ft_strncmp("Tripouille", "TripouilleX", 42), -1, EX_STRNCMP);
	V_SIGNE(ft_strncmp("Tripouille", "Tripouill", 42), 1, EX_STRNCMP);
	V_SIGNE(ft_strncmp("", "1", 0), 0, "ft_strncmp avec n = 0 doit renvoyer 0");
	V_SIGNE(ft_strncmp("1", "", 0), 0, "ft_strncmp avec n = 0 doit renvoyer 0");
	V_SIGNE(ft_strncmp("", "1", 1), -1, EX_STRNCMP);
	V_SIGNE(ft_strncmp("1", "", 1), 1, EX_STRNCMP);
	V_SIGNE(ft_strncmp("", "", 1), 0, EX_STRNCMP);
}

static char	*g_nc1;
static char	*g_nc2;

static void	el_strncmp(void)
{
	g_nc1 = alloc_electrique(10);
	g_nc2 = alloc_electrique(10);
	strcpy(g_nc1, "{'-_-'} !");
	strcpy(g_nc2, "{'-_-'} !");
	lecture_seule(g_nc1, 10);
	lecture_seule(g_nc2, 10);
	g_puits = (size_t)ft_strncmp(g_nc1, g_nc2, 10);
}

static void	al_strncmp(void)
{
	strncmp_un(__LINE__, STRING_1, STRING_2, strlen(STRING_1),
		"your strncmp does not work with basic input");
	strncmp_un(__LINE__, "omg1||||||||||||||||", "omg3", 4,
		"your strncmp does not work with basic input");
	strncmp_un(__LINE__, "", "", 1, "your strncmp does not work with basic "
		"input");
	strncmp_un(__LINE__, "AAAAAAAAA1", "AAAAAAAAA2", 3,
		"your strncmp does not work with basic input");
	strncmp_un(__LINE__, "AAAAAA", "", 6, "your strncmp does not work with "
		"zero length string");
	strncmp_un(__LINE__, "", "AAAAAA", 6, "your strncmp does not work with "
		"zero length string");
	strncmp_un(__LINE__, "\200", "\0", 1, "your strncmp does not cast in "
		"unsigned char the diff");
	strncmp_un(__LINE__, "omg1", "omg3                ", 100000,
		"your strncmp does not work with a big n");
	strncmp_un(__LINE__, "atoms\0\0\0\0", "atoms\0abc", 8,
		"your strncmp doesn't stop at \\0");
	strncmp_un(__LINE__, "\x12\xff\x65\x12\xbd\xde\xad", "\x12\x02", 6,
		"your strncmp does not work with non ascii chars");
	strncmp_un(__LINE__, "abcdef", "abcdefghijklmnop", 6,
		"your strncmp does not work when s1 is contained in s2");
	T_SANS_CRASH(el_strncmp, "ft_strncmp(s1, s2, 10) avec des chaines de 10 "
		"octets suivies d'une page interdite", EX_ELEC_LIT);
}

static void	wm_strncmp(void)
{
	V_SIGNE(ft_strncmp("salut", "salut", 5), 0, EX_STRNCMP);
	V_SIGNE(ft_strncmp("test", "testss", 7), -1, EX_STRNCMP);
	V_SIGNE(ft_strncmp("testss", "test", 7), 1, EX_STRNCMP);
	V_SIGNE(ft_strncmp("test", "tEst", 4), 1, EX_STRNCMP);
	V_SIGNE(ft_strncmp("", "test", 4), -1, EX_STRNCMP);
	V_SIGNE(ft_strncmp("test", "", 4), 1, EX_STRNCMP);
	V_SIGNE(ft_strncmp("abcdefghij", "abcdefgxyz", 3), 0, EX_STRNCMP);
	V_SIGNE(ft_strncmp("abcdefgh", "abcdwxyz", 4), 0, EX_STRNCMP);
	V_SIGNE(ft_strncmp("zyxbcdefgh", "abcdwxyz", 0), 0, "ft_strncmp avec "
		"n = 0 doit renvoyer 0");
	V_SIGNE(ft_strncmp("abcdefgh", "", 0), 0, "ft_strncmp avec n = 0 doit "
		"renvoyer 0");
	V_SIGNE(ft_strncmp("test\200", "test\0", 6), 1, "ft_strncmp doit "
		"comparer en unsigned char : '\\200' > '\\0'");
}

/* ========================================================================== */
/*                               ft_strnstr                                   */
/* ========================================================================== */

static void	strnstr_un(int ligne, const char *big, const char *little,
		size_t n, const char *ex)
{
	char	*r;

	essai(ligne, "ft_strnstr(%s, %s, %zu)", esc(big), esc(little), n);
	r = ft_strnstr(big, little, n);
	v_ptr(ligne, r, ref_strnstr(big, little, n), big, ex);
}

static void	fs_strnstr(void)
{
	char			empty[10];
	unsigned char	s1[10];
	unsigned char	s2[10];

	memset(empty, 0, sizeof(empty));
	empty[1] = 'a';
	strnstr_un(__LINE__, "abc", "xyz", 0, EX_STRNSTR);
	strnstr_un(__LINE__, "abc", "xyz", 1, EX_STRNSTR);
	strnstr_un(__LINE__, "", "", 0, EX_STRNSTR);
	strnstr_un(__LINE__, "", "", 1, EX_STRNSTR);
	strnstr_un(__LINE__, "", "", 2, EX_STRNSTR);
	strnstr_un(__LINE__, "", "teste", 0, EX_STRNSTR);
	strnstr_un(__LINE__, "", "teste", 1, EX_STRNSTR);
	strnstr_un(__LINE__, "", "teste", 2, EX_STRNSTR);
	strnstr_un(__LINE__, "teste", "", 0, EX_STRNSTR);
	strnstr_un(__LINE__, "teste", "", 1, EX_STRNSTR);
	strnstr_un(__LINE__, "teste", "", 2, EX_STRNSTR);
	strnstr_un(__LINE__, "abcdefgh", "abc", 2, EX_STRNSTR);
	strnstr_un(__LINE__, "abcdefgh", "abc", 3, EX_STRNSTR);
	strnstr_un(__LINE__, "abcdefgh", "abc", 4, EX_STRNSTR);
	strnstr_un(__LINE__, "abcdefgh", "abc", 5, EX_STRNSTR);
	strnstr_un(__LINE__, "abc", "abcdef", 2, EX_STRNSTR);
	strnstr_un(__LINE__, "abc", "abcdef", 3, EX_STRNSTR);
	strnstr_un(__LINE__, "abc", "abcdef", 4, EX_STRNSTR);
	strnstr_un(__LINE__, "abc", "abcdef", 5, EX_STRNSTR);
	strnstr_un(__LINE__, "aaxx", "xx", 2, EX_STRNSTR);
	strnstr_un(__LINE__, "aaxx", "xx", 3, EX_STRNSTR);
	strnstr_un(__LINE__, "aaxx", "xx", 4, EX_STRNSTR);
	strnstr_un(__LINE__, "aaxx", "xx", 5, EX_STRNSTR);
	strnstr_un(__LINE__, "aaxx", "xx", 6, EX_STRNSTR);
	strnstr_un(__LINE__, empty, "xx", 0xffffffff, "ft_strnstr ne doit pas "
		"chercher apres le '\\0' de big");
	memset(s1, 0, sizeof(s1));
	memset(s2, 0, sizeof(s2));
	memcpy(s1, "abcdef", 6);
	memcpy(s2, "abc\xfdxx", 6);
	strnstr_un(__LINE__, (char *)s1, (char *)s2, 3, EX_STRNSTR);
	strnstr_un(__LINE__, (char *)s1, (char *)s2, 4, EX_STRNSTR);
	strnstr_un(__LINE__, (char *)s1, (char *)s2, 5, EX_STRNSTR);
	s1[3] = 0;
	s2[3] = 0;
	strnstr_un(__LINE__, (char *)s1, (char *)s2, 7, "You are not stopping at "
		"the '\\0' : little s'arrete a son '\\0'");
}

static void	tr_strnstr(void)
{
	char	haystack[30] = "aaabcabcd";
	char	needle[10] = "aabc";
	char	*empty;

	empty = "";
	V_PTR(ft_strnstr(haystack, needle, 0), NULL, haystack, EX_STRNSTR);
	V_PTR(ft_strnstr(haystack, needle, -1), haystack + 1, haystack,
		EX_STRNSTR);
	V_PTR(ft_strnstr(haystack, "a", -1), haystack, haystack, EX_STRNSTR);
	V_PTR(ft_strnstr(haystack, "c", -1), haystack + 4, haystack, EX_STRNSTR);
	V_PTR(ft_strnstr(empty, "", -1), empty, empty, "ft_strnstr(\"\", \"\", n) "
		"doit renvoyer big");
	V_PTR(ft_strnstr(empty, "", 0), empty, empty, "ft_strnstr avec little "
		"vide doit renvoyer big, meme si len = 0");
	V_PTR(ft_strnstr(empty, "coucou", -1), NULL, empty, EX_STRNSTR);
	V_PTR(ft_strnstr(haystack, "aaabc", 5), haystack, haystack, EX_STRNSTR);
	V_PTR(ft_strnstr(empty, "12345", 5), NULL, empty, EX_STRNSTR);
	V_PTR(ft_strnstr(haystack, "abcd", 9), haystack + 5, haystack,
		EX_STRNSTR);
	V_PTR(ft_strnstr(haystack, "cd", 8), NULL, haystack, "ft_strnstr : \"cd\" "
		"finit apres les len = 8 premiers caracteres, il faut renvoyer NULL");
	V_PTR(ft_strnstr(haystack, "a", 1), haystack, haystack, EX_STRNSTR);
	V_PTR(ft_strnstr("1", "a", 1), NULL, NULL, EX_STRNSTR);
	V_PTR(ft_strnstr("22", "b", 2), NULL, NULL, EX_STRNSTR);
}

static char	*g_ns1;
static char	*g_ns2;

static void	el_strnstr(void)
{
	g_ns1 = alloc_electrique(20);
	g_ns2 = alloc_electrique(20);
	strcpy(g_ns1, "is there a nyacat ?");
	strcpy(g_ns2, "is there a nyacat ?");
	lecture_seule(g_ns1, 20);
	lecture_seule(g_ns2, 20);
	g_puits = (size_t)ft_strnstr(g_ns1, g_ns2, 20);
}

static void	al_strnstr(void)
{
	const char	*a;

	a = "see FF your FF return FF now FF";
	strnstr_un(__LINE__, a, "FF", strlen(a), "your strnstr does not work "
		"with basic input");
	strnstr_un(__LINE__, "FF", "see F your F return F now F",
		strlen("see F your F return F now F"), "your strnstr does not work "
		"with basic input");
	strnstr_un(__LINE__, "MZIRIBMZIRIBMZE123", "MZIRIBMZE", strlen("MZIRIBMZE"),
		"your strnstr does not work with basic input");
	strnstr_un(__LINE__, "abcdef", "abcdefghijklmnop", strlen("abcdef"),
		"your strnstr does not work with basic input");
	strnstr_un(__LINE__, "123456789", "9", 8, "your strnstr does not work "
		"with basic input");
	strnstr_un(__LINE__, "FF", a, 4, "your strnstr does not work with not "
		"found patern");
	strnstr_un(__LINE__, "", "oh no not the empty string !",
		strlen("oh no not the empty string !"), "your strnstr does not work "
		"with empty strings");
	strnstr_un(__LINE__, "oh no not the empty string !", "",
		strlen("oh no not the empty string !"), "your strnstr does not work "
		"with empty strings");
	strnstr_un(__LINE__, "oh no not the empty string !", "", 0,
		"your strnstr does not work with empty strings and 0 length");
	a = "AAAAAAAAAAAAA";
	strnstr_un(__LINE__, a, a, strlen(a), "your strnstr does not work with "
		"the same pointer \"little\" string");
	a = "A";
	strnstr_un(__LINE__, a, a, strlen(a) + 1, "your strnstr does not work "
		"with the same pointer and an exceeding length value");
	T_SANS_CRASH(el_strnstr, "ft_strnstr(s1, s2, 20) avec des chaines de 20 "
		"octets suivies d'une page interdite", EX_ELEC_LIT);
}

static void	wm_strnstr(void)
{
	const char	l[] = "lorem ipsum dolor sit amet";
	const char	l2[] = "lorem ipsum dolor sit lorem ipsum dolor";

	V_PTR(ft_strnstr(l, "lorem", 15), l, l, EX_STRNSTR);
	V_PTR(ft_strnstr(l, "ipsum", 15), l + 6, l, EX_STRNSTR);
	V_PTR(ft_strnstr(l2, "ipsum", 35), l2 + 6, l2, EX_STRNSTR);
	V_PTR(ft_strnstr(l, "", 10), l, l, "ft_strnstr avec little vide doit "
		"renvoyer big");
	V_PTR(ft_strnstr(l, "ipsumm", 30), NULL, l, EX_STRNSTR);
	V_PTR(ft_strnstr(l, "dol", 30), l + 12, l, EX_STRNSTR);
	V_PTR(ft_strnstr(l, "consectetur", 30), NULL, l, EX_STRNSTR);
	V_PTR(ft_strnstr(l, "sit", 10), NULL, l, EX_STRNSTR);
	V_PTR(ft_strnstr(l, "dolor", 15), NULL, l, "ft_strnstr : \"dolor\" depasse "
		"les len = 15 premiers caracteres, il faut renvoyer NULL");
	V_PTR(ft_strnstr(l, "dolor", 0), NULL, l, EX_STRNSTR);
}

/* ========================================================================== */
/*                                ft_atoi                                     */
/* ========================================================================== */

static void	atoi_un(int ligne, const char *s, const char *ex)
{
	int	r;

	essai(ligne, "ft_atoi(%s)", esc(s));
	r = ft_atoi(s);
	v_int(ligne, r, atoi(s), ex);
}

static void	fs_atoi(void)
{
	char	buffer[200];
	int		i;

	atoi_un(__LINE__, " \t\v\n\r\f123", EX_ATOI);
	atoi_un(__LINE__, "0", EX_ATOI);
	atoi_un(__LINE__, "-1000043", EX_ATOI);
	atoi_un(__LINE__,
		"+0000000000000000000000000000000000000000000000000000123", EX_ATOI);
	atoi_un(__LINE__, "    123", EX_ATOI);
	atoi_un(__LINE__, "--123", "ft_atoi : un seul signe est autorise");
	atoi_un(__LINE__, "-+123", "ft_atoi : un seul signe est autorise");
	atoi_un(__LINE__, "+-123", "ft_atoi : un seul signe est autorise");
	atoi_un(__LINE__, "++123", "ft_atoi : un seul signe est autorise");
	atoi_un(__LINE__, "- 123", "ft_atoi : pas d'espace entre le signe et "
		"les chiffres");
	atoi_un(__LINE__, "+ 123", "ft_atoi : pas d'espace entre le signe et "
		"les chiffres");
	atoi_un(__LINE__, "+\n123", "ft_atoi : pas d'espace entre le signe et "
		"les chiffres");
	atoi_un(__LINE__, "1209", EX_ATOI);
	atoi_un(__LINE__, "12/3", "ft_atoi doit s'arreter au premier caractere "
		"qui n'est pas un chiffre");
	atoi_un(__LINE__, "12;3", "ft_atoi doit s'arreter au premier caractere "
		"qui n'est pas un chiffre");
	sprintf(buffer, "%i", INT_MAX);
	atoi_un(__LINE__, buffer, "ft_atoi doit gerer INT_MAX");
	sprintf(buffer, "%i", INT_MIN);
	atoi_un(__LINE__, buffer, "ft_atoi doit gerer INT_MIN");
	i = 0;
	while (i <= 0xFF)
	{
		sprintf(buffer, "%c %i", i, i + 1);
		atoi_un(__LINE__, buffer, "ft_atoi : seuls ' ' \\t \\n \\v \\f \\r sont "
			"des espaces a sauter, tout autre caractere arrete la lecture");
		i++;
	}
}

static void	tr_atoi(void)
{
	V_INT(ft_atoi("\t\n\v\f\r1"), 1, EX_ATOI);
	V_INT(ft_atoi("\t\n\v\f\ra1"), 0, EX_ATOI);
	V_INT(ft_atoi("\t\n\v\f\r--1"), 0, "ft_atoi : un seul signe est autorise");
	V_INT(ft_atoi("\t\n\v\f\r++1"), 0, "ft_atoi : un seul signe est autorise");
	V_INT(ft_atoi("\t\n\v\f\r+1"), 1, EX_ATOI);
	V_INT(ft_atoi("\t\n\v\f\r-1"), -1, EX_ATOI);
	V_INT(ft_atoi("\t\n\v\f\r0"), 0, EX_ATOI);
	V_INT(ft_atoi("\t\n\v\f\r+42lyon"), 42, EX_ATOI);
	V_INT(ft_atoi("\t\n\v\f\r+101"), 101, EX_ATOI);
	V_INT(ft_atoi("\t\n\v\f\r2147483647"), INT_MAX, "ft_atoi doit gerer "
		"INT_MAX");
	V_INT(ft_atoi("\t\n\v\f\r-2147483648"), INT_MIN, "ft_atoi doit gerer "
		"INT_MIN");
	V_INT(ft_atoi("-+42"), 0, "ft_atoi : un seul signe est autorise");
	V_INT(ft_atoi("+-42"), 0, "ft_atoi : un seul signe est autorise");
	V_INT(ft_atoi("+\t\n\v\f\r42"), 0, "ft_atoi : pas d'espace entre le "
		"signe et les chiffres");
	V_INT(ft_atoi("-\t\n\v\f\r42"), 0, "ft_atoi : pas d'espace entre le "
		"signe et les chiffres");
	V_INT(ft_atoi("1\t\n\v\f\r42"), 1, "ft_atoi doit s'arreter au premier "
		"caractere qui n'est pas un chiffre");
	V_INT(ft_atoi("-1\t\n\v\f\r42"), -1, "ft_atoi doit s'arreter au premier "
		"caractere qui n'est pas un chiffre");
}

static void	al_atoi(void)
{
	char	n[40];
	int		i;

	atoi_un(__LINE__, "0", "your atoi does not work with zero number");
	atoi_un(__LINE__, "945", "your atoi does not work with positive numbers");
	atoi_un(__LINE__, "-085", "your atoi does not work with negative numbers");
	srand(42);
	i = 0;
	while (i < 100)
	{
		sprintf(n, "%i", rand());
		atoi_un(__LINE__, n, "your atoi does not work with random numbers");
		i++;
	}
	atoi_un(__LINE__, "\t\v\f\r\n \f-06050", "your atoi is not working with "
		"blanks");
	atoi_un(__LINE__, "\t\v\f\r\n \f- \f\t\n\r    06050", "your atoi is not "
		"working with blanks (pas d'espace apres le signe)");
	atoi_un(__LINE__, "\x1b" "06050", "your atoi is not working with non "
		"space invisible char (\\e n'est pas un espace)");
	atoi_un(__LINE__, "-123THERE IS A NYANCAT UNDER YOUR BED",
		"ft_atoi doit s'arreter au premier caractere qui n'est pas un chiffre");
	sprintf(n, "%i", INT_MAX);
	atoi_un(__LINE__, n, "your atoi does not work with int max value");
	sprintf(n, "%i", INT_MIN);
	atoi_un(__LINE__, n, "your atoi does not work with int min value");
	atoi_un(__LINE__, "+1234", "your atoi does not work with the plus sign");
	atoi_un(__LINE__, "\t\v\f\r\n \f+\t\v\f\r\n \f1234", "your atoi does not "
		"work with the plus sign and spaces");
	atoi_un(__LINE__, "+\t\v\f\r\n \f1234", "your atoi does not work with "
		"the plus sign and spaces");
}

static void	wm_atoi(void)
{
	V_INT(ft_atoi("0"), 0, EX_ATOI);
	V_INT(ft_atoi("546:5"), 546, EX_ATOI);
	V_INT(ft_atoi("-4886"), -4886, EX_ATOI);
	V_INT(ft_atoi("+548"), 548, EX_ATOI);
	V_INT(ft_atoi("054854"), 54854, EX_ATOI);
	V_INT(ft_atoi("000074"), 74, EX_ATOI);
	V_INT(ft_atoi("+-54"), 0, "ft_atoi : un seul signe est autorise");
	V_INT(ft_atoi("-+48"), 0, "ft_atoi : un seul signe est autorise");
	V_INT(ft_atoi("--47"), 0, "ft_atoi : un seul signe est autorise");
	V_INT(ft_atoi("++47"), 0, "ft_atoi : un seul signe est autorise");
	V_INT(ft_atoi("+47+5"), 47, EX_ATOI);
	V_INT(ft_atoi("-47-5"), -47, EX_ATOI);
	V_INT(ft_atoi("\x1b" "475"), 0, "ft_atoi : \\e (27) n'est pas un espace");
	V_INT(ft_atoi("\t\n\r\v\f  469 \n"), 469, EX_ATOI);
	V_INT(ft_atoi("-2147483648"), INT_MIN, "ft_atoi doit gerer INT_MIN");
	V_INT(ft_atoi("2147483647"), INT_MAX, "ft_atoi doit gerer INT_MAX");
	V_INT(ft_atoi("\t\n\r\v\fd469 \n"), 0, EX_ATOI);
	V_INT(ft_atoi("\n\n\n  -46\b9 \n5d6"), -46, EX_ATOI);
	V_INT(ft_atoi(""), 0, "ft_atoi(\"\") doit renvoyer 0");
}

/* ========================================================================== */
/*                               ft_calloc                                    */
/* ========================================================================== */

static void	calloc_un(int ligne, size_t nmemb, size_t size, int non_null,
		const char *ex)
{
	void	*r;
	void	*zero;
	size_t	total;

	total = nmemb * size;
	essai(ligne, "ft_calloc(%zu, %zu)", nmemb, size);
	suivi_debut(0);
	r = ft_calloc(nmemb, size);
	suivi_fin();
	if (non_null || total)
		verif(ligne, r != NULL, EX_CALLOC_NULL, "obtenu : NULL");
	if (r && total)
	{
		zero = calloc(1, total);
		if (zero)
			v_mem(ligne, r, zero, total, ex);
		free(zero);
		t_taille_verif(ligne, r, total, "bloc renvoye par ft_calloc");
	}
	free(r);
	t_fuites_verif(ligne, "ft_calloc");
}

static void	fs_calloc(void)
{
	calloc_un(__LINE__, 0, 10, 0, EX_CALLOC);
	calloc_un(__LINE__, 10, 0, 0, EX_CALLOC);
	calloc_un(__LINE__, 10, sizeof(long), 1, EX_CALLOC);
	T_PROTECTION(ft_calloc(10, sizeof(long)), free);
}

static void	tr_calloc(void)
{
	calloc_un(__LINE__, 2, 2, 1, "ft_calloc(2, 2) doit renvoyer 4 octets a 0");
	T_PROTECTION(ft_calloc(2, 2), free);
}

static void	al_calloc(void)
{
	void	*d;

	ESSAI("free(ft_calloc(42, 1))");
	d = ft_calloc(42, 1);
	V_OK(d != NULL, "your calloc don't allocate memory");
	free(d);
	ESSAI("ft_calloc(0, 0)");
	d = ft_calloc(0, 0);
	V_OK(d != NULL, "your calloc don't work with 0 size : ft_calloc(0, 0) "
		"doit renvoyer un pointeur liberable (comme la libc), pas NULL");
	free(d);
	calloc_un(__LINE__, 8539, sizeof(int), 1, "ft_calloc doit mettre TOUTE "
		"la zone a 0 (la memoire de malloc contient des dechets)");
	calloc_un(__LINE__, 42 * 3, sizeof(char), 1, "your calloc did not "
		"allocate the good size");
	ESSAI("ft_calloc(SIZE_MAX, 2)");
	d = ft_calloc(SIZE_MAX, 2);
	V_OK(d == NULL, "ft_calloc doit renvoyer NULL si nmemb * size deborde "
		"(SIZE_MAX * 2)");
	free(d);
	ESSAI("ft_calloc(ULONG_MAX, 1)");
	d = ft_calloc(ULONG_MAX, 1);
	V_OK(d == NULL, "your calloc does not work with too large numbers");
	free(d);
	ESSAI("ft_calloc(1, ULONG_MAX)");
	d = ft_calloc(1, ULONG_MAX);
	V_OK(d == NULL, "your calloc does not work with too large numbers");
	free(d);
	if (g_malloc_dispo)
	{
		ESSAI("ft_calloc(2, 1073741829)  [malloc force a NULL]");
		suivi_debut(1);
		d = ft_calloc(2, 1073741829);
		suivi_fin();
		V_OK(suivi_nb_malloc() >= 1, "ft_calloc(2, 1073741829) doit appeler "
			"malloc(2147483658) : ce total depasse INT_MAX mais tient dans "
			"un size_t, ce n'est PAS un debordement");
		if (suivi_nb_malloc() >= 1)
			V_UINT(suivi_derniere_taille(), 2147483658ULL, "ft_calloc doit "
				"calculer nmemb * size en size_t (ne pas passer par un int)");
		V_OK(d == NULL, "ft_calloc doit renvoyer NULL quand malloc echoue");
		free(d);
	}
}

static void	wm_calloc(void)
{
	char	*str;
	char	zero[30];

	memset(zero, 0, sizeof(zero));
	ESSAI("ft_calloc(30, 1)");
	suivi_debut(0);
	str = ft_calloc(30, 1);
	suivi_fin();
	V_OK(str != NULL, EX_CALLOC_NULL);
	if (str)
		V_MEM(str, zero, 30, EX_CALLOC);
	free(str);
}

/* ========================================================================== */
/*                               ft_strdup                                    */
/* ========================================================================== */

static void	fs_strdup(void)
{
	T_ALLOC_STR(ft_strdup(""), "", EX_STRDUP);
	T_ALLOC_STR(ft_strdup("sadfvbf"), "sadfvbf", EX_STRDUP);
	T_ALLOC_STR(ft_strdup("fdfjkdf\n35346"), "fdfjkdf\n35346", EX_STRDUP);
}

static void	tr_strdup(void)
{
	T_ALLOC_STR(ft_strdup("coucou"), "coucou", EX_STRDUP);
	T_ALLOC_STR(ft_strdup(""), "", EX_STRDUP);
}

static char	*g_sd;

static void	el_strdup(void)
{
	char	*r;

	g_sd = alloc_electrique(10);
	strcpy(g_sd, "\xd\xe\xa\xd\xb\xe\xe\xf.");
	lecture_seule(g_sd, 10);
	r = ft_strdup(g_sd);
	free(r);
}

static void	al_strdup(void)
{
	T_ALLOC_STR(ft_strdup("HAHAHA \0 tu me vois pas !"), "HAHAHA ",
		"your strdup does not add \\0 at the end of the string");
	T_ALLOC_STR(ft_strdup(""), "", "your strdup don't work with empty string");
	T_ALLOC_STR(ft_strdup("this is a normal test"), "this is a normal test",
		"your strdup did not allocate the good size");
	T_ALLOC_STR(ft_strdup("I malloc so I am."), "I malloc so I am.",
		"your strdup doesn't work with basic input");
	T_SANS_CRASH(el_strdup, "ft_strdup(s) avec s = 10 octets suivis d'une "
		"page interdite", EX_ELEC_LIT);
}

static void	wm_strdup(void)
{
	char	str[] = "lorem ipsum dolor sit amet";
	char	*dup;

	ESSAI("ft_strdup(\"lorem ipsum dolor sit amet\")");
	dup = ft_strdup(str);
	V_STR(dup, str, EX_STRDUP);
	V_OK(dup != str, "str_dup's adress == str's adress : ft_strdup doit "
		"renvoyer une NOUVELLE zone memoire");
	if (dup != str)
		free(dup);
}

/* ========================================================================== */
/*                                  TABLE                                     */
/* ========================================================================== */

const t_test_fr	g_fr_part1[] = {
{"ft_isalpha", "fsoares", fs_isalpha},
{"ft_isalpha", "Tripouille", tr_isalpha},
{"ft_isalpha", "alelievr", al_isalpha},
{"ft_isalpha", "war-machine", wm_isalpha},
{"ft_isdigit", "fsoares", fs_isdigit},
{"ft_isdigit", "Tripouille", tr_isdigit},
{"ft_isdigit", "alelievr", al_isdigit},
{"ft_isdigit", "war-machine", wm_isdigit},
{"ft_isalnum", "fsoares", fs_isalnum},
{"ft_isalnum", "Tripouille", tr_isalnum},
{"ft_isalnum", "alelievr", al_isalnum},
{"ft_isalnum", "war-machine", wm_isalnum},
{"ft_isascii", "fsoares", fs_isascii},
{"ft_isascii", "Tripouille", tr_isascii},
{"ft_isascii", "alelievr", al_isascii},
{"ft_isascii", "war-machine", wm_isascii},
{"ft_isprint", "fsoares", fs_isprint},
{"ft_isprint", "Tripouille", tr_isprint},
{"ft_isprint", "alelievr", al_isprint},
{"ft_isprint", "war-machine", wm_isprint},
{"ft_toupper", "fsoares", fs_toupper},
{"ft_toupper", "Tripouille", tr_toupper},
{"ft_toupper", "alelievr", al_toupper},
{"ft_toupper", "war-machine", wm_toupper},
{"ft_tolower", "fsoares", fs_tolower},
{"ft_tolower", "Tripouille", tr_tolower},
{"ft_tolower", "alelievr", al_tolower},
{"ft_tolower", "war-machine", wm_tolower},
{"ft_strlen", "fsoares", fs_strlen},
{"ft_strlen", "Tripouille", tr_strlen},
{"ft_strlen", "alelievr", al_strlen},
{"ft_strlen", "war-machine", wm_strlen},
{"ft_memset", "fsoares", fs_memset},
{"ft_memset", "Tripouille", tr_memset},
{"ft_memset", "alelievr", al_memset},
{"ft_memset", "war-machine", wm_memset},
{"ft_bzero", "fsoares", fs_bzero},
{"ft_bzero", "Tripouille", tr_bzero},
{"ft_bzero", "alelievr", al_bzero},
{"ft_bzero", "war-machine", wm_bzero},
{"ft_memcpy", "fsoares", fs_memcpy},
{"ft_memcpy", "Tripouille", tr_memcpy},
{"ft_memcpy", "alelievr", al_memcpy},
{"ft_memcpy", "war-machine", wm_memcpy},
{"ft_memmove", "fsoares", fs_memmove},
{"ft_memmove", "Tripouille", tr_memmove},
{"ft_memmove", "alelievr", al_memmove},
{"ft_memmove", "war-machine", wm_memmove},
{"ft_memchr", "fsoares", fs_memchr},
{"ft_memchr", "Tripouille", tr_memchr},
{"ft_memchr", "alelievr", al_memchr},
{"ft_memchr", "war-machine", wm_memchr},
{"ft_memcmp", "fsoares", fs_memcmp},
{"ft_memcmp", "Tripouille", tr_memcmp},
{"ft_memcmp", "alelievr", al_memcmp},
{"ft_memcmp", "war-machine", wm_memcmp},
{"ft_strlcpy", "fsoares", fs_strlcpy},
{"ft_strlcpy", "Tripouille", tr_strlcpy},
{"ft_strlcpy", "alelievr", al_strlcpy},
{"ft_strlcpy", "war-machine", wm_strlcpy},
{"ft_strlcat", "fsoares", fs_strlcat},
{"ft_strlcat", "Tripouille", tr_strlcat},
{"ft_strlcat", "alelievr", al_strlcat},
{"ft_strlcat", "war-machine", wm_strlcat},
{"ft_strchr", "fsoares", fs_strchr},
{"ft_strchr", "Tripouille", tr_strchr},
{"ft_strchr", "alelievr", al_strchr},
{"ft_strchr", "war-machine", wm_strchr},
{"ft_strrchr", "fsoares", fs_strrchr},
{"ft_strrchr", "Tripouille", tr_strrchr},
{"ft_strrchr", "alelievr", al_strrchr},
{"ft_strrchr", "war-machine", wm_strrchr},
{"ft_strncmp", "fsoares", fs_strncmp},
{"ft_strncmp", "Tripouille", tr_strncmp},
{"ft_strncmp", "alelievr", al_strncmp},
{"ft_strncmp", "war-machine", wm_strncmp},
{"ft_strnstr", "fsoares", fs_strnstr},
{"ft_strnstr", "Tripouille", tr_strnstr},
{"ft_strnstr", "alelievr", al_strnstr},
{"ft_strnstr", "war-machine", wm_strnstr},
{"ft_atoi", "fsoares", fs_atoi},
{"ft_atoi", "Tripouille", tr_atoi},
{"ft_atoi", "alelievr", al_atoi},
{"ft_atoi", "war-machine", wm_atoi},
{"ft_calloc", "fsoares", fs_calloc},
{"ft_calloc", "Tripouille", tr_calloc},
{"ft_calloc", "alelievr", al_calloc},
{"ft_calloc", "war-machine", wm_calloc},
{"ft_strdup", "fsoares", fs_strdup},
{"ft_strdup", "Tripouille", tr_strdup},
{"ft_strdup", "alelievr", al_strdup},
{"ft_strdup", "war-machine", wm_strdup},
{NULL, NULL, NULL}
};
