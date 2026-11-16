/*
** norminette.c - lance norminette sur les sources de la libft et affiche,
** pour chaque fichier, [OK] en vert ou [KO] en rouge. Pour chaque erreur :
** ligne, colonne, code norminette, message d'origine et explication en
** francais.
**
** Seuls les .c et .h a la RACINE du dossier sont verifies (le dossier
** test/ n'est pas a rendre et n'est pas a la norme).
*/
#include "test.h"
#include <dirent.h>
#include <sys/stat.h>

typedef struct s_norm_expl
{
	const char	*code;
	const char	*texte;
}	t_norm_expl;

/*
** Codes de norminette v3 (norminette/norm_error.py) et leur explication.
*/
static const t_norm_expl	g_norm_expl[] = {
	/* ----- espaces / tabulations ----- */
	{"SPC_INSTEAD_TAB", "Espaces en debut de ligne : l'indentation se fait "
		"uniquement avec des tabulations."},
	{"TAB_INSTEAD_SPC", "Tabulation trouvee la ou un espace est attendu."},
	{"SPACE_REPLACE_TAB", "Un espace a ete trouve la ou une tabulation est "
		"attendue (indentation, ou entre le type et le nom)."},
	{"TAB_REPLACE_SPACE", "Une tabulation a ete trouvee la ou un espace "
		"simple est attendu."},
	{"CONSECUTIVE_SPC", "Deux espaces (ou plus) a la suite : un seul espace "
		"est autorise."},
	{"CONSECUTIVE_WS", "Plusieurs blancs (espaces/tabulations) a la suite "
		"a un endroit ou un seul est autorise."},
	{"SPC_BEFORE_NL", "Espace ou tabulation en fin de ligne : supprime les "
		"blancs avant le retour a la ligne."},
	{"SPACE_EMPTY_LINE", "Une ligne vide contient des espaces ou des "
		"tabulations : elle doit etre totalement vide."},
	{"MIXED_SPACE_TAB", "Melange d'espaces et de tabulations : n'utilise "
		"que des tabulations pour indenter et aligner."},
	{"SPC_LINE_START", "Espace ou tabulation inattendu en debut de ligne."},
	{"SPACE_BEFORE_FUNC", "Il faut une TABULATION (pas un espace) entre le "
		"type de retour et le nom de la fonction."},
	{"TOO_MANY_TABS_FUNC", "Trop de tabulations avant le nom de la fonction "
		"(alignement incorrect)."},
	{"TOO_MANY_TABS_TD", "Trop de tabulations avant le nom du typedef."},
	{"MISSING_TAB_FUNC", "Il manque une tabulation avant le nom de la "
		"fonction."},
	{"MISSING_TAB_VAR", "Il manque une tabulation avant le nom de la "
		"variable."},
	{"MISSING_TAB_TYPDEF", "Il manque une tabulation avant le nom du "
		"typedef."},
	{"TOO_MANY_TAB_VAR", "Trop de tabulations avant le nom de la variable."},
	{"NO_TAB_BF_TYPEDEF", "Il manque un blanc (tabulation) avant le nom du "
		"typedef."},
	{"TOO_FEW_TAB", "Indentation insuffisante : il manque des tabulations "
		"pour ce niveau de bloc."},
	{"TOO_MANY_TAB", "Indentation trop grande : trop de tabulations pour ce "
		"niveau de bloc."},
	{"TOO_MANY_WS", "Trop de blancs pour ce niveau d'indentation."},
	{"EXP_TAB", "Une tabulation est attendue ici."},
	/* ----- espaces autour des operateurs / parentheses ----- */
	{"SPC_BFR_OPERATOR", "Il manque un espace AVANT l'operateur "
		"(ex : a + b, pas a+ b)."},
	{"SPC_AFTER_OPERATOR", "Il manque un espace APRES l'operateur "
		"(ex : a + b, pas a +b)."},
	{"NO_SPC_BFR_OPR", "Espace en trop AVANT l'operateur (ex : i++, pas "
		"i ++)."},
	{"NO_SPC_AFR_OPR", "Espace en trop APRES l'operateur (ex : -n, *p, !x)."},
	{"SPC_AFTER_PAR", "Il manque un espace apres la parenthese / accolade / "
		"crochet."},
	{"SPC_BFR_PAR", "Il manque un espace avant la parenthese / accolade / "
		"crochet."},
	{"NO_SPC_AFR_PAR", "Espace en trop apres une parenthese ouvrante "
		"(ex : f(x), pas f( x))."},
	{"NO_SPC_BFR_PAR", "Espace en trop avant une parenthese fermante ou "
		"entre le nom d'une fonction et '(' (ex : f(x), pas f (x))."},
	{"SPC_AFTER_POINTER", "Espace en trop apres l'etoile du pointeur : "
		"ecris char *s, pas char * s."},
	{"SPC_BFR_POINTER", "Mauvais espacement avant l'etoile du pointeur : "
		"ecris char *s (etoile collee au nom)."},
	{"SPACE_AFTER_KW", "Il manque un espace apres le mot-cle (if, while, "
		"return, sizeof...) : ecris if (x), return (x)."},
	{"COMMA_START_LINE", "Une virgule ne doit pas commencer une ligne : "
		"laisse-la en fin de ligne precedente."},
	/* ----- longueurs / quantites ----- */
	{"LINE_TOO_LONG", "Ligne trop longue : 80 colonnes maximum (une "
		"tabulation compte jusqu'a la prochaine colonne multiple de 4)."},
	{"TOO_MANY_LINES", "La fonction depasse 25 lignes entre ses accolades : "
		"decoupe-la en fonctions auxiliaires (static)."},
	{"TOO_MANY_FUNCS", "Plus de 5 fonctions dans ce fichier : deplace "
		"certaines fonctions dans un autre fichier."},
	{"TOO_MANY_ARGS", "La fonction a plus de 4 parametres : regroupe-les "
		"dans une structure ou revois le decoupage."},
	{"TOO_MANY_VARS_FUNC", "Plus de 5 variables declarees dans la "
		"fonction : decoupe-la ou reutilise des variables."},
	{"TOO_MANY_INSTR", "Plusieurs instructions sur la meme ligne : une seule "
		"instruction par ligne."},
	{"TOO_MANY_VALS", "Trop de valeurs dans le #define."},
	/* ----- declarations ----- */
	{"DECL_ASSIGN_LINE", "Declaration et affectation sur la meme ligne "
		"interdites : declare d'abord (int i;), affecte ensuite (i = 0;)."},
	{"MULT_DECL_LINE", "Plusieurs declarations sur une meme ligne : une "
		"seule variable declaree par ligne."},
	{"MULT_ASSIGN_LINE", "Plusieurs affectations sur une meme ligne "
		"(ex : a = b = 0) : une seule affectation par ligne."},
	{"MULT_IN_SINGLE_INSTR", "Plusieurs instructions dans une structure de "
		"controle sur une seule ligne."},
	{"VAR_DECL_START_FUNC", "Les declarations de variables doivent toutes "
		"etre au tout debut de la fonction."},
	{"NL_AFTER_VAR_DECL", "Il faut une ligne vide juste apres le bloc de "
		"declarations de variables."},
	{"WRONG_SCOPE_VAR", "Variable declaree dans un mauvais bloc : declare-la "
		"au debut de la fonction."},
	{"IMPLICIT_VAR_TYPE", "Type manquant dans la declaration de variable."},
	{"MISALIGNED_VAR_DECL", "Declarations de variables mal alignees : les "
		"noms doivent commencer sur la meme colonne (avec des tabulations)."},
	{"MISALIGNED_FUNC_DECL", "Prototypes / definitions mal alignes : les noms "
		"de fonctions doivent etre alignes avec des tabulations."},
	{"VLA_FORBIDDEN", "Tableau de taille variable (VLA) interdit : utilise "
		"une taille constante ou malloc."},
	{"NEWLINE_IN_DECL", "Retour a la ligne au milieu d'une declaration."},
	{"ARG_TYPE_UKN", "Type de variable non reconnu par norminette (typedef "
		"inconnu ? include manquant ?)."},
	{"MISSING_IDENTIFIER", "Il manque le type ou le nom d'un parametre dans "
		"le prototype."},
	{"NO_ARGS_VOID", "Une fonction sans parametre doit etre declaree avec "
		"(void), ex : int f(void)."},
	/* ----- lignes vides ----- */
	{"EMPTY_LINE_FUNCTION", "Ligne vide interdite dans une fonction (seule "
		"celle qui suit les declarations est autorisee)."},
	{"EMPTY_LINE_FILE_START", "Ligne vide en debut de fichier : le header 42 "
		"doit etre sur la premiere ligne."},
	{"EMPTY_LINE_EOF", "Ligne vide en fin de fichier : supprime-la."},
	{"CONSECUTIVE_NEWLINES", "Deux lignes vides consecutives : une seule "
		"ligne vide maximum."},
	{"NEWLINE_PRECEDES_FUNC", "Les fonctions doivent etre separees par "
		"exactement une ligne vide."},
	{"NL_AFTER_PREPROC", "Il faut une ligne vide apres les directives de "
		"preprocesseur."},
	/* ----- accolades / retours a la ligne ----- */
	{"BRACE_NEWLINE", "Une accolade doit etre seule sur sa ligne : retour "
		"a la ligne AVANT l'accolade."},
	{"BRACE_SHOULD_EOL", "Retour a la ligne attendu APRES l'accolade."},
	{"EXP_NEWLINE", "Retour a la ligne attendu apres la structure de "
		"controle (if/while) : l'instruction va sur la ligne suivante."},
	{"EXP_SEMI_COLON", "Point-virgule attendu."},
	{"EXP_PARENTHESIS", "Parenthese attendue."},
	{"EOL_OPERATOR", "Un operateur (&&, ||, +...) ne doit pas finir une "
		"ligne : coupe la ligne AVANT l'operateur."},
	{"ATTR_EOL", "Un attribut de fonction doit etre en fin de ligne."},
	/* ----- structures interdites ----- */
	{"FORBIDDEN_CS", "Structure de controle interdite (for, do...while, "
		"switch, case) : utilise while et if."},
	{"TERNARY_FBIDDEN", "Operateur ternaire (a ? b : c) interdit : utilise "
		"if / else."},
	{"GOTO_FBIDDEN", "goto est interdit."},
	{"LABEL_FBIDDEN", "Les labels (etiquettes) sont interdits."},
	{"ASSIGN_IN_CONTROL", "Affectation dans une condition interdite "
		"(ex : while ((c = s[i]))) : fais l'affectation avant."},
	{"RETURN_PARENTHESIS", "La valeur de retour doit etre entre "
		"parentheses : return (x);"},
	/* ----- commentaires / header ----- */
	{"WRONG_SCOPE_COMMENT", "Commentaire interdit a l'interieur d'une "
		"fonction : place-le au-dessus de la fonction."},
	{"COMMENT_ON_INSTR", "Un commentaire doit etre sur sa propre ligne ou "
		"en fin de ligne."},
	{"INVALID_HEADER", "Header 42 absent ou abime : regenere-le (vim :Stdheader "
		"ou F1) sans le modifier a la main."},
	/* ----- nommage ----- */
	{"FORBIDDEN_CHAR_NAME", "Les noms (variables, fonctions, types) ne "
		"doivent contenir que des minuscules, chiffres et '_'."},
	{"USER_DEFINED_TYPEDEF", "Un typedef doit commencer par t_ "
		"(ex : t_list)."},
	{"STRUCT_TYPE_NAMING", "Un nom de structure doit commencer par s_ "
		"(ex : struct s_list)."},
	{"ENUM_TYPE_NAMING", "Un nom d'enum doit commencer par e_."},
	{"UNION_TYPE_NAMING", "Un nom d'union doit commencer par u_."},
	{"GLOBAL_VAR_NAMING", "Une variable globale doit commencer par g_."},
	{"GLOBAL_VAR_DETECTED", "Variable globale detectee : elles sont "
		"interdites dans la libft."},
	{"MACRO_NAME_CAPITAL", "Un nom de macro doit etre en MAJUSCULES."},
	{"MACRO_FUNC_FORBIDDEN", "Les macros-fonctions (#define f(x) ...) sont "
		"interdites."},
	{"MISSING_TYPEDEF_ID", "Il manque le nom dans la declaration du "
		"typedef."},
	/* ----- portee ----- */
	{"WRONG_SCOPE_FCT", "Prototype de fonction a un mauvais endroit (il doit "
		"etre hors de toute fonction, idealement dans le .h)."},
	{"WRONG_SCOPE", "Instruction placee a un endroit interdit."},
	{"TYPE_NOT_GLOBAL", "Les enum, struct et union doivent etre definies "
		"au niveau global."},
	{"FORBIDDEN_TYPEDEF", "typedef interdit dans un .c : mets-le dans le "
		".h."},
	{"FORBIDDEN_STRUCT", "Declaration de structure interdite dans un .c : "
		"mets-la dans le .h."},
	{"FORBIDDEN_UNION", "Declaration d'union interdite dans un .c : "
		"mets-la dans le .h."},
	{"FORBIDDEN_ENUM", "Declaration d'enum interdite dans un .c : mets-la "
		"dans le .h."},
	/* ----- preprocesseur / includes / header guard ----- */
	{"PREPROC_NO_SPACE", "Il manque un espace apres la directive de "
		"preprocesseur."},
	{"PREPROC_UKN_STATEMENT", "Directive de preprocesseur non reconnue."},
	{"PREPROC_START_LINE", "Une directive de preprocesseur doit commencer "
		"en debut de ligne (le # en colonne 1)."},
	{"PREPROC_CONSTANT", "Un #define ne doit contenir que des constantes."},
	{"PREPROC_EXPECTED_EOL", "Fin de ligne attendue apres la directive de "
		"preprocesseur."},
	{"PREPROC_BAD_IF", "#if sans #endif."},
	{"PREPROC_BAD_ELIF", "#elif sans #if."},
	{"PREPROC_BAD_IFDEF", "#ifdef sans #endif."},
	{"PREPROC_BAD_IFNDEF", "#ifndef sans #endif."},
	{"PREPROC_BAD_ELSE", "#else sans #if."},
	{"PREPROC_BAD_ENDIF", "#endif sans #if / #ifdef / #ifndef."},
	{"PREPROC_BAD_INDENT", "Mauvaise indentation du preprocesseur : dans un "
		"#ifndef, ecris \"# define\", \"# include\" (un espace par niveau)."},
	{"PREPROC_MULTLINE", "Directive de preprocesseur sur plusieurs lignes "
		"interdite."},
	{"PREPOC_ONLY_GLOBAL", "Les directives de preprocesseur ne sont "
		"autorisees qu'au niveau global (pas dans une fonction)."},
	{"PREPROC_GLOBAL", "Le preprocesseur ne peut etre utilise qu'au niveau "
		"global."},
	{"NEWLINE_DEFINE", "Retour a la ligne dans un #define."},
	{"INCORRECT_DEFINE", "Valeur incorrecte dans le #define."},
	{"INCLUDE_HEADER_ONLY", "Seuls les fichiers .h peuvent etre inclus "
		"(#include d'un .c interdit)."},
	{"INCLUDE_START_FILE", "Les #include doivent etre en debut de fichier "
		"(juste apres le header 42)."},
	{"INCLUDE_MISSING_SP", "Il manque un espace entre #include et le nom du "
		"fichier."},
	{"HEADER_PROT_ALL", "La protection du header (#ifndef / #define / "
		"#endif) doit englober tout le contenu du .h."},
	{"HEADER_PROT_ALL_AF", "Rien ne doit se trouver apres le #endif de la "
		"protection du header."},
	{"HEADER_PROT_NAME", "Nom de protection incorrect : pour libft.h il "
		"faut LIBFT_H."},
	{"HEADER_PROT_UPPER", "Le nom de la protection du header doit etre en "
		"MAJUSCULES."},
	{"HEADER_PROT_MULT", "Plusieurs protections de header : une seule est "
		"autorisee."},
	{"HEADER_PROT_NODEF", "La protection du header ne contient pas de "
		"#define."},
	/* ----- lexer ----- */
	{"UNEXPECTED_EOF_CHR", "Fin de fichier inattendue dans un caractere "
		"(apostrophe non fermee ?)."},
	{"UNEXPECTED_EOL_CHR", "Fin de ligne inattendue dans un caractere "
		"(apostrophe non fermee ?)."},
	{"UNEXPECTED_EOF_MC", "Fin de fichier dans un commentaire /* ... */ non "
		"ferme."},
	{"UNEXPECTED_EOF_STR", "Fin de fichier dans une chaine : guillemet non "
		"ferme ?"},
	{"EMPTY_CHAR", "Caractere vide '' : il faut un caractere entre les "
		"apostrophes."},
	{"CHAR_AS_STRING", "Une constante caractere ne contient qu'un seul "
		"caractere : utilise des guillemets pour une chaine."},
	{"UNKNOWN_ESCAPE", "Sequence d'echappement inconnue (ex : \\q)."},
	{"NO_HEX_DIGITS", "\\x doit etre suivi de chiffres hexadecimaux."},
	{"MAXIMAL_MUNCH", "Suite d'operateurs ambigue (ex : a+++b) : ajoute des "
		"espaces ou des parentheses."},
	{"INVALID_SUFFIX", "Suffixe de constante invalide."},
	{"BAD_FLOAT_SUFFIX", "Suffixe invalide pour une constante flottante."},
	{"INVALID_BIN_INT", "Constante binaire invalide."},
	{"INVALID_OCT_INT", "Constante octale invalide."},
	{"INVALID_HEX_INT", "Constante hexadecimale invalide."},
	{"BAD_EXPONENT", "Exposant sans chiffre dans une constante flottante."},
	{"MULTIPLE_DOTS", "Plusieurs points dans une constante flottante."},
	{"MULTIPLE_X", "Plusieurs 'x' dans une constante hexadecimale."},
	{NULL, NULL}
};

