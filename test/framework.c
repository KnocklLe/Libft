/*
** framework.c - implementation du socle de tests (voir test.h).
** Ce fichier n'est PAS a rendre : il n'est pas a la norme.
*/
#define _GNU_SOURCE
#define TEST_FRAMEWORK
#include "test.h"

#ifndef MAP_ANONYMOUS
# define MAP_ANONYMOUS MAP_ANON
#endif

#if defined(__linux__) && defined(__GLIBC__)
# define LIGNES_DISPO 1
# include <dlfcn.h>
# include <execinfo.h>
#else
# define LIGNES_DISPO 0
#endif

t_bac		*g_bac;
const char	*g_test_fichier = "";

/* ========================================================================== */
/*            LIGNE DANS TON CODE : pile d'appels -> fichier:ligne            */
/* ========================================================================== */

static char			g_exe[1024];
static uintptr_t	g_base;

/* Chemin de l'executable et decalage de chargement (PIE). */
static void	lignes_init(void)
{
#if LIGNES_DISPO
	ssize_t			n;
	int				fd;
	unsigned char	entete[18];
	Dl_info			info;
	void			*tmp[2];

	n = readlink("/proc/self/exe", g_exe, sizeof(g_exe) - 1);
	if (n <= 0)
	{
		g_exe[0] = '\0';
		return ;
	}
	g_exe[n] = '\0';
	g_base = 0;
	fd = open(g_exe, O_RDONLY);
	if (fd >= 0)
	{
		if (read(fd, entete, sizeof(entete)) == (ssize_t)sizeof(entete)
			&& entete[16] == 3 && dladdr((void *)lignes_init, &info))
			g_base = (uintptr_t)info.dli_fbase;
		close(fd);
	}
	backtrace(tmp, 2);
#endif
}

/* Enleve le "\n" final et le " (discriminator N)" d'addr2line. */
static void	nettoyer_ligne(char *s)
{
	char	*p;

	p = strchr(s, '\n');
	if (p)
		*p = '\0';
	p = strstr(s, " (discriminator");
	if (p)
		*p = '\0';
}

int	lignes_de_ton_code(void *const *pile, int n, char *dst, size_t taille)
{
#if LIGNES_DISPO
	char	cmd[4096];
	char	fonction[256];
	char	lieu[1024];
	char	racine[1024];
	size_t	pos;
	size_t	lr;
	FILE	*f;
	int		i;
	int		trouve;
	char	*rel;

	dst[0] = '\0';
	if (!g_exe[0] || n <= 0 || !getcwd(racine, sizeof(racine)))
		return (0);
	lr = strlen(racine);
	pos = (size_t)snprintf(cmd, sizeof(cmd), "addr2line -f -e '%s'", g_exe);
	for (i = 0; i < n && pos < sizeof(cmd) - 40; i++)
		pos += (size_t)snprintf(cmd + pos, sizeof(cmd) - pos, " 0x%lx",
				(unsigned long)((uintptr_t)pile[i] - g_base - 1));
	snprintf(cmd + pos, sizeof(cmd) - pos, " 2>/dev/null");
	f = popen(cmd, "r");
	if (!f)
		return (0);
	trouve = 0;
	pos = 0;
	while (fgets(fonction, sizeof(fonction), f)
		&& fgets(lieu, sizeof(lieu), f))
	{
		nettoyer_ligne(fonction);
		nettoyer_ligne(lieu);
		if (trouve >= 4 || strncmp(lieu, racine, lr) != 0 || lieu[lr] != '/')
			continue ;
		rel = lieu + lr + 1;
		if (strchr(rel, '/') || !strncmp(rel, "??", 2))
			continue ;
		if (pos < taille)
			pos += (size_t)snprintf(dst + pos, taille - pos, "%s%s (%s)",
					trouve ? "\n" : "", rel, fonction);
		trouve++;
	}
	pclose(f);
	return (trouve);
#else
	(void)pile;
	(void)n;
	if (taille)
		dst[0] = '\0';
	return (0);
#endif
}

/* Ajoute au journal "dans ton code : ft_x.c:12 (ft_x)" + les appelants. */
static void	journal_lignes(const char *titre, void *const *pile, int n)
{
	char		lignes[2048];
	const char	*p;
	const char	*fin;
	int			premier;

	if (!lignes_de_ton_code(pile, n, lignes, sizeof(lignes)))
		return ;
	premier = 1;
	p = lignes;
	while (*p)
	{
		fin = strchr(p, '\n');
		if (!fin)
			fin = p + strlen(p);
		if (premier)
			journal("        %s : " CYAN "%.*s" NC "\n", titre,
				(int)(fin - p), p);
		else
			journal("        %*s   <- appelee depuis " CYAN "%.*s" NC "\n",
				(int)strlen(titre), "", (int)(fin - p), p);
		premier = 0;
		p = *fin ? fin + 1 : fin;
	}
}

