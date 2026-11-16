/*
** fr_bonus.c - tests francinette des fonctions BONUS (listes chainees)
**
** Portage fidele de :
**   fsoares     (francinette/tests/libft/fsoares/test_lst*.c)
**   Tripouille  (libftTester/tests/ft_lst*_test.cpp)
**   alelievr    (libft-unit-test/src/test_functions.c, test_ft_lst*)
**   war-machine (libft-war-machine/tests/Bonus_functions)
**
** Les tests alelievr *_null* (crash attendu sur NULL) ne sont pas portes :
** le sujet ne demande rien pour des parametres NULL.
*/
#include "test.h"

/* ========================================================================== */
/*                               EXPLICATIONS                                 */
/* ========================================================================== */

#define EX_NEW "ft_lstnew doit renvoyer un noeud alloue (malloc(sizeof(t_list)))"
#define EX_NEW_CONTENT "ft_lstnew doit stocker le pointeur content tel quel \
(sans le copier)"
#define EX_NEW_NEXT "ft_lstnew doit mettre next a NULL"
#define EX_FRONT "ft_lstadd_front doit placer new en tete : new->next = *lst \
puis *lst = new"
#define EX_BACK "ft_lstadd_back doit accrocher new apres le dernier noeud \
(ou faire *lst = new si la liste est vide)"
#define EX_SIZE "ft_lstsize doit compter les noeuds jusqu'a next == NULL \
(0 pour une liste vide)"
#define EX_LAST "ft_lstlast doit renvoyer le dernier noeud (celui dont next == \
NULL), ou NULL si la liste est vide"
#define EX_DELONE "ft_lstdelone doit appeler del sur le contenu puis free sur \
le noeud, sans toucher au noeud suivant"
#define EX_CLEAR "ft_lstclear doit appeler del sur chaque contenu, free chaque \
noeud, puis mettre *lst a NULL"
#define EX_ITER "ft_lstiter doit appeler f sur le contenu de chaque noeud, \
dans l'ordre"
#define EX_MAP "ft_lstmap doit creer une NOUVELLE liste dont chaque contenu \
vaut f(contenu d'origine)"
#define EX_MAP_ORIG "ft_lstmap ne doit pas modifier la liste d'origine"
#define EX_NO_MALLOC "cette fonction n'a aucune raison d'appeler malloc"

/* ========================================================================== */
/*                                  OUTILS                                    */
/* ========================================================================== */

static int	g_del_appels;

/* del qui compte et libere le contenu */
static void	del_free(void *p)
{
	g_del_appels++;
	free(p);
}

/* del qui libere *(void **)p (contenu = adresse d'un pointeur) */
static void	del_free_ptr(void *p)
{
	g_del_appels++;
	free(*(void **)p);
}

/* del qui compte seulement */
static void	del_compte(void *p)
{
	(void)p;
	g_del_appels++;
}

/* strdup qui passe par malloc (donc suivi pendant suivi_debut) */
static char	*dup_suivi(const char *s)
{
	char	*r;
	size_t	n;

	n = strlen(s) + 1;
	r = malloc(n);
	if (r)
		memcpy(r, s, n);
	return (r);
}

static void	ajouter(char *buf, size_t taille, size_t *pos, const char *txt)
{
	size_t	n;

	if (taille == 0 || *pos >= taille - 1)
		return ;
	n = strlen(txt);
	if (n > taille - 1 - *pos)
		n = taille - 1 - *pos;
	memcpy(buf + *pos, txt, n);
	*pos += n;
	buf[*pos] = '\0';
}

/* Texte d'une liste dont les contenus sont des chaines */
static const char	*txt_liste(char *buf, size_t taille, const t_list *l)
{
	size_t	pos;
	int		i;

	pos = 0;
	buf[0] = '\0';
	ajouter(buf, taille, &pos, "[");
	i = 0;
	while (l && i < 32)
	{
		if (i)
			ajouter(buf, taille, &pos, ", ");
		if (l->content)
			ajouter(buf, taille, &pos, esc((const char *)l->content));
		else
			ajouter(buf, taille, &pos, "NULL");
		l = l->next;
		i++;
	}
	if (l)
		ajouter(buf, taille, &pos, ", ... (trop long ou cyclique)");
	ajouter(buf, taille, &pos, "]");
	return (buf);
}

/* Texte d'un tableau de chaines */
static const char	*txt_tab(char *buf, size_t taille, int n,
	const char **tab)
{
	size_t	pos;
	int		i;

	pos = 0;
	buf[0] = '\0';
	ajouter(buf, taille, &pos, "[");
	i = 0;
	while (i < n)
	{
		if (i)
			ajouter(buf, taille, &pos, ", ");
		ajouter(buf, taille, &pos, esc(tab[i]));
		i++;
	}
	ajouter(buf, taille, &pos, "]");
	return (buf);
}

/* 1 si la liste contient exactement les n chaines de att */
static int	liste_egale(const t_list *l, int n, const char **att)
{
	int	i;

	i = 0;
	while (i < n && l && i < 64)
	{
		if (!l->content || strcmp((const char *)l->content, att[i]))
			return (0);
		l = l->next;
		i++;
	}
	return (i == n && l == NULL);
}

static int	v_liste(int ligne, const t_list *l, int n, const char **att,
	const char *expl)
{
	char	a[1500];
	char	o[1500];
	int		ok;

	ok = liste_egale(l, n, att);
	txt_tab(a, sizeof(a), n, att);
	txt_liste(o, sizeof(o), l);
	return (verif(ligne, ok, expl, "attendu : %s\nobtenu  : %s", a, o));
}

#define V_LISTE(l, n, att, e) v_liste(__LINE__, (l), (n), (att), (e))

/* Compare une liste dont les contenus sont des entiers castes en pointeur */
static int	v_liste_num(int ligne, const t_list *l, int n, const long *vals,
	const char *expl)
{
	char	a[512];
	char	o[512];
	char	tmp[48];
	size_t	pa;
	size_t	po;
	int		i;
	int		ok;

	pa = 0;
	po = 0;
	a[0] = '\0';
	o[0] = '\0';
	ok = 1;
	i = 0;
	while (i < n)
	{
		snprintf(tmp, sizeof(tmp), "%s%ld", i ? ", " : "", vals[i]);
		ajouter(a, sizeof(a), &pa, tmp);
		i++;
	}
	i = 0;
	while (l && i < 32)
	{
		if (i >= n || (long)(intptr_t)l->content != vals[i])
			ok = 0;
		snprintf(tmp, sizeof(tmp), "%s%ld", i ? ", " : "",
			(long)(intptr_t)l->content);
		ajouter(o, sizeof(o), &po, tmp);
		l = l->next;
		i++;
	}
	if (l)
	{
		ok = 0;
		ajouter(o, sizeof(o), &po, ", ...");
	}
	if (i != n)
		ok = 0;
	return (verif(ligne, ok, expl, "attendu : [%s]\nobtenu  : [%s]", a, o));
}

/* Avec le suivi malloc : la fonction ne doit avoir fait aucun malloc */
static void	sans_malloc(int ligne, const char *fonction)
{
	if (!g_malloc_dispo)
		return ;
	verif(ligne, suivi_nb_malloc() == 0, EX_NO_MALLOC,
		"%s a appele malloc %zu fois", fonction, suivi_nb_malloc());
}

/* add_back "maison" de Tripouille : utilise ft_lstlast */
static void	tr_add_back(t_list **list, t_list *node)
{
	t_list	*last;

	last = ft_lstlast(*list);
	if (last == NULL)
		*list = node;
	else
		last->next = node;
}

/* Les 4 noeuds de war-machine : lorem -> ipsum -> dolor -> sit */
static void	wm_quatre(t_list **e, int lier)
{
	e[0] = lst_noeud(dup_suivi("lorem"));
	e[1] = lst_noeud(dup_suivi("ipsum"));
	e[2] = lst_noeud(dup_suivi("dolor"));
	e[3] = lst_noeud(dup_suivi("sit"));
	if (lier)
	{
		e[0]->next = e[1];
		e[1]->next = e[2];
		e[2]->next = e[3];
	}
}

/* ========================================================================== */
/*                                ft_lstnew                                   */
/* ========================================================================== */

static void	verif_lstnew(int ligne, void *content, const char *desc)
{
	t_list	*res;

	essai(ligne, "ft_lstnew(%s)", desc);
	suivi_debut(0);
	res = ft_lstnew(content);
	suivi_fin();
	if (!verif(ligne, res != NULL, EX_NEW, "obtenu : NULL (aucun noeud)"))
		return ;
	v_ptr(ligne, res->content, content, NULL, EX_NEW_CONTENT);
	v_ptr(ligne, res->next, NULL, NULL, EX_NEW_NEXT);
	if (g_malloc_dispo)
		t_taille_verif(ligne, res, sizeof(t_list), "le noeud de ft_lstnew");
	free(res);
	if (g_malloc_dispo)
		t_fuites_verif(ligne, "ft_lstnew");
}

static void	fs_lstnew(void)
{
	char	texte[] = "hahaha";

	verif_lstnew(__LINE__, NULL, "NULL");
	verif_lstnew(__LINE__, texte, "\"hahaha\"");
	T_PROTECTION(ft_lstnew(NULL), free);
	T_PROTECTION(ft_lstnew(texte), free);
}

static void	tr_lstnew(void)
{
	t_list	*l;

	ESSAI("ft_lstnew((void *)42)");
	suivi_debut(0);
	l = ft_lstnew((void *)42);
	suivi_fin();
	if (!V_OK(l != NULL, EX_NEW))
		return ;
	V_PTR(l->content, (void *)42, NULL, EX_NEW_CONTENT);
	V_PTR(l->next, NULL, NULL, EX_NEW_NEXT);
	T_TAILLE(l, sizeof(t_list), "le noeud de ft_lstnew");
	free(l);
	T_FUITES("ft_lstnew((void *)42)");
}

static void	al_lstnew(void)
{
	char	*data;
	t_list	*l;

	data = "hello, i'm a data";
	ESSAI("ft_lstnew(\"hello, i'm a data\")");
	suivi_debut(0);
	l = ft_lstnew(data);
	suivi_fin();
	if (!V_OK(l != NULL, EX_NEW))
		return ;
	V_STR((const char *)l->content, data, "your lstnew does not work with "
		"basic input : le contenu doit etre la donnee passee");
	V_PTR(l->content, data, NULL, EX_NEW_CONTENT);
	free(l);
	T_FUITES("ft_lstnew : le noeud doit etre alloue avec malloc");
}

static void	wm_lstnew(void)
{
	char	str[] = "lorem ipsum dolor sit";
	int		i;
	t_list	*l;

	i = 42;
	ESSAI("ft_lstnew(\"lorem ipsum dolor sit\")");
	l = ft_lstnew(str);
	if (V_OK(l != NULL, EX_NEW))
	{
		V_STR((const char *)l->content, "lorem ipsum dolor sit",
			EX_NEW_CONTENT);
		V_PTR(l->next, NULL, NULL, EX_NEW_NEXT);
		free(l);
	}
	ESSAI("ft_lstnew(&i) avec i = 42");
	l = ft_lstnew(&i);
	if (V_OK(l != NULL, EX_NEW))
	{
		V_PTR(l->content, &i, NULL, EX_NEW_CONTENT);
		if (l->content == &i)
			V_INT(*(int *)l->content, 42, EX_NEW_CONTENT);
		V_PTR(l->next, NULL, NULL, EX_NEW_NEXT);
		free(l);
	}
	ESSAI("ft_lstnew(NULL)");
	l = ft_lstnew(NULL);
	if (V_OK(l != NULL, EX_NEW))
	{
		V_PTR(l->content, NULL, NULL, EX_NEW_CONTENT);
		V_PTR(l->next, NULL, NULL, EX_NEW_NEXT);
		free(l);
	}
}

/* ========================================================================== */
/*                             ft_lstadd_front                                */
/* ========================================================================== */

static void	fs_lstadd_front(void)
{
	t_list		*l;
	t_list		*n;
	t_list		*perdu;
	int			ok3;
	int			ok4;
	char		b[3][1024];
	const char	*a1[] = {"new head"};
	const char	*a2[] = {"new head", "old head"};
	const char	*a3[] = {"new head", "old head", "tail"};
	const char	*a4[] = {"new head", "lost node", "old head", "tail"};

	l = NULL;
	n = lst_noeud("new head");
	ESSAI("ft_lstadd_front(&[] (liste vide), {\"new head\"})");
	suivi_debut(0);
	ft_lstadd_front(&l, n);
	suivi_fin();
	V_LISTE(l, 1, a1, EX_FRONT);
	sans_malloc(__LINE__, "ft_lstadd_front");
	if (liste_egale(l, 1, a1))
		lst_liberer_noeuds(l);
	l = lst_creer(1, "old head");
	n = lst_noeud("new head");
	ESSAI("ft_lstadd_front(&[\"old head\"], {\"new head\"})");
	suivi_debut(0);
	ft_lstadd_front(&l, n);
	suivi_fin();
	V_LISTE(l, 2, a2, EX_FRONT);
	sans_malloc(__LINE__, "ft_lstadd_front");
	if (liste_egale(l, 2, a2))
		lst_liberer_noeuds(l);
	l = lst_creer(2, "old head", "tail");
	n = lst_creer(2, "new head", "lost node");
	perdu = n->next;
	ESSAI("ft_lstadd_front(&[\"old head\", \"tail\"], "
		"[\"new head\", \"lost node\"])");
	ft_lstadd_front(&l, n);
	ok3 = liste_egale(l, 3, a3);
	ok4 = liste_egale(l, 4, a4);
	verif(__LINE__, ok3 || ok4, EX_FRONT,
		"attendu : %s\n     ou : %s\nobtenu  : %s",
		txt_tab(b[0], sizeof(b[0]), 3, a3), txt_tab(b[1], sizeof(b[1]), 4, a4),
		txt_liste(b[2], sizeof(b[2]), l));
	if (ok3)
		free(perdu);
	if (ok3 || ok4)
		lst_liberer_noeuds(l);
}

static void	tr_lstadd_front(void)
{
	t_list		*l;
	const long	v1[] = {1};
	const long	v2[] = {2, 1};

	l = NULL;
	ESSAI("ft_lstadd_front(&l, ft_lstnew((void *)1)) avec l = NULL");
	ft_lstadd_front(&l, ft_lstnew((void *)1));
	if (!V_OK(l != NULL, EX_FRONT))
		return ;
	V_PTR(l->content, (void *)1, NULL, EX_FRONT);
	V_PTR(l->next, NULL, NULL, EX_FRONT);
	v_liste_num(__LINE__, l, 1, v1, EX_FRONT);
	ESSAI("ft_lstadd_front(&l, ft_lstnew((void *)2)) avec l = [1]");
	ft_lstadd_front(&l, ft_lstnew((void *)2));
	if (!V_OK(l != NULL, EX_FRONT))
		return ;
	V_PTR(l->content, (void *)2, NULL, EX_FRONT);
	if (!V_OK(l->next != NULL, EX_FRONT))
		return ;
	V_PTR(l->next->content, (void *)1, NULL, EX_FRONT);
	V_PTR(l->next->next, NULL, NULL, EX_FRONT);
	v_liste_num(__LINE__, l, 2, v2, EX_FRONT);
	lst_liberer_noeuds(l);
}

static void	al_lstadd_front(void)
{
	t_list	*l;
	t_list	*ancien;
	t_list	*n;

	l = lst_noeud(dup_suivi("nyacat"));
	ancien = l;
	n = lst_noeud(dup_suivi("OK"));
	ESSAI("ft_lstadd_front(&[\"nyacat\"], {\"OK\"})");
	ft_lstadd_front(&l, n);
	V_PTR(l, n, NULL, "your lstadd_front does not work with basic input : "
		"*lst doit devenir new");
	if (l != n)
		return ;
	V_STR((const char *)l->content, "OK", EX_FRONT);
	V_PTR(l->next, ancien, NULL, "new->next doit pointer sur l'ancienne "
		"tete de liste");
	lst_liberer_tout(l);
}

static void	wm_lstadd_front(void)
{
	t_list		*e[4];
	t_list		*begin;
	const char	*att[] = {"lorem", "ipsum", "dolor", "sit"};

	wm_quatre(e, 0);
	begin = NULL;
	ESSAI("ft_lstadd_front(&begin, sit) puis dolor, ipsum, lorem "
		"(begin = NULL au depart)");
	ft_lstadd_front(&begin, e[3]);
	ft_lstadd_front(&begin, e[2]);
	ft_lstadd_front(&begin, e[1]);
	ft_lstadd_front(&begin, e[0]);
	if (V_LISTE(begin, 4, att, EX_FRONT))
		lst_liberer_tout(begin);
}

/* ========================================================================== */
/*                               ft_lstsize                                   */
/* ========================================================================== */

static void	verif_lstsize(int ligne, t_list *l, const char *desc, int attendu)
{
	unsigned int	r;

	essai(ligne, "ft_lstsize(%s)", desc);
	suivi_debut(0);
	r = ft_lstsize(l);
	suivi_fin();
	v_int(ligne, (long long)r, attendu, EX_SIZE);
	sans_malloc(ligne, "ft_lstsize");
}

static void	fs_lstsize(void)
{
	t_list	*l;
	char	b[512];

	verif_lstsize(__LINE__, NULL, "[] (NULL)", 0);
	l = lst_creer(1, "one");
	verif_lstsize(__LINE__, l, txt_liste(b, sizeof(b), l), 1);
	lst_liberer_noeuds(l);
	l = lst_creer(5, "one", "two", "three", "four", "five");
	verif_lstsize(__LINE__, l, txt_liste(b, sizeof(b), l), 5);
	lst_liberer_noeuds(l);
}

static void	tr_lstsize(void)
{
	t_list	*l;

	l = NULL;
	verif_lstsize(__LINE__, l, "NULL", 0);
	ESSAI("ft_lstadd_front(&l, ft_lstnew((void *)1))");
	ft_lstadd_front(&l, ft_lstnew((void *)1));
	verif_lstsize(__LINE__, l, "[1]", 1);
	ESSAI("ft_lstadd_front(&l, ft_lstnew((void *)2))");
	ft_lstadd_front(&l, ft_lstnew((void *)2));
	verif_lstsize(__LINE__, l, "[2, 1]", 2);
}

static void	al_lstsize(void)
{
	t_list	*l;

	l = lst_noeud(dup_suivi("1"));
	l->next = lst_noeud(dup_suivi("2"));
	l->next->next = lst_noeud(dup_suivi("3"));
	verif_lstsize(__LINE__, l, "[\"1\", \"2\", \"3\"]", 3);
	lst_liberer_tout(l);
}

static void	wm_lstsize(void)
{
	t_list	*e[4];

	wm_quatre(e, 1);
	verif_lstsize(__LINE__, e[0], "[lorem, ipsum, dolor, sit]", 4);
	e[0]->next = NULL;
	verif_lstsize(__LINE__, e[0], "[lorem] (elem->next = NULL)", 1);
	verif_lstsize(__LINE__, NULL, "NULL", 0);
	lst_liberer_tout(e[0]);
	lst_liberer_tout(e[1]);
}

/* ========================================================================== */
/*                               ft_lstlast                                   */
/* ========================================================================== */

static t_list	*verif_lstlast(int ligne, t_list *l, const char *desc,
	t_list *attendu)
{
	t_list	*r;

	essai(ligne, "ft_lstlast(%s)", desc);
	suivi_debut(0);
	r = ft_lstlast(l);
	suivi_fin();
	v_ptr(ligne, r, attendu, NULL, EX_LAST);
	sans_malloc(ligne, "ft_lstlast");
	return (r);
}

static void	fs_lstlast(void)
{
	t_list	*l;
	char	b[512];

	verif_lstlast(__LINE__, NULL, "[] (NULL)", NULL);
	l = lst_creer(1, "one");
	verif_lstlast(__LINE__, l, txt_liste(b, sizeof(b), l), l);
	lst_liberer_noeuds(l);
	l = lst_creer(5, "one", "two", "three", "four", "five");
	verif_lstlast(__LINE__, l, txt_liste(b, sizeof(b), l),
		l->next->next->next->next);
	lst_liberer_noeuds(l);
}

static void	tr_lstlast(void)
{
	t_list	*l;
	t_list	*r;

	l = NULL;
	ESSAI("ft_lstlast(NULL)");
	V_PTR(ft_lstlast(l), NULL, NULL, EX_LAST);
	ESSAI("own_add_back(&l, ft_lstnew((void *)1)) puis ft_lstlast(l)");
	tr_add_back(&l, ft_lstnew((void *)1));
	r = ft_lstlast(l);
	if (V_OK(r != NULL, EX_LAST))
		V_PTR(r->content, (void *)1, NULL, EX_LAST);
	ESSAI("own_add_back(&l, ft_lstnew((void *)2)) puis ft_lstlast(l)");
	tr_add_back(&l, ft_lstnew((void *)2));
	r = ft_lstlast(l);
	if (V_OK(r != NULL, EX_LAST))
	{
		V_PTR(r->content, (void *)2, NULL, EX_LAST);
		V_PTR(r->next, NULL, NULL, EX_LAST);
	}
}

static void	al_lstlast(void)
{
	t_list	*l;

	l = lst_noeud(dup_suivi("1"));
	l->next = lst_noeud(dup_suivi("2"));
	l->next->next = lst_noeud(dup_suivi("3"));
	verif_lstlast(__LINE__, l, "[\"1\", \"2\", \"3\"]", l->next->next);
	lst_liberer_tout(l);
}

static void	wm_lstlast(void)
{
	t_list	*e[4];
	t_list	*r;

	wm_quatre(e, 1);
	r = verif_lstlast(__LINE__, e[0], "[lorem, ipsum, dolor, sit]", e[3]);
	if (r == e[3])
		V_STR((const char *)r->content, "sit", EX_LAST);
	e[0]->next = NULL;
	r = verif_lstlast(__LINE__, e[0], "[lorem] (elem->next = NULL)", e[0]);
	if (r == e[0])
		V_STR((const char *)r->content, "lorem", EX_LAST);
	verif_lstlast(__LINE__, NULL, "NULL", NULL);
	lst_liberer_tout(e[0]);
	lst_liberer_tout(e[1]);
}

/* ========================================================================== */
/*                              ft_lstadd_back                                */
/* ========================================================================== */

static void	fs_lstadd_back(void)
{
	t_list		*l;
	t_list		*n;
	const char	*a1[] = {"last"};
	const char	*a2[] = {"first", "last"};
	const char	*a6[] = {"one", "two", "three", "four", "five", "last"};

	l = NULL;
	n = lst_noeud("last");
	ESSAI("ft_lstadd_back(&[] (liste vide), {\"last\"})");
	suivi_debut(0);
	ft_lstadd_back(&l, n);
	suivi_fin();
	V_LISTE(l, 1, a1, EX_BACK);
	sans_malloc(__LINE__, "ft_lstadd_back");
	if (liste_egale(l, 1, a1))
		lst_liberer_noeuds(l);
	l = lst_creer(1, "first");
	n = lst_noeud("last");
	ESSAI("ft_lstadd_back(&[\"first\"], {\"last\"})");
	suivi_debut(0);
	ft_lstadd_back(&l, n);
	suivi_fin();
	V_LISTE(l, 2, a2, EX_BACK);
	sans_malloc(__LINE__, "ft_lstadd_back");
	if (liste_egale(l, 2, a2))
		lst_liberer_noeuds(l);
	l = lst_creer(5, "one", "two", "three", "four", "five");
	n = lst_noeud("last");
	ESSAI("ft_lstadd_back(&[\"one\", \"two\", \"three\", \"four\", \"five\"], "
		"{\"last\"})");
	suivi_debut(0);
	ft_lstadd_back(&l, n);
	suivi_fin();
	V_LISTE(l, 6, a6, EX_BACK);
	sans_malloc(__LINE__, "ft_lstadd_back");
	if (liste_egale(l, 6, a6))
		lst_liberer_noeuds(l);
}

static void	tr_lstadd_back(void)
{
	t_list		*l;
	t_list		*l2;
	const long	v2[] = {1, 2};
	const long	v4[] = {1, 2, 3, 4};

	l = NULL;
	l2 = NULL;
	ESSAI("ft_lstadd_back(&l, ft_lstnew((void *)1)) avec l = NULL");
	ft_lstadd_back(&l, ft_lstnew((void *)1));
	if (!V_OK(l != NULL, EX_BACK))
		return ;
	V_PTR(l->content, (void *)1, NULL, EX_BACK);
	V_PTR(l->next, NULL, NULL, EX_BACK);
	ESSAI("ft_lstadd_back(&l, ft_lstnew((void *)2)) avec l = [1]");
	ft_lstadd_back(&l, ft_lstnew((void *)2));
	if (!v_liste_num(__LINE__, l, 2, v2, EX_BACK))
		return ;
	ESSAI("ft_lstadd_back(&l2, ft_lstnew((void *)3)) puis "
		"ft_lstadd_back(&l2, ft_lstnew((void *)4))");
	ft_lstadd_back(&l2, ft_lstnew((void *)3));
	ft_lstadd_back(&l2, ft_lstnew((void *)4));
	ESSAI("ft_lstadd_back(&l, l2) avec l = [1, 2] et l2 = [3, 4]");
	ft_lstadd_back(&l, l2);
	if (v_liste_num(__LINE__, l, 4, v4, EX_BACK))
		lst_liberer_noeuds(l);
}

static void	al_lstadd_back(void)
{
	t_list	*l;
	t_list	*n;

	l = lst_noeud(dup_suivi("nyacat"));
	n = lst_noeud(dup_suivi("OK"));
	ESSAI("ft_lstadd_back(&[\"nyacat\"], {\"OK\"})");
	ft_lstadd_back(&l, n);
	if (!V_OK(l != NULL, EX_BACK))
		return ;
	V_PTR(l->next, n, NULL, "your lstadd_back does not work with basic input "
		": l->next doit etre le nouveau noeud");
	if (l->next != n)
		return ;
	V_STR((const char *)l->next->content, "OK", EX_BACK);
	V_PTR(n->next, NULL, NULL, EX_BACK);
	lst_liberer_tout(l);
}

static void	wm_lstadd_back(void)
{
	t_list		*e[4];
	t_list		*begin;
	const char	*att[] = {"lorem", "ipsum", "dolor", "sit"};

	wm_quatre(e, 0);
	begin = NULL;
	ESSAI("ft_lstadd_back(&begin, lorem) puis ipsum, dolor, sit "
		"(begin = NULL au depart)");
	ft_lstadd_back(&begin, e[0]);
	ft_lstadd_back(&begin, e[1]);
	ft_lstadd_back(&begin, e[2]);
	ft_lstadd_back(&begin, e[3]);
	if (V_LISTE(begin, 4, att, EX_BACK))
		lst_liberer_tout(begin);
}

/* ========================================================================== */
/*                              ft_lstdelone                                  */
/* ========================================================================== */

/* fsoares 1 : contenu alloue, del = free */
static void	fs_delone_1(void)
{
	char	*s;
	t_list	*l;

	suivi_debut(0);
	s = dup_suivi("teste");
	l = lst_noeud(s);
	g_del_appels = 0;
	ESSAI("ft_lstdelone({\"teste\"}, [(x) => free(x)])");
	ft_lstdelone(l, del_free);
	suivi_fin();
	V_INT(g_del_appels, 1, "ft_lstdelone doit appeler del une fois, "
		"sur le contenu");
	T_FUITES("ft_lstdelone : del doit etre utilise sur le contenu, et free "
		"sur le noeud");
}

/* fsoares 2 : contenu = adresse d'un pointeur, del = free(*x) */
static void	fs_delone_2(void)
{
	char	*s;
	t_list	*l;

	suivi_debut(0);
	s = dup_suivi("test");
	l = lst_noeud(&s);
	g_del_appels = 0;
	ESSAI("ft_lstdelone({content -> ptr -> \"test\"}, [(x) => free(*x)])");
	ft_lstdelone(l, del_free_ptr);
	suivi_fin();
	V_INT(g_del_appels, 1, "ft_lstdelone doit appeler del une fois, "
		"sur le contenu (pas sur le noeud)");
	T_FUITES("ft_lstdelone : n'utilise pas del sur le noeud, utilise free");
}

/* fsoares 3 : le noeud suivant ne doit pas etre libere */
static void	fs_delone_3(void)
{
	char	*s;
	t_list	*l;
	t_list	*suivant;
	size_t	vivantes;

	suivi_debut(0);
	s = dup_suivi("test");
	l = lst_creer(2, s, "second");
	suivant = l->next;
	g_del_appels = 0;
	ESSAI("ft_lstdelone([\"test\", \"second\"] (1er noeud), "
		"[(x) => free(x)])");
	ft_lstdelone(l, del_free);
	suivi_fin();
	V_INT(g_del_appels, 1, "ft_lstdelone doit appeler del une seule fois "
		"(seulement sur ce noeud)");
	if (!g_malloc_dispo)
	{
		free(suivant);
		return ;
	}
	vivantes = suivi_vivantes();
	verif(__LINE__, vivantes >= 1, "ft_lstdelone ne doit pas liberer le "
		"noeud suivant (lst->next)", "le noeud suivant a ete libere");
	if (vivantes >= 1)
	{
		free(suivant);
		T_FUITES("ft_lstdelone : le contenu et le noeud doivent etre liberes");
	}
}

static void	fs_lstdelone(void)
{
	T_SANS_CRASH(fs_delone_1, "ft_lstdelone({\"teste\"}, free)",
		"crash dans ft_lstdelone : appelle del(lst->content) puis free(lst)");
	T_SANS_CRASH(fs_delone_2, "ft_lstdelone({ptr -> \"test\"}, free(*x))",
		"crash : n'appelle pas del sur le noeud lui-meme (del(lst)), "
		"utilise free(lst)");
	T_SANS_CRASH(fs_delone_3, "ft_lstdelone(1er noeud de 2, free)",
		"crash : ft_lstdelone ne doit liberer que CE noeud, pas le suivant");
}

static void	tr_lstdelone(void)
{
	t_list	*l;

	suivi_debut(0);
	ESSAI("ft_lstdelone(ft_lstnew(malloc(1)), free)");
	l = ft_lstnew(malloc(1));
	g_del_appels = 0;
	if (!V_OK(l != NULL, EX_NEW))
	{
		suivi_fin();
		return ;
	}
	ft_lstdelone(l, del_free);
	suivi_fin();
	V_INT(g_del_appels, 1, EX_DELONE);
	T_FUITES("ft_lstdelone : le contenu (via del) et le noeud (via free) "
		"doivent etre liberes");
}

static void	al_lstdelone(void)
{
	t_list	*l;

	l = lst_noeud(malloc(10));
	g_del_appels = 0;
	ESSAI("ft_lstdelone({malloc(10)}, del)");
	ft_lstdelone(l, del_free);
	V_INT(g_del_appels, 1, "your lstdelone does not work : del n'a pas ete "
		"appele sur le contenu");
}

static void	wm_lstdelone(void)
{
	t_list	*e[4];

	wm_quatre(e, 1);
	g_del_appels = 0;
	ESSAI("ft_lstdelone(elem3 \"dolor\", &ft_del) dans "
		"[lorem, ipsum, dolor, sit]");
	ft_lstdelone(e[2], del_free);
	V_INT(g_del_appels, 1, EX_DELONE);
	V_STR((const char *)e[0]->content, "lorem", "ft_lstdelone ne doit "
		"toucher qu'au noeud donne");
	V_STR((const char *)e[1]->content, "ipsum", "ft_lstdelone ne doit "
		"toucher qu'au noeud donne");
	V_STR((const char *)e[3]->content, "sit", "ft_lstdelone ne doit pas "
		"liberer le noeud suivant");
}

/* ========================================================================== */
/*                               ft_lstclear                                  */
/* ========================================================================== */

static int			g_nb_clear;
static const char	*g_mots_clear[] = {"um", "dois", "tres", "quatre",
	"cinco"};

static void	fs_clear_cas(void)
{
	char	*args[10];
	t_list	*l;
	t_list	*dernier;
	t_list	*n;
	int		i;

	suivi_debut(0);
	l = NULL;
	dernier = NULL;
	i = 0;
	while (i < g_nb_clear)
	{
		args[i] = dup_suivi(g_nb_clear == 1 ? "hello!" : g_mots_clear[i]);
		n = lst_noeud(args + i);
		if (dernier)
			dernier->next = n;
		else
			l = n;
		dernier = n;
		i++;
	}
	g_del_appels = 0;
	ESSAI("ft_lstclear(&liste de %d noeud(s), [(x) => free(*x)])",
		g_nb_clear);
	ft_lstclear(&l, del_free_ptr);
	suivi_fin();
	V_PTR(l, NULL, NULL, "The value stored in the lst should be NULL after "
		"the clear : *lst doit valoir NULL");
	V_INT(g_del_appels, g_nb_clear, "ft_lstclear doit appeler del exactement "
		"une fois par noeud");
	T_FUITES(EX_CLEAR);
}

static void	fs_lstclear(void)
{
	g_nb_clear = 0;
	T_SANS_CRASH(fs_clear_cas, "ft_lstclear(&[] (NULL), free(*x))",
		"crash : ft_lstclear doit accepter une liste vide (*lst == NULL)");
	g_nb_clear = 1;
	T_SANS_CRASH(fs_clear_cas, "ft_lstclear(&[\"hello!\"], free(*x))",
		"crash dans ft_lstclear : del sur le contenu, free sur le noeud, "
		"et ne pas lire un noeud deja libere");
	g_nb_clear = 5;
	T_SANS_CRASH(fs_clear_cas, "ft_lstclear(&[um, dois, tres, quatre, "
		"cinco], free(*x))", "crash dans ft_lstclear : sauvegarde next "
		"AVANT de liberer le noeud");
}

static void	tr_lstclear(void)
{
	t_list	*l;
	int		i;

	suivi_debut(0);
	ESSAI("ft_lstnew(malloc(1)) + 10 x ft_lstadd_front(&l, "
		"ft_lstnew(malloc(1)))");
	l = ft_lstnew(malloc(1));
	i = 0;
	while (i < 10)
	{
		ft_lstadd_front(&l, ft_lstnew(malloc(1)));
		i++;
	}
	g_del_appels = 0;
	ESSAI("ft_lstclear(&l, free) sur une liste de 11 noeuds");
	ft_lstclear(&l, del_free);
	suivi_fin();
	V_PTR(l, NULL, NULL, "apres ft_lstclear, *lst doit valoir NULL");
	V_INT(g_del_appels, 11, "ft_lstclear doit appeler del une fois par noeud");
	T_FUITES(EX_CLEAR);
}

static void	al_lstclear(void)
{
	t_list	*l;
	t_list	*list;
	char	*content;

	l = lst_noeud(dup_suivi("nyancat"));
	l->next = lst_noeud(dup_suivi("#TEST#"));
	ESSAI("ft_lstclear(&[\"nyancat\", \"#TEST#\"], del)");
	ft_lstclear(&l, del_free);
	V_PTR(l, NULL, NULL, "your lstclear does not work with basic input : "
		"*lst doit valoir NULL");
	suivi_debut(0);
	l = lst_noeud(dup_suivi("nyancat"));
	l->next = lst_noeud(dup_suivi("#TEST#"));
	ESSAI("ft_lstclear(&[\"nyancat\", \"#TEST#\"], del) : tout doit etre "
		"libere");
	ft_lstclear(&l, del_free);
	suivi_fin();
	T_FUITES("your lstclear does not free the list : chaque noeud doit etre "
		"libere avec free");
	content = "hello !";
	list = lst_noeud(content);
	list->next = lst_noeud(content + 2);
	g_del_appels = 0;
	ESSAI("ft_lstclear(&[\"hello !\", \"llo !\"], del qui compte)");
	ft_lstclear(&list, del_compte);
	V_INT(g_del_appels, 2, "bad call number of the function pointer : del "
		"doit etre appele une fois par noeud");
	V_PTR(list, NULL, NULL, "apres ft_lstclear, *lst doit valoir NULL");
}

static void	wm_lstclear(void)
{
	t_list	*e[4];
	t_list	*elem3;

	wm_quatre(e, 1);
	elem3 = e[2];
	g_del_appels = 0;
	ESSAI("ft_lstclear(&elem3, &ft_del) dans [lorem, ipsum, dolor, sit]");
	ft_lstclear(&elem3, del_free);
	V_PTR(elem3, NULL, NULL, "apres ft_lstclear, *lst doit valoir NULL");
	V_STR((const char *)e[0]->content, "lorem", "ft_lstclear ne doit "
		"liberer qu'a partir du noeud donne");
	V_STR((const char *)e[1]->content, "ipsum", "ft_lstclear ne doit "
		"liberer qu'a partir du noeud donne");
	V_INT(g_del_appels, 2, "nb_free_done : del doit etre appele exactement "
		"une fois par noeud libere (dolor et sit)");
}

/* ========================================================================== */
/*                               ft_lstiter                                   */
/* ========================================================================== */

static void	fs_compte_car(void *s)
{
	char	*str;
	char	tmp[32];

	str = (char *)s;
	snprintf(tmp, sizeof(tmp), "%zu", strlen(str));
	memcpy(str, tmp, strlen(tmp) + 1);
}

static void	fs_lstiter(void)
{
	t_list		*l;
	const char	*a1[] = {"3"};
	const char	*a5[] = {"2", "4", "4", "6", "5"};

	ESSAI("ft_lstiter([] (NULL), [(s) => (s = strlen(s))])");
	suivi_debut(0);
	ft_lstiter(NULL, fs_compte_car);
	suivi_fin();
	V_OK(1, EX_ITER);
	sans_malloc(__LINE__, "ft_lstiter");
	l = lst_creer(1, dup_suivi("one"));
	ESSAI("ft_lstiter([\"one\"], [(s) => (s = strlen(s))])");
	suivi_debut(0);
	ft_lstiter(l, fs_compte_car);
	suivi_fin();
	V_LISTE(l, 1, a1, EX_ITER);
	sans_malloc(__LINE__, "ft_lstiter");
	lst_liberer_tout(l);
	l = lst_creer(5, dup_suivi("um"), dup_suivi("dois"), dup_suivi("tres"),
			dup_suivi("quatre"), dup_suivi("cinco"));
	ESSAI("ft_lstiter([\"um\", \"dois\", \"tres\", \"quatre\", \"cinco\"], "
		"[(s) => (s = strlen(s))])");
	suivi_debut(0);
	ft_lstiter(l, fs_compte_car);
	suivi_fin();
	V_LISTE(l, 5, a5, EX_ITER);
	sans_malloc(__LINE__, "ft_lstiter");
	lst_liberer_tout(l);
}

static void	tr_plus_un(void *p)
{
	++*(int *)p;
}

static void	tr_lstiter(void)
{
	int		tab[4];
	t_list	*l;
	t_list	*tmp;
	int		i;

	i = 0;
	while (i < 4)
	{
		tab[i] = i;
		i++;
	}
	ESSAI("construction de [0, 1, 2, 3] avec ft_lstnew + ft_lstlast");
	l = ft_lstnew(tab);
	i = 1;
	while (i < 4)
	{
		tr_add_back(&l, ft_lstnew(tab + i));
		i++;
	}
	ESSAI("ft_lstiter([0, 1, 2, 3] (int *), addOne)");
	ft_lstiter(l, tr_plus_un);
	tmp = l;
	i = 0;
	while (i < 4)
	{
		if (!V_OK(tmp != NULL, EX_ITER))
			return ;
		V_INT(*(int *)tmp->content, i + 1, EX_ITER);
		tmp = tmp->next;
		i++;
	}
	lst_liberer_noeuds(l);
}

static void	al_iter_ok(void *content)
{
	char	*s;

	s = content;
	s[0] = 'O';
	s[1] = 'K';
}

static void	al_lstiter(void)
{
	t_list		*l;
	const char	*att[] = {"OK !", "OK !", "OK !"};

	l = lst_noeud(dup_suivi("KO !"));
	l->next = lst_noeud(dup_suivi("KO !"));
	l->next->next = lst_noeud(dup_suivi("KO !"));
	ESSAI("ft_lstiter([\"KO !\", \"KO !\", \"KO !\"], f qui ecrit \"OK\")");
	ft_lstiter(l, al_iter_ok);
	V_LISTE(l, 3, att, "your lstiter does not work with basic input");
	lst_liberer_tout(l);
}

static void	wm_avec_d(void *elem)
{
	char	*content;
	int		len;

	len = 0;
	content = (char *)elem;
	while (content[len])
		content[len++] = 'd';
}

static void	wm_lstiter(void)
{
	t_list		*e[4];
	const char	*att[] = {"ddddd", "ddddd", "ddddd", "ddd"};

	wm_quatre(e, 1);
	ESSAI("ft_lstiter([lorem, ipsum, dolor, sit], "
		"&ft_modify_list_with_d)");
	ft_lstiter(e[0], wm_avec_d);
	V_LISTE(e[0], 4, att, EX_ITER);
	lst_liberer_tout(e[0]);
}

/* ========================================================================== */
/*                                ft_lstmap                                   */
/* ========================================================================== */

static int	g_crees;
static int	g_liberes;
static int	g_del_mauvais;

/* f de fsoares : "__" + strlen(s) en hexa (allocation hors suivi) */
static void	*fs_map_len(void *s)
{
	char	*str;

	str = malloc_reel(30);
	if (str)
	{
		snprintf(str, 30, "__%lX", (unsigned long)strlen((char *)s));
		g_crees++;
	}
	return (str);
}

/* del de fsoares : ne doit recevoir QUE des contenus crees par f */
static void	fs_map_del(void *content)
{
	char	*str;

	str = (char *)content;
	if (str == NULL)
		return ;
	if (str[0] != '_' || str[1] != '_')
	{
		g_del_mauvais++;
		return ;
	}
	g_liberes++;
	free_reel(content);
}

static void	fs_liberer_resultat(t_list *l)
{
	t_list	*suivant;
	int		i;

	i = 0;
	while (l && i < 1000)
	{
		suivant = l->next;
		fs_map_del(l->content);
		free(l);
		l = suivant;
		i++;
	}
}

static void	fs_map_cas(int ligne, t_list *init, int n, const char **orig,
	const char **att)
{
	t_list	*res;
	char	desc[512];
	size_t	nb;
	size_t	i;

	txt_liste(desc, sizeof(desc), init);
	essai(ligne, "ft_lstmap(%s, [s => __strlen(s)], [x => free(x)])", desc);
	g_crees = 0;
	g_liberes = 0;
	g_del_mauvais = 0;
	suivi_debut(0);
	res = ft_lstmap(init, fs_map_len, fs_map_del);
	suivi_fin();
	nb = suivi_nb_malloc();
	v_liste(ligne, res, n, att, EX_MAP);
	if (init)
		verif(ligne, res != init, "ft_lstmap doit renvoyer une NOUVELLE "
			"liste", "la liste d'origine a ete renvoyee");
	v_liste(ligne, init, n, orig, EX_MAP_ORIG);
	verif(ligne, g_del_mauvais == 0, "You are not using the del function "
		"correctly : del ne doit recevoir que des contenus crees par f",
		"del appele %d fois sur un contenu de la liste d'origine",
		g_del_mauvais);
	if (res != init)
		fs_liberer_resultat(res);
	if (g_malloc_dispo)
		t_fuites_verif(ligne, "ft_lstmap : les noeuds de la nouvelle liste");
	i = 1;
	while (g_malloc_dispo && i <= nb)
	{
		essai(ligne, "ft_lstmap(%s, ...)  [malloc numero %zu/%zu force a "
			"NULL]", desc, i, nb);
		g_crees = 0;
		g_liberes = 0;
		g_del_mauvais = 0;
		suivi_debut(i);
		res = ft_lstmap(init, fs_map_len, fs_map_del);
		suivi_fin();
		verif(ligne, res == NULL, "si un malloc echoue, ft_lstmap doit "
			"renvoyer NULL", "malloc numero %zu/%zu a renvoye NULL, mais "
			"ft_lstmap a renvoye %p", i, nb, (void *)res);
		if (res == NULL)
		{
			verif(ligne, suivi_vivantes() == 0, "si un malloc echoue, "
				"ft_lstmap doit liberer les noeuds deja crees (ft_lstclear)",
				"%zu noeud(s) non libere(s) apres l'echec du malloc numero "
				"%zu/%zu", suivi_vivantes(), i, nb);
			verif(ligne, g_crees == g_liberes, "Most likely you are not "
				"calling del on the content when a new list node allocation "
				"fails : si ft_lstnew echoue, appelle del sur le contenu que "
				"f vient de creer", "contenus crees par f : %d, liberes par "
				"del : %d", g_crees, g_liberes);
		}
		else if (res != init)
			fs_liberer_resultat(res);
		i++;
	}
}

static void	fs_lstmap(void)
{
	t_list		*l;
	const char	*o1[] = {"hello!"};
	const char	*a1[] = {"__6"};
	const char	*o5[] = {"one", "two", "three", "four", "five"};
	const char	*a5[] = {"__3", "__3", "__5", "__4", "__4"};

	fs_map_cas(__LINE__, NULL, 0, NULL, NULL);
	l = lst_creer(1, "hello!");
	fs_map_cas(__LINE__, l, 1, o1, a1);
	lst_liberer_noeuds(l);
	l = lst_creer(5, "one", "two", "three", "four", "five");
	fs_map_cas(__LINE__, l, 5, o5, a5);
	lst_liberer_noeuds(l);
}

static void	*tr_map_plus_un(void *p)
{
	void	*r;

	r = malloc(sizeof(int));
	if (r)
		*(int *)r = *(int *)p + 1;
	return (r);
}

static void	tr_lstmap(void)
{
	int		tab[4];
	t_list	*l;
	t_list	*m;
	t_list	*tmp;
	int		i;

	i = 0;
	while (i < 4)
	{
		tab[i] = i;
		i++;
	}
	suivi_debut(0);
	ESSAI("construction de [0, 1, 2, 3] avec ft_lstnew + ft_lstlast");
	l = ft_lstnew(tab);
	i = 1;
	while (i < 4)
	{
		tr_add_back(&l, ft_lstnew(tab + i));
		i++;
	}
	ESSAI("ft_lstmap([0, 1, 2, 3] (int *), addOne, free)");
	m = ft_lstmap(l, tr_map_plus_un, free);
	tmp = l;
	i = 0;
	while (i < 4)
	{
		if (!V_OK(tmp != NULL, EX_MAP_ORIG))
			return ;
		V_INT(*(int *)tmp->content, i, EX_MAP_ORIG);
		tmp = tmp->next;
		i++;
	}
	tmp = m;
	i = 0;
	while (i < 4)
	{
		if (!V_OK(tmp != NULL, EX_MAP))
			return ;
		V_INT(*(int *)tmp->content, i + 1, EX_MAP);
		tmp = tmp->next;
		i++;
	}
	V_PTR(tmp, NULL, NULL, "la nouvelle liste doit avoir autant de noeuds "
		"que l'originale");
	lst_liberer_noeuds(l);
	ESSAI("ft_lstclear(&m, free) sur le resultat de ft_lstmap");
	ft_lstclear(&m, free);
	suivi_fin();
	T_FUITES("ft_lstmap : chaque noeud cree doit pouvoir etre libere sans "
		"fuite");
}

static void	*al_map_ok(void *content)
{
	(void)content;
	return ((void *)"OK !");
}

static void	al_lstmap(void)
{
	t_list		*l;
	t_list		*ret;
	const char	*att[] = {"OK !", "OK !", "OK !"};
	const char	*orig[] = {" 1 2 3 ", "ss", "-_-"};

	l = lst_noeud(dup_suivi(" 1 2 3 "));
	l->next = lst_noeud(dup_suivi("ss"));
	l->next->next = lst_noeud(dup_suivi("-_-"));
	ESSAI("ft_lstmap([\" 1 2 3 \", \"ss\", \"-_-\"], f => \"OK !\", del)");
	ret = ft_lstmap(l, al_map_ok, del_compte);
	V_LISTE(ret, 3, att, "your lstmap does not work with basic input");
	V_LISTE(l, 3, orig, EX_MAP_ORIG);
	if (ret != l && liste_egale(ret, 3, att))
		lst_liberer_noeuds(ret);
	lst_liberer_tout(l);
}

static void	*wm_map_o_a(void *ct)
{
	char	*pouet;
	int		i;

	pouet = (char *)ct;
	i = -1;
	while (pouet[++i])
		if (pouet[i] == 'o')
			pouet[i] = 'a';
	return (ct);
}

static void	wm_lstmap(void)
{
	t_list		*e[4];
	t_list		*list;
	const char	*att[] = {"larem", "ipsum", "dalar", "sit"};

	wm_quatre(e, 1);
	ESSAI("ft_lstmap([lorem, ipsum, dolor, sit], &ft_map ('o' -> 'a'), "
		"&ft_del)");
	list = ft_lstmap(e[0], wm_map_o_a, del_free);
	if (!V_OK(list != NULL, EX_MAP))
		return ;
	V_OK(list != e[0], "A new list is not returned : ft_lstmap doit creer "
		"de nouveaux noeuds");
	V_LISTE(list, 4, att, EX_MAP);
	if (list != e[0] && liste_egale(list, 4, att))
		lst_liberer_noeuds(list);
	lst_liberer_tout(e[0]);
}

/* ========================================================================== */
/*                                  TABLE                                     */
/* ========================================================================== */

const t_test_fr	g_fr_bonus[] = {
	{"ft_lstnew", "fsoares", fs_lstnew},
	{"ft_lstnew", "Tripouille", tr_lstnew},
	{"ft_lstnew", "alelievr", al_lstnew},
	{"ft_lstnew", "war-machine", wm_lstnew},
	{"ft_lstadd_front", "fsoares", fs_lstadd_front},
	{"ft_lstadd_front", "Tripouille", tr_lstadd_front},
	{"ft_lstadd_front", "alelievr", al_lstadd_front},
	{"ft_lstadd_front", "war-machine", wm_lstadd_front},
	{"ft_lstsize", "fsoares", fs_lstsize},
	{"ft_lstsize", "Tripouille", tr_lstsize},
	{"ft_lstsize", "alelievr", al_lstsize},
	{"ft_lstsize", "war-machine", wm_lstsize},
	{"ft_lstlast", "fsoares", fs_lstlast},
	{"ft_lstlast", "Tripouille", tr_lstlast},
	{"ft_lstlast", "alelievr", al_lstlast},
	{"ft_lstlast", "war-machine", wm_lstlast},
	{"ft_lstadd_back", "fsoares", fs_lstadd_back},
	{"ft_lstadd_back", "Tripouille", tr_lstadd_back},
	{"ft_lstadd_back", "alelievr", al_lstadd_back},
	{"ft_lstadd_back", "war-machine", wm_lstadd_back},
	{"ft_lstdelone", "fsoares", fs_lstdelone},
	{"ft_lstdelone", "Tripouille", tr_lstdelone},
	{"ft_lstdelone", "alelievr", al_lstdelone},
	{"ft_lstdelone", "war-machine", wm_lstdelone},
	{"ft_lstclear", "fsoares", fs_lstclear},
	{"ft_lstclear", "Tripouille", tr_lstclear},
	{"ft_lstclear", "alelievr", al_lstclear},
	{"ft_lstclear", "war-machine", wm_lstclear},
	{"ft_lstiter", "fsoares", fs_lstiter},
	{"ft_lstiter", "Tripouille", tr_lstiter},
	{"ft_lstiter", "alelievr", al_lstiter},
	{"ft_lstiter", "war-machine", wm_lstiter},
	{"ft_lstmap", "fsoares", fs_lstmap},
	{"ft_lstmap", "Tripouille", tr_lstmap},
	{"ft_lstmap", "alelievr", al_lstmap},
	{"ft_lstmap", "war-machine", wm_lstmap},
	{NULL, NULL, NULL}
};