static const char	*norm_explication(const char *code)
{
	size_t	i;

	i = 0;
	while (g_norm_expl[i].code)
	{
		if (!strcmp(g_norm_expl[i].code, code))
			return (g_norm_expl[i].texte);
		i++;
	}
	return (NULL);
}

static int	norm_cmp(const void *a, const void *b)
{
	return (strcmp(*(char *const *)a, *(char *const *)b));
}

static int	norm_est_source(const char *dossier, const char *nom)
{
	size_t		n;
	char		chemin[4096];
	struct stat	st;

	n = strlen(nom);
	if (n < 3 || nom[0] == '.')
		return (0);
	if (strcmp(nom + n - 2, ".c") && strcmp(nom + n - 2, ".h"))
		return (0);
	snprintf(chemin, sizeof(chemin), "%s/%s", dossier, nom);
	if (stat(chemin, &st) != 0 || !S_ISREG(st.st_mode))
		return (0);
	return (1);
}

/* Liste triee des .c / .h a la racine de dossier. */
static char	**norm_lister(const char *dossier, int *nb)
{
	DIR				*d;
	struct dirent	*e;
	char			**liste;
	char			**tmp;
	int				cap;

	*nb = 0;
	d = opendir(dossier);
	if (!d)
		return (NULL);
	cap = 64;
	liste = malloc(sizeof(*liste) * cap);
	while (liste && (e = readdir(d)) != NULL)
	{
		if (!norm_est_source(dossier, e->d_name))
			continue ;
		if (*nb == cap)
		{
			cap *= 2;
			tmp = realloc(liste, sizeof(*liste) * cap);
			if (!tmp)
				break ;
			liste = tmp;
		}
		liste[*nb] = strdup(e->d_name);
		if (liste[*nb])
			(*nb)++;
	}
	closedir(d);
	if (liste && *nb > 1)
		qsort(liste, *nb, sizeof(*liste), norm_cmp);
	return (liste);
}