/* ========================================================================== */
/*                           MEMOIRE PARTAGEE                                 */
/* ========================================================================== */

void	bac_init(void)
{
	void	*p;

	p = mmap(NULL, sizeof(t_bac), PROT_READ | PROT_WRITE,
			MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	if (p == MAP_FAILED)
	{
		perror("mmap");
		exit(2);
	}
	memset(p, 0, sizeof(t_bac));
	g_bac = p;
	lignes_init();
}

void	bac_reset_groupe(const char *fonction, const char *source)
{
	g_bac->tests = 0;
	g_bac->erreurs = 0;
	g_bac->affiches = 0;
	g_bac->ouvert = 0;
	g_bac->ligne = 0;
	g_bac->appel[0] = '\0';
	g_bac->fichier[0] = '\0';
	snprintf(g_bac->fonction, sizeof(g_bac->fonction), "%s",
		fonction ? fonction : "");
	snprintf(g_bac->source, sizeof(g_bac->source), "%s",
		source ? source : "");
	g_bac->taille_journal = 0;
	g_bac->journal[0] = '\0';
}

void	journal(const char *fmt, ...)
{
	va_list	args;
	size_t	reste;
	int		n;

	if (g_bac->taille_journal >= JOURNAL_MAX - 1)
		return ;
	reste = JOURNAL_MAX - g_bac->taille_journal;
	va_start(args, fmt);
	n = vsnprintf(g_bac->journal + g_bac->taille_journal, reste, fmt, args);
	va_end(args);
	if (n < 0)
		return ;
	if ((size_t)n >= reste)
		g_bac->taille_journal = JOURNAL_MAX - 1;
	else
		g_bac->taille_journal += (size_t)n;
}

void	bac_noter_ko(const char *nom)
{
	char	cle[80];
	size_t	l;

	snprintf(cle, sizeof(cle), " %s ", nom);
	if (strstr(g_bac->liste_ko, cle))
		return ;
	l = strlen(g_bac->liste_ko);
	if (l == 0)
		snprintf(g_bac->liste_ko, sizeof(g_bac->liste_ko), " %s ", nom);
	else if (l + strlen(nom) + 2 < sizeof(g_bac->liste_ko))
		snprintf(g_bac->liste_ko + l, sizeof(g_bac->liste_ko) - l,
			"%s ", nom);
}

/* ========================================================================== */
/*                             ECHAPPEMENT                                    */
/* ========================================================================== */

#define NB_BUF 8
#define TAILLE_BUF 320
#define MAX_AFF 70

static char	*buffer_tournant(void)
{
	static char	bufs[NB_BUF][TAILLE_BUF];
	static int	i;

	i = (i + 1) % NB_BUF;
	return (bufs[i]);
}

static size_t	ajouter_car(char *dst, size_t pos, unsigned char c)
{
	const char	*s;
	char		tmp[8];

	s = NULL;
	if (c == '\n')
		s = "\\n";
	else if (c == '\t')
		s = "\\t";
	else if (c == '\r')
		s = "\\r";
	else if (c == '\v')
		s = "\\v";
	else if (c == '\f')
		s = "\\f";
	else if (c == '\0')
		s = "\\0";
	else if (c == '\\')
		s = "\\\\";
	else if (c == '"')
		s = "\\\"";
	else if (c < 32 || c >= 127)
	{
		snprintf(tmp, sizeof(tmp), "\\x%02x", c);
		s = tmp;
	}
	if (!s)
	{
		dst[pos++] = (char)c;
		return (pos);
	}
	while (*s)
		dst[pos++] = *s++;
	return (pos);
}

static const char	*echapper(const unsigned char *m, size_t n, int borne)
{
	char	*dst;
	size_t	pos;
	size_t	i;

	dst = buffer_tournant();
	pos = 0;
	dst[pos++] = '"';
	i = 0;
	while ((borne ? i < n : m[i] != '\0') && i < MAX_AFF)
		pos = ajouter_car(dst, pos, m[i++]);
	dst[pos++] = '"';
	if (borne ? i < n : m[i] != '\0')
	{
		memcpy(dst + pos, "...", 3);
		pos += 3;
	}
	dst[pos] = '\0';
	return (dst);
}

const char	*esc(const char *s)
{
	if (!s)
		return ("NULL");
	return (echapper((const unsigned char *)s, 0, 0));
}

const char	*esc_n(const void *m, size_t n)
{
	if (!m)
		return ("NULL");
	return (echapper((const unsigned char *)m, n, 1));
}

const char	*esc_c(int c)
{
	char	*dst;
	size_t	pos;

	dst = buffer_tournant();
	if (c < -128 || c > 255)
	{
		snprintf(dst, TAILLE_BUF, "%d", c);
		return (dst);
	}
	pos = 0;
	dst[pos++] = '\'';
	pos = ajouter_car(dst, pos, (unsigned char)c);
	dst[pos++] = '\'';
	snprintf(dst + pos, TAILLE_BUF - pos, " (%d)", c);
	return (dst);
}

/* ========================================================================== */
/*                             VERIFICATIONS                                  */
/* ========================================================================== */

static void	noter_fichier(void)
{
	const char	*f;

	f = g_test_fichier ? g_test_fichier : "";
	snprintf(g_bac->fichier, sizeof(g_bac->fichier), "%s", f);
}

void	essai(int ligne, const char *fmt, ...)
{
	va_list	args;

	noter_fichier();
	g_bac->ligne = ligne;
	va_start(args, fmt);
	vsnprintf(g_bac->appel, sizeof(g_bac->appel), fmt, args);
	va_end(args);
}

void	essai_expr(int ligne, const char *texte)
{
	noter_fichier();
	g_bac->ligne = ligne;
	if (texte && strstr(texte, "ft_"))
		snprintf(g_bac->appel, sizeof(g_bac->appel), "%s", texte);
}

static void	journal_details(const char *details)
{
	const char	*fin;

	while (details && *details)
	{
		fin = strchr(details, '\n');
		if (!fin)
		{
			journal("        %s\n", details);
			return ;
		}
		journal("        %.*s\n", (int)(fin - details), details);
		details = fin + 1;
	}
}

int	verif(int ligne, int ok, const char *explication, const char *fmt, ...)
{
	va_list	args;
	char	details[2048];

	noter_fichier();
	g_bac->tests++;
	g_bac->ligne = ligne;
	if (ok)
		return (1);
	g_bac->erreurs++;
	if (g_bac->affiches >= MAX_AFFICHES)
		return (0);
	g_bac->affiches++;
	journal("    " ROUGE "x" NC " " CYAN "%s:%d" NC, g_bac->fichier, ligne);
	if (g_bac->source[0])
		journal(" " MAGENTA "[%s]" NC, g_bac->source);
	if (g_bac->appel[0])
		journal(" " GRAS "%s" NC, g_bac->appel);
	journal("\n");
	details[0] = '\0';
	if (fmt)
	{
		va_start(args, fmt);
		vsnprintf(details, sizeof(details), fmt, args);
		va_end(args);
	}
	journal_details(details);
	if (explication && *explication)
		journal("      " JAUNE "-> %s" NC "\n", explication);
	return (0);
}

int	v_int(int ligne, long long obtenu, long long attendu, const char *expl)
{
	return (verif(ligne, obtenu == attendu, expl,
			"attendu : %lld\nobtenu  : %lld", attendu, obtenu));
}

int	v_uint(int ligne, unsigned long long obtenu, unsigned long long attendu,
		const char *expl)
{
	return (verif(ligne, obtenu == attendu, expl,
			"attendu : %llu\nobtenu  : %llu", attendu, obtenu));
}

int	v_bool(int ligne, long long obtenu, long long attendu, const char *expl)
{
	return (verif(ligne, !obtenu == !attendu, expl,
			"attendu : %s\nobtenu  : %lld (%s)",
			attendu ? "vrai (valeur non nulle)" : "faux (0)", obtenu,
			obtenu ? "vrai" : "faux"));
}

static const char	*nom_signe(long long v)
{
	if (v < 0)
		return ("negatif");
	if (v > 0)
		return ("positif");
	return ("nul (0)");
}

int	v_signe(int ligne, long long obtenu, long long attendu, const char *expl)
{
	int	s1;
	int	s2;

	s1 = (obtenu > 0) - (obtenu < 0);
	s2 = (attendu > 0) - (attendu < 0);
	return (verif(ligne, s1 == s2, expl,
			"attendu : un resultat %s (reference : %lld)\nobtenu  : %lld",
			nom_signe(attendu), attendu, obtenu));
}

int	v_str(int ligne, const char *obtenu, const char *attendu,
		const char *expl)
{
	int	ok;

	if (!obtenu || !attendu)
		ok = (obtenu == attendu);
	else
		ok = (strcmp(obtenu, attendu) == 0);
	return (verif(ligne, ok, expl, "attendu : %s\nobtenu  : %s",
			esc(attendu), esc(obtenu)));
}

static const char	*decrire_ptr(const void *p, const void *base)
{
	char		*dst;
	ptrdiff_t	d;

	dst = buffer_tournant();
	if (!p)
		return ("NULL");
	if (base)
	{
		d = (const char *)p - (const char *)base;
		if (d >= -4096 && d <= 1 << 20)
		{
			snprintf(dst, TAILLE_BUF, "debut + %td (%p)", d, p);
			return (dst);
		}
	}
	snprintf(dst, TAILLE_BUF, "%p", p);
	return (dst);
}

int	v_ptr(int ligne, const void *obtenu, const void *attendu,
		const void *base, const char *expl)
{
	return (verif(ligne, obtenu == attendu, expl,
			"attendu : %s\nobtenu  : %s", decrire_ptr(attendu, base),
			decrire_ptr(obtenu, base)));
}

int	v_mem(int ligne, const void *obtenu, const void *attendu, size_t n,
		const char *expl)
{
	const unsigned char	*o;
	const unsigned char	*a;
	size_t				i;
	size_t				debut;
	size_t				lg;

	o = obtenu;
	a = attendu;
	if (!o || !a)
		return (verif(ligne, o == a, expl, "attendu : %s\nobtenu  : %s",
				a ? "un pointeur valide" : "NULL",
				o ? "un pointeur valide" : "NULL"));
	i = 0;
	while (i < n && o[i] == a[i])
		i++;
	if (i == n)
		return (verif(ligne, 1, expl, NULL));
	debut = i > 8 ? i - 8 : 0;
	lg = n - debut < 40 ? n - debut : 40;
	return (verif(ligne, 0, expl,
			"premier octet different : index %zu (attendu %s, obtenu %s)\n"
			"attendu (a partir de l'index %zu) : %s\n"
			"obtenu  (a partir de l'index %zu) : %s",
			i, esc_c(a[i]), esc_c(o[i]), debut, esc_n(a + debut, lg),
			debut, esc_n(o + debut, lg)));
}

/* ========================================================================== */
/*                         SUIVI DE MALLOC / FREE                             */
/* ========================================================================== */

#ifdef TEST_WRAP_MALLOC

const int	g_malloc_dispo = 1;
#else

const int	g_malloc_dispo = 0;
#endif

#define TRACES_ALLOC 65536

int				g_trace_active;
size_t			g_total_alloc;
size_t			g_nombres_echecs;
static size_t	g_echec_au;
static size_t	g_derniere_taille;
static size_t	g_nb_traces;
static void		*g_traces_ptr[TRACES_ALLOC];
static size_t	g_traces_taille[TRACES_ALLOC];
static char		g_traces_vivant[TRACES_ALLOC];
static void		*g_traces_appel[TRACES_ALLOC];

void	suivi_debut(size_t echec_au)
{
	g_trace_active = 0;
	g_nb_traces = 0;
	g_total_alloc = 0;
	g_echec_au = echec_au;
	g_nombres_echecs = 0;
	g_derniere_taille = 0;
	g_trace_active = 1;
}

void	suivi_fin(void)
{
	g_trace_active = 0;
}

size_t	suivi_vivantes(void)
{
	size_t	i;
	size_t	n;

	n = 0;
	i = 0;
	while (i < g_nb_traces)
		n += (g_traces_vivant[i++] != 0);
	return (n);
}

size_t	suivi_nb_malloc(void)
{
	return (g_total_alloc);
}

long long	suivi_taille(const void *p)
{
	size_t	i;

	i = g_nb_traces;
	while (p && i > 0)
	{
		i--;
		if (g_traces_ptr[i] == p)
			return ((long long)g_traces_taille[i]);
	}
	return (-1);
}

size_t	suivi_derniere_taille(void)
{
	return (g_derniere_taille);
}

#ifdef TEST_WRAP_MALLOC

void	*__wrap_malloc(size_t taille)
{
	void	*ptr;

	if (g_trace_active)
	{
		g_total_alloc++;
		g_derniere_taille = taille;
		if (g_echec_au && g_total_alloc == g_echec_au)
		{
			g_nombres_echecs++;
			return (NULL);
		}
	}
	ptr = __real_malloc(taille);
	if (g_trace_active && ptr)
	{
		if (taille < (1u << 20))
			memset(ptr, '#', taille);
		if (g_nb_traces < TRACES_ALLOC)
		{
			g_traces_ptr[g_nb_traces] = ptr;
			g_traces_taille[g_nb_traces] = taille;
			g_traces_vivant[g_nb_traces] = 1;
			g_traces_appel[g_nb_traces] = __builtin_return_address(0);
			g_nb_traces++;
		}
	}
	return (ptr);
}

void	__wrap_free(void *ptr)
{
	size_t	i;

	i = g_nb_traces;
	while (ptr && i > 0)
	{
		i--;
		if (g_traces_ptr[i] == ptr && g_traces_vivant[i])
		{
			g_traces_vivant[i] = 0;
			break ;
		}
	}
	__real_free(ptr);
}

void	*malloc_reel(size_t taille)
{
	return (__real_malloc(taille));
}

void	free_reel(void *p)
{
	__real_free(p);
}
#else

void	*malloc_reel(size_t taille)
{
	return (malloc(taille));
}

void	free_reel(void *p)
{
	free(p);
}
#endif

/*
** Pour les 3 premiers blocs non liberes : taille + ligne du malloc.
** "dans ton code" si le malloc est dans un .c de la racine, sinon c'est un
** bloc cree par le test que ta fonction aurait du liberer (ex: lstclear).
*/
static void	decrire_fuites(char *dst, size_t taille)
{
	size_t	i;
	size_t	pos;
	int		n;
	char	lieu[512];
	char	*nl;

	dst[0] = '\0';
	pos = 0;
	n = 0;
	for (i = 0; i < g_nb_traces && n < 3 && pos < taille; i++)
	{
		if (!g_traces_vivant[i])
			continue ;
		if (lignes_de_ton_code(&g_traces_appel[i], 1, lieu, sizeof(lieu)))
		{
			nl = strchr(lieu, '\n');
			if (nl)
				*nl = '\0';
			pos += (size_t)snprintf(dst + pos, taille - pos,
					"\nbloc de %zu octet(s) alloue par malloc dans ton code : "
					CYAN "%s" NC, g_traces_taille[i], lieu);
		}
		else
			pos += (size_t)snprintf(dst + pos, taille - pos,
					"\nbloc de %zu octet(s) cree par le test (ta fonction "
					"devait le liberer)", g_traces_taille[i]);
		n++;
	}
}

void	t_fuites_verif(int ligne, const char *quoi)
{
	size_t	v;
	char	fuites[1024];

	if (!g_malloc_dispo)
		return ;
	v = suivi_vivantes();
	fuites[0] = '\0';
	if (v)
		decrire_fuites(fuites, sizeof(fuites));
	verif(ligne, v == 0,
		"toute memoire allouee doit etre soit renvoyee, soit liberee "
		"(sinon c'est une fuite memoire)",
		"%s : %zu bloc(s) alloue(s) par malloc jamais libere(s)%s",
		quoi ? quoi : "apres l'appel", v, fuites);
}

void	t_taille_verif(int ligne, const void *p, size_t attendue,
		const char *quoi)
{
	long long	t;

	if (!g_malloc_dispo || !p)
		return ;
	t = suivi_taille(p);
	if (t < 0)
	{
		verif(ligne, 0, "le resultat doit etre alloue avec malloc par "
			"la libft (pas de tableau local, pas de chaine litterale)",
			"%s : ce pointeur n'a pas ete renvoye par malloc pendant l'appel",
			quoi ? quoi : "resultat");
		return ;
	}
	verif(ligne, (size_t)t == attendue,
		"il faut allouer EXACTEMENT la taille necessaire "
		"(pour une chaine : longueur + 1 pour le '\\0')",
		"%s : malloc(%zu) attendu\n%s : malloc(%lld) obtenu",
		quoi ? quoi : "resultat", attendue, quoi ? quoi : "resultat", t);
}

void	t_alloc_str_verif(int ligne, char *res, const char *attendu,
		const char *expl)
{
	if (!verif(ligne, res != NULL, expl,
			"attendu : %s\nobtenu  : NULL (la fonction a renvoye NULL)",
			esc(attendu)))
	{
		t_fuites_verif(ligne, "apres un retour NULL");
		return ;
	}
	v_str(ligne, res, attendu, expl);
	t_taille_verif(ligne, res, strlen(attendu) + 1, "chaine renvoyee");
	free(res);
	t_fuites_verif(ligne, "apres free(resultat)");
}

void	t_protection_verif(int ligne, size_t i, size_t n, void *res)
{
	size_t	v;
	char	fuites[1024];

	v = suivi_vivantes();
	fuites[0] = '\0';
	if (!verif(ligne, res == NULL,
			"si un malloc echoue, la fonction doit renvoyer NULL "
			"(il faut proteger chaque malloc)",
			"malloc numero %zu sur %zu a renvoye NULL\n"
			"la fonction a quand meme renvoye un pointeur non NULL", i, n))
		return ;
	if (v)
		decrire_fuites(fuites, sizeof(fuites));
	verif(ligne, v == 0,
		"quand un malloc echoue, il faut liberer tout ce qui a deja ete "
		"alloue avant de renvoyer NULL",
		"malloc numero %zu sur %zu a renvoye NULL\n"
		"%zu bloc(s) alloue(s) avant l'echec n'ont pas ete libere(s) (fuite)%s",
		i, n, v, fuites);
}

/* ========================================================================== */
/*                          MEMOIRE ELECTRIQUE                                */
/* ========================================================================== */

static size_t	taille_page(void)
{
	long	p;

	p = sysconf(_SC_PAGESIZE);
	if (p <= 0)
		return (4096);
	return ((size_t)p);
}

void	*alloc_electrique(size_t taille)
{
	size_t	pg;
	size_t	nb;
	char	*p;

	pg = taille_page();
	nb = (taille + pg - 1) / pg;
	if (nb == 0)
		nb = 1;
	p = mmap(NULL, (nb + 1) * pg, PROT_READ | PROT_WRITE,
			MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (p == MAP_FAILED)
		return (NULL);
	memset(p, 'Z', nb * pg);
	mprotect(p + nb * pg, pg, PROT_NONE);
	return (p + nb * pg - taille);
}

void	*alloc_electrique_avant(size_t taille)
{
	size_t	pg;
	size_t	nb;
	char	*p;

	pg = taille_page();
	nb = (taille + pg - 1) / pg;
	if (nb == 0)
		nb = 1;
	p = mmap(NULL, (nb + 1) * pg, PROT_READ | PROT_WRITE,
			MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (p == MAP_FAILED)
		return (NULL);
	memset(p + pg, 'Z', nb * pg);
	mprotect(p, pg, PROT_NONE);
	return (p + pg);
}

void	lecture_seule(void *p, size_t taille)
{
	size_t		pg;
	uintptr_t	debut;
	uintptr_t	fin;

	pg = taille_page();
	debut = (uintptr_t)p & ~(uintptr_t)(pg - 1);
	fin = ((uintptr_t)p + taille + pg - 1) & ~(uintptr_t)(pg - 1);
	if (fin > debut)
		mprotect((void *)debut, fin - debut, PROT_READ);
}

/* ========================================================================== */
/*                          ISOLATION (fork)                                  */
/* ========================================================================== */

int	delai_test(void)
{
	const char	*s;
	int			d;

	s = getenv("TEST_TIMEOUT");
	d = s ? atoi(s) : 0;
	if (d <= 0)
		d = 10;
	return (d);
}

/*
** Dans le fils : au crash (ou au TIMEOUT), on photographie la pile d'appels
** dans la memoire partagee, puis on laisse le signal tuer le processus.
** La pile alternative permet de gerer aussi une recursion infinie.
*/
static char	g_pile_alt[65536];

static void	capturer_pile(int sig)
{
#if LIGNES_DISPO
	g_bac->nb_pile = backtrace(g_bac->pile, 64);
#endif
	signal(sig, SIG_DFL);
	raise(sig);
}

static void	installer_capture(void)
{
	stack_t				ss;
	struct sigaction	sa;
	const int			sigs[] = {SIGSEGV, SIGBUS, SIGABRT, SIGFPE, SIGILL,
		SIGALRM};
	size_t				i;

	g_bac->nb_pile = 0;
	ss.ss_sp = g_pile_alt;
	ss.ss_size = sizeof(g_pile_alt);
	ss.ss_flags = 0;
	sigaltstack(&ss, NULL);
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = capturer_pile;
	sa.sa_flags = SA_ONSTACK | SA_NODEFER | SA_RESETHAND;
	sigemptyset(&sa.sa_mask);
	for (i = 0; i < sizeof(sigs) / sizeof(sigs[0]); i++)
		sigaction(sigs[i], &sa, NULL);
}

int	executer_isole(void (*f)(void), int delai)
{
	pid_t	pid;
	int		statut;

	fflush(stdout);
	fflush(stderr);
	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid == 0)
	{
		installer_capture();
		alarm((unsigned int)delai);
		f();
		fflush(stdout);
		_exit(0);
	}
	while (waitpid(pid, &statut, 0) < 0)
		if (errno != EINTR)
			return (-1);
	if (WIFSIGNALED(statut))
		return (WTERMSIG(statut));
	if (WIFEXITED(statut) && WEXITSTATUS(statut) != 0)
		return (-1);
	return (0);
}

const char	*nom_signal(int sig)
{
	char	*dst;

	if (sig == SIGSEGV)
		return ("SIGSEGV (Segmentation fault)");
	if (sig == SIGBUS)
		return ("SIGBUS (Bus error)");
	if (sig == SIGABRT)
		return ("SIGABRT (Abort)");
	if (sig == SIGFPE)
		return ("SIGFPE (Floating point exception)");
	if (sig == SIGILL)
		return ("SIGILL (Illegal instruction)");
	if (sig == SIGALRM)
		return ("TIMEOUT (temps depasse)");
	if (sig == SIGKILL)
		return ("SIGKILL (processus tue)");
	if (sig == SIGPIPE)
		return ("SIGPIPE (Broken pipe)");
	if (sig < 0)
		return ("arret anormal (exit avec un code d'erreur)");
	dst = buffer_tournant();
	snprintf(dst, TAILLE_BUF, "signal %d (%s)", sig, strsignal(sig));
	return (dst);
}

const char	*explication_signal(int sig)
{
	if (sig == SIGSEGV)
		return ("acces a une adresse memoire interdite : pointeur NULL "
			"dereference, lecture/ecriture apres la fin d'un buffer "
			"ou d'une chaine (oubli du '\\0'), ou pointeur deja libere");
	if (sig == SIGBUS)
		return ("acces memoire invalide : souvent une ecriture dans une "
			"chaine litterale (\"...\") ou un pointeur non initialise");
	if (sig == SIGABRT)
		return ("malloc a detecte une corruption : double free, free d'un "
			"pointeur qui ne vient pas de malloc, ou ecriture hors du bloc "
			"alloue (malloc trop petit)");
	if (sig == SIGFPE)
		return ("division (ou modulo) par zero");
	if (sig == SIGALRM)
		return ("la fonction ne s'est pas terminee a temps : boucle infinie "
			"probable (condition d'arret jamais atteinte ?)");
	if (sig == SIGKILL)
		return ("le systeme a tue le processus : memoire epuisee "
			"(allocation enorme ou boucle d'allocations) ?");
	if (sig < 0)
		return ("le processus a appele exit() : une fonction de la libft "
			"ne doit jamais quitter le programme");
	return ("le programme s'est arrete brutalement");
}

void	journal_crash(int sig)
{
	g_bac->erreurs++;
	journal("    " ROUGE "x CRASH" NC);
	if (g_bac->source[0])
		journal(" " MAGENTA "[%s]" NC, g_bac->source);
	journal(" " ROUGE "%s" NC "\n", nom_signal(sig));
	if (g_bac->appel[0])
		journal("        pendant : " GRAS "%s" NC "\n", g_bac->appel);
	if (g_bac->ligne)
		journal("        derniere etape atteinte : " CYAN "%s:%d" NC "\n",
			g_bac->fichier, g_bac->ligne);
	else
		journal("        avant la toute premiere verification du groupe "
			"(le premier appel a la fonction a deja echoue)\n");
	if (g_bac->nb_pile > 0)
		journal_lignes(sig == SIGALRM ? "ta fonction tournait ici"
			: "crash dans ton code", g_bac->pile, g_bac->nb_pile);
	g_bac->nb_pile = 0;
	journal("      " JAUNE "-> %s" NC "\n", explication_signal(sig));
	if (sig == SIGALRM)
		journal("      " JAUNE "-> (delai : %d s, modifiable avec "
			"TEST_TIMEOUT=30)" NC "\n", delai_test());
}

static void	(*g_sous_f)(void);

static void	lancer_sous_test(void)
{
	g_sous_f();
}

int	sous_test(int ligne, void (*f)(void), const char *quoi, const char *expl)
{
	int		sig;
	char	appel[512];
	char	fichier[64];
	char	lignes[1024];
	char	*nl;

	snprintf(appel, sizeof(appel), "%s", g_bac->appel);
	snprintf(fichier, sizeof(fichier), "%s",
		g_test_fichier ? g_test_fichier : "");
	g_sous_f = f;
	g_bac->ligne = ligne;
	snprintf(g_bac->appel, sizeof(g_bac->appel), "%s", quoi ? quoi : "");
	sig = executer_isole(lancer_sous_test, delai_test());
	g_test_fichier = fichier;
	if (sig == 0)
		sig = !verif(ligne, 1, expl, NULL);
	else
	{
		lignes[0] = '\0';
		if (g_bac->nb_pile > 0 && lignes_de_ton_code(g_bac->pile,
				g_bac->nb_pile, lignes + 1, sizeof(lignes) - 1))
		{
			lignes[0] = '\n';
			nl = lignes;
			while ((nl = strchr(nl + 1, '\n')) != NULL)
				*nl = ' ';
		}
		g_bac->nb_pile = 0;
		verif(ligne, 0, expl, "crash : %s\n(%s)%s%s", nom_signal(sig),
			explication_signal(sig), lignes[0] ? "\ndans ton code : " : "",
			lignes[0] ? lignes + 1 : "");
	}
	snprintf(g_bac->appel, sizeof(g_bac->appel), "%s", appel);
	g_test_fichier = "";
	return (sig == 0);
}

/* ========================================================================== */
/*                       DESCRIPTEURS DE FICHIER                              */
/* ========================================================================== */

int	fd_temporaire(void)
{
	char	modele[64];
	int		fd;

	snprintf(modele, sizeof(modele), "/tmp/libft_test_fd_XXXXXX");
	fd = mkstemp(modele);
	if (fd >= 0)
		unlink(modele);
	return (fd);
}

int	v_fd(int ligne, int fd, const char *attendu, size_t n, const char *expl)
{
	char	lu[8192];
	ssize_t	r;
	size_t	total;
	int		ok;
	size_t	i;

	if (fd < 0)
		return (verif(ligne, 0, "impossible de creer un fichier temporaire",
				"mkstemp a echoue"));
	total = 0;
	lseek(fd, 0, SEEK_SET);
	while (total < sizeof(lu)
		&& (r = read(fd, lu + total, sizeof(lu) - total)) > 0)
		total += (size_t)r;
	close(fd);
	ok = (total == n && memcmp(lu, attendu, n) == 0);
	if (ok)
		return (verif(ligne, 1, expl, NULL));
	i = 0;
	while (i < n && i < total && lu[i] == attendu[i])
		i++;
	return (verif(ligne, 0, expl,
			"attendu dans le fichier : %s (%zu octets)\n"
			"ecrit par ta fonction   : %s (%zu octets)\n"
			"premiere difference a l'octet %zu",
			esc_n(attendu, n), n, esc_n(lu, total), total, i));
}

/* ========================================================================== */
/*                                LISTES                                      */
/* ========================================================================== */

t_list	*lst_noeud(void *content)
{
	t_list	*n;

	n = malloc(sizeof(t_list));
	if (!n)
		return (NULL);
	n->content = content;
	n->next = NULL;
	return (n);
}

t_list	*lst_creer(int n, ...)
{
	va_list	args;
	t_list	*tete;
	t_list	**fin;
	int		i;

	tete = NULL;
	fin = &tete;
	va_start(args, n);
	i = 0;
	while (i++ < n)
	{
		*fin = lst_noeud(va_arg(args, void *));
		if (!*fin)
			break ;
		fin = &(*fin)->next;
	}
	va_end(args);
	return (tete);
}

const char	*lst_texte(const t_list *l)
{
	static char	bufs[4][1024];
	static int	k;
	char		*dst;
	size_t		pos;
	int			nb;

	k = (k + 1) % 4;
	dst = bufs[k];
	pos = 0;
	nb = 0;
	pos += snprintf(dst + pos, sizeof(bufs[k]) - pos, "[");
	while (l && nb < 12 && pos < sizeof(bufs[k]) - 100)
	{
		pos += snprintf(dst + pos, sizeof(bufs[k]) - pos, "%s%s",
				nb ? ", " : "", l->content ? esc(l->content) : "NULL");
		l = l->next;
		nb++;
	}
	if (l)
		pos += snprintf(dst + pos, sizeof(bufs[k]) - pos, ", ...");
	snprintf(dst + pos, sizeof(bufs[k]) - pos, "]");
	return (dst);
}

void	lst_liberer_noeuds(t_list *l)
{
	t_list	*suivant;

	while (l)
	{
		suivant = l->next;
		free(l);
		l = suivant;
	}
}

void	lst_liberer_tout(t_list *l)
{
	t_list	*suivant;

	while (l)
	{
		suivant = l->next;
		free(l->content);
		free(l);
		l = suivant;
	}
}