static void	norm_liberer(char **liste, int nb)
{
	int	i;

	i = 0;
	while (liste && i < nb)
		free(liste[i++]);
	free(liste);
}

/* Ajoute s entre quotes simples (shell) a la fin de dst. */
static void	norm_ajouter_quote(char *dst, const char *s)
{
	size_t	j;

	j = strlen(dst);
	dst[j++] = '\'';
	while (*s)
	{
		if (*s == '\'')
		{
			memcpy(dst + j, "'\\''", 4);
			j += 4;
		}
		else
			dst[j++] = *s;
		s++;
	}
	dst[j++] = '\'';
	dst[j] = '\0';
}

static char	*norm_commande(const char *dossier, char **liste, int nb)
{
	size_t	taille;
	char	*cmd;
	int		i;

	taille = strlen(dossier) * 4 + 128;
	i = 0;
	while (i < nb)
		taille += strlen(liste[i++]) * 4 + 4;
	cmd = malloc(taille);
	if (!cmd)
		return (NULL);
	strcpy(cmd, "cd ");
	norm_ajouter_quote(cmd, dossier);
	strcat(cmd, " && PATH=\"$PATH:$HOME/.local/bin\" norminette");
	i = 0;
	while (i < nb)
	{
		strcat(cmd, " ");
		norm_ajouter_quote(cmd, liste[i++]);
	}
	strcat(cmd, " 2>&1");
	return (cmd);
}

/* Retire les sequences de couleur ANSI et le retour a la ligne final. */
static void	norm_nettoyer(char *ligne)
{
	char	*r;
	char	*w;
	size_t	n;

	r = ligne;
	w = ligne;
	while (*r)
	{
		if (*r == '\033' && r[1] == '[')
		{
			r += 2;
			while (*r && !isalpha((unsigned char)*r))
				r++;
			if (*r)
				r++;
			continue ;
		}
		*w++ = *r++;
	}
	*w = '\0';
	n = strlen(ligne);
	while (n > 0 && (ligne[n - 1] == '\n' || ligne[n - 1] == '\r'))
		ligne[--n] = '\0';
}

static int	norm_finit_par(const char *s, const char *fin)
{
	size_t	ls;
	size_t	lf;

	ls = strlen(s);
	lf = strlen(fin);
	return (ls >= lf && !strcmp(s + ls - lf, fin));
}

/*
** Ligne "Error: CODE   (line:  12, col:   5):\tMessage"
** ou "Notice: ..." / "Warning: ...". Renvoie 0 si non reconnue.
*/
static int	norm_afficher_erreur(const char *ligne)
{
	const char	*p;
	char		code[128];
	size_t		k;
	int			l;
	int			c;
	int			remarque;
	int			avec_pos;
	const char	*expl;

	remarque = 0;
	if (!strncmp(ligne, "Error:", 6))
		p = ligne + 6;
	else if (!strncmp(ligne, "Notice:", 7) && (remarque = 1))
		p = ligne + 7;
	else if (!strncmp(ligne, "Warning:", 8) && (remarque = 1))
		p = ligne + 8;
	else
		return (0);
	while (*p == ' ' || *p == '\t')
		p++;
	k = 0;
	while (*p && *p != ' ' && *p != '\t' && *p != '(' && k + 1 < sizeof(code))
		code[k++] = *p++;
	code[k] = '\0';
	while (*p == ' ' || *p == '\t')
		p++;
	avec_pos = 0;
	if (sscanf(p, "(line: %d, col: %d)", &l, &c) == 2)
	{
		avec_pos = 1;
		p = strstr(p, "):");
		p = p ? p + 2 : "";
	}
	while (*p == ' ' || *p == '\t' || *p == ':')
		p++;
	printf("    ");
	if (remarque)
		printf(JAUNE "remarque" NC " ");
	if (avec_pos)
		printf("ligne %d, col %d : ", l, c);
	printf(JAUNE "%s" NC, code);
	if (*p)
		printf(" - %s", p);
	printf("\n");
	expl = norm_explication(code);
	if (expl)
		printf("      " CYAN "-> " NC "%s\n", expl);
	return (1);
}

/*
** Lignes lues avant le premier "fichier: OK!/Error!" (message d'erreur du
** shell, trace python...) : gardees pour decider a la fin.
*/
#define NORM_AVANT_MAX 8192

int	norminette_verifier(const char *dossier, int *nb_fichiers)
{
	char	**liste;
	char	*cmd;
	FILE	*p;
	char	ligne[4096];
	char	avant[NORM_AVANT_MAX];
	int		nb;
	int		vus;
	int		ko;
	int		statut;
	size_t	n;

	if (nb_fichiers)
		*nb_fichiers = 0;
	liste = norm_lister(dossier, &nb);
	if (!liste || nb == 0)
	{
		printf(JAUNE "[--]" NC " aucun fichier .c / .h trouve dans %s\n",
			dossier);
		norm_liberer(liste, nb);
		return (0);
	}
	cmd = norm_commande(dossier, liste, nb);
	norm_liberer(liste, nb);
	if (!cmd)
		return (-1);
	fflush(stdout);
	p = popen(cmd, "r");
	free(cmd);
	if (!p)
	{
		printf(JAUNE "[--]" NC " impossible de lancer norminette (popen)\n");
		return (-1);
	}
	vus = 0;
	ko = 0;
	avant[0] = '\0';
	while (fgets(ligne, sizeof(ligne), p))
	{
		norm_nettoyer(ligne);
		if (ligne[0] == '\0')
			continue ;
		if (norm_finit_par(ligne, ": OK!") || norm_finit_par(ligne, ": Error!"))
		{
			n = strlen(ligne) - (norm_finit_par(ligne, ": OK!") ? 5 : 8);
			ligne[n] = '\0';
			if (ligne[n + 2] == 'O')
				printf(TAG_OK " %s\n", ligne);
			else
			{
				printf(TAG_KO " %s\n", ligne);
				ko++;
			}
			vus++;
			continue ;
		}
		if (vus == 0)
		{
			n = strlen(avant);
			if (n + strlen(ligne) + 2 < sizeof(avant))
			{
				strcat(avant, ligne);
				strcat(avant, "\n");
			}
			continue ;
		}
		if (!norm_afficher_erreur(ligne))
			printf("    " GRIS "%s" NC "\n", ligne);
	}
	statut = pclose(p);
	if (vus == 0)
	{
		if ((WIFEXITED(statut) && WEXITSTATUS(statut) == 127)
			|| strstr(avant, "not found") || strstr(avant, "introuvable"))
			printf(JAUNE "[--] norminette introuvable : installe-la avec "
				" python3 -m pip install --user norminette" NC "\n");
		else
		{
			printf(JAUNE "[--] norminette n'a renvoye aucun resultat :" NC
				"\n");
			if (avant[0])
				printf(GRIS "%s" NC, avant);
		}
		return (-1);
	}
	if (avant[0])
		printf(GRIS "%s" NC, avant);
	if (vus < nb)
		printf(JAUNE "[--]" NC " %d fichier(s) sans resultat norminette\n",
			nb - vus);
	if (nb_fichiers)
		*nb_fichiers = nb;
	return (ko);
}
