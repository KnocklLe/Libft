/*
** Tests Libft v19.3 - 43 fonctions (parties 1, 2 et 3)
** V4 : tests exhaustifs bornes, callbacks, fichiers FD et malloc en echec.
** Compile sans -std : cc -Wall -Wextra -Werror main.c libft.a -o test_libft
** -DTEST_WRAP_MALLOC et -Wl,--wrap=malloc,--wrap=free : injections optionnelles.
** Ne pas ajouter ce fichier aux SOURCES du Makefile du projet.
*/
#include "../libft.h"
#include <ctype.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>

#define TOURS 1500
#define CAP 256

static unsigned long g_tests;
static unsigned long g_erreurs;
static unsigned long g_debut_tests;
static unsigned long g_debut_erreurs;
static const char *g_fonction;
static unsigned int g_graine = 0x42C0FFEEu;
static int g_destructions;

static unsigned int aleatoire(void)
{
    g_graine = g_graine * 1664525u + 1013904223u;
    return (g_graine);
}

static size_t hasard(size_t maximum)
{
    if (maximum == 0)
        return (0);
    return ((size_t)(aleatoire() % (unsigned int)maximum));
}

static void texte_aleatoire(char *dst, size_t taille, int jeu)
{
    const char *alphabet = jeu ? "abcXyZ 001_-\n\r\t\v\f" : "aaabbbcccddeee\n\r\t\v\f";
    size_t i;
    size_t longueur;

    longueur = strlen(alphabet);
    i = 0;
    while (i < taille)
    {
        dst[i] = alphabet[hasard(longueur)];
        i++;
    }
    dst[i] = '\0';
}

static void octets_aleatoires(unsigned char *dst, size_t taille)
{
    size_t i;

    i = 0;
    while (i < taille)
    {
        dst[i] = (unsigned char)(aleatoire() >> 24);
        i++;
    }
}

static void verifier(int ok, const char *details, ...)
{
    va_list args;

    g_tests++;
    if (ok)
        return ;
    g_erreurs++;
    if (g_erreurs > 80)
        return ;
    printf("\n  [FAIL] %s : ", g_fonction);
    va_start(args, details);
    vprintf(details, args);
    va_end(args);
}

static void debut(const char *nom)
{
    g_fonction = nom;
    g_debut_tests = g_tests;
    g_debut_erreurs = g_erreurs;
}

static void fin(void)
{
    printf("%s %s (%lu tests, %lu erreurs)\n",
        g_erreurs == g_debut_erreurs ? "[OK]  " : "[FAIL]",
        g_fonction, g_tests - g_debut_tests,
        g_erreurs - g_debut_erreurs);
}

static int signe(int n)
{
    return ((n > 0) - (n < 0));
}

static size_t copie_limitee_ref(char *dst, const char *src, size_t taille)
{
    size_t len;
    size_t i;

    len = strlen(src);
    if (taille == 0)
        return (len);
    i = 0;
    while (i < taille - 1 && src[i])
    {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
    return (len);
}

static size_t concat_limitee_ref(char *dst, const char *src, size_t taille)
{
    size_t d;
    size_t s;
    size_t i;

    d = 0;
    while (d < taille && dst[d])
        d++;
    s = strlen(src);
    if (d == taille)
        return (taille + s);
    i = 0;
    while (i < taille - d - 1 && src[i])
    {
        dst[d + i] = src[i];
        i++;
    }
    dst[d + i] = '\0';
    return (d + s);
}

static char *cherche_limitee_ref(const char *grand, const char *petit,
    size_t limite)
{
    size_t i;
    size_t j;

    if (*petit == '\0')
        return ((char *)grand);
    i = 0;
    while (i < limite && grand[i])
    {
        j = 0;
        while (petit[j] && i + j < limite && grand[i + j]
            && grand[i + j] == petit[j])
            j++;
        if (!petit[j])
            return ((char *)grand + i);
        i++;
    }
    return (NULL);
}

static void liberer_mots(char **mots)
{
    size_t i;

    if (!mots)
        return ;
    i = 0;
    while (mots[i])
        free(mots[i++]);
    free(mots);
}

static char **decouper_ref(const char *s, char separateur)
{
    size_t i;
    size_t debut;
    size_t nb;
    size_t k;
    char **mots;

    nb = 0;
    i = 0;
    while (s[i])
    {
        while (s[i] && s[i] == separateur)
            i++;
        if (!s[i])
            break ;
        nb++;
        while (s[i] && s[i] != separateur)
            i++;
    }
    mots = calloc(nb + 1, sizeof(*mots));
    if (!mots)
        return (NULL);
    i = 0;
    k = 0;
    while (s[i])
    {
        while (s[i] && s[i] == separateur)
            i++;
        if (!s[i])
            break ;
        debut = i;
        while (s[i] && s[i] != separateur)
            i++;
        mots[k] = malloc(i - debut + 1);
        if (!mots[k])
            return (liberer_mots(mots), NULL);
        memcpy(mots[k], s + debut, i - debut);
        mots[k][i - debut] = '\0';
        k++;
    }
    return (mots);
}

static int memes_mots(char **a, char **b)
{
    size_t i;

    if (!a || !b)
        return (a == b);
    i = 0;
    while (a[i] && b[i])
    {
        if (strcmp(a[i], b[i]) != 0)
            return (0);
        i++;
    }
    return (a[i] == NULL && b[i] == NULL);
}

static char *elaguer_ref(const char *s, const char *ensemble)
{
    size_t debut;
    size_t fin;
    size_t longueur;
    char *r;

    debut = 0;
    fin = strlen(s);
    while (debut < fin && strchr(ensemble, s[debut]))
        debut++;
    while (fin > debut && strchr(ensemble, s[fin - 1]))
        fin--;
    longueur = fin - debut;
    r = malloc(longueur + 1);
    if (!r)
        return (NULL);
    memcpy(r, s + debut, longueur);
    r[longueur] = '\0';
    return (r);
}

static char application(unsigned int i, char c)
{
    return ((char)(c ^ (char)((i % 3) + 1)));
}

static void iteration(unsigned int i, char *c)
{
    *c = application(i, *c);
}

static void test_is_et_casse(void)
{
    int c;

    debut("ft_isalpha");
    for (c = -1; c <= 255; c++)
        verifier(ft_isalpha(c) == !!isalpha(c), "c=%d", c);
    fin();
    debut("ft_isdigit");
    for (c = -1; c <= 255; c++)
        verifier(ft_isdigit(c) == !!isdigit(c), "c=%d", c);
    fin();
    debut("ft_isalnum");
    for (c = -1; c <= 255; c++)
        verifier(ft_isalnum(c) == !!isalnum(c), "c=%d", c);
    fin();
    debut("ft_isascii");
    for (c = -300; c <= 400; c++)
        verifier(ft_isascii(c) == (c >= 0 && c <= 127), "c=%d", c);
    fin();
    debut("ft_isprint");
    for (c = -1; c <= 255; c++)
        verifier(ft_isprint(c) == !!isprint(c), "c=%d", c);
    fin();
    debut("ft_toupper");
    for (c = -1; c <= 255; c++)
        verifier(ft_toupper(c) == toupper(c), "c=%d", c);
    fin();
    debut("ft_tolower");
    for (c = -1; c <= 255; c++)
        verifier(ft_tolower(c) == tolower(c), "c=%d", c);
    fin();
}

static void test_strlen_et_dup(void)
{
    char s[CAP];
    char *copie;
    int i;

    debut("ft_strlen");
    for (i = 0; i < TOURS; i++)
    {
        texte_aleatoire(s, hasard(CAP), 1);
        verifier(ft_strlen(s) == strlen(s), "tour=%d", i);
    }
    fin();
    debut("ft_strdup");
    for (i = 0; i < TOURS; i++)
    {
        texte_aleatoire(s, hasard(CAP), 1);
        copie = ft_strdup(s);
        verifier(copie && strcmp(copie, s) == 0 && copie != s,
            "tour=%d, longueur=%zu", i, strlen(s));
        if (copie)
        {
            if (s[0])
            {
                char avant = copie[0];
                s[0] ^= 1;
                verifier(copie[0] == avant, "copie non independante, tour=%d", i);
            }
            free(copie);
        }
    }
    fin();
}

static void test_chr_et_ncmp(void)
{
    char s[CAP];
    char a[CAP];
    char b[CAP];
    size_t len;
    int i;
    int c;

    debut("ft_strchr");
    for (i = 0; i < TOURS; i++)
    {
        len = hasard(CAP);
        texte_aleatoire(s, len, 1);
        if (len > 1 && i % 3 == 0)
            s[hasard(len)] = (char)0xff;
        for (c = -1; c < 260; c++)
            verifier(ft_strchr(s, c) == strchr(s, c),
                "tour=%d, c=%d", i, c);
    }
    fin();
    debut("ft_strrchr");
    for (i = 0; i < TOURS; i++)
    {
        len = hasard(CAP);
        texte_aleatoire(s, len, 1);
        if (len > 1 && i % 3 == 0)
            s[hasard(len)] = (char)0xff;
        for (c = -1; c < 260; c++)
            verifier(ft_strrchr(s, c) == strrchr(s, c),
                "tour=%d, c=%d", i, c);
    }
    fin();
    debut("ft_strncmp");
    for (i = 0; i < TOURS * 5; i++)
    {
        texte_aleatoire(a, hasard(CAP), 1);
        strcpy(b, a);
        if (i % 3 == 0)
            texte_aleatoire(b, hasard(CAP), 0);
        if (i % 5 == 0 && b[0])
            b[hasard(strlen(b))] = (char)0xff;
        len = hasard(CAP + 2);
        verifier(signe(ft_strncmp(a, b, len)) == signe(strncmp(a, b, len)),
            "tour=%d, n=%zu, a='%.24s', b='%.24s'", i, len, a, b);
    }
    fin();
}

static void test_strnstr(void)
{
    char grand[CAP];
    char petit[CAP];
    int i;
    size_t limite;

    debut("ft_strnstr");
    for (i = 0; i < TOURS * 6; i++)
    {
        texte_aleatoire(grand, hasard(180), i & 1);
        texte_aleatoire(petit, hasard(30), i & 1);
        if (i % 4 == 0 && strlen(grand) > 4)
        {
            size_t debut_texte = hasard(strlen(grand) - 3);
            size_t taille = hasard(5);
            memcpy(petit, grand + debut_texte, taille);
            petit[taille] = '\0';
        }
        limite = hasard(200);
        verifier(ft_strnstr(grand, petit, limite)
            == cherche_limitee_ref(grand, petit, limite),
            "tour=%d, len=%zu, grand='%.40s', petit='%.20s'",
            i, limite, grand, petit);
    }
    strcpy(grand, "aaaaaaaab");
    verifier(ft_strnstr(grand, "aaaab", 9) == grand + 4,
        "repetitions longues");
    strcpy(grand, "abc");
    verifier(ft_strnstr(grand, "", 0) == grand,
        "chaine vide, limite nulle");
    fin();
}

static void test_strl(void)
{
    char a[CAP];
    char b[CAP];
    char src[CAP];
    char initial[CAP];
    size_t taille;
    size_t attendu;
    size_t recu;
    int i;

    debut("ft_strlcpy");
    for (i = 0; i < TOURS * 5; i++)
    {
        texte_aleatoire(src, hasard(180), 1);
        memset(a, 0xab, sizeof(a));
        memset(b, 0xab, sizeof(b));
        taille = hasard(200);
        attendu = copie_limitee_ref(a, src, taille);
        recu = ft_strlcpy(b, src, taille);
        verifier(recu == attendu && memcmp(a, b, CAP) == 0,
            "tour=%d, size=%zu, src_len=%zu, retour=%zu attendu=%zu",
            i, taille, strlen(src), recu, attendu);
    }
    fin();
    debut("ft_strlcat");
    for (i = 0; i < TOURS * 5; i++)
    {
        texte_aleatoire(src, hasard(120), 1);
        texte_aleatoire(initial, hasard(180), 1);
        memset(a, 0xab, sizeof(a));
        memset(b, 0xab, sizeof(b));
        memcpy(a, initial, strlen(initial) + 1);
        memcpy(b, initial, strlen(initial) + 1);
        taille = hasard(220);
        attendu = concat_limitee_ref(a, src, taille);
        recu = ft_strlcat(b, src, taille);
        verifier(recu == attendu && memcmp(a, b, CAP) == 0,
            "tour=%d, size=%zu, src_len=%zu, dst_len=%zu, retour=%zu attendu=%zu",
            i, taille, strlen(src), strlen(initial), recu, attendu);
    }
    fin();
}

static void test_memset_bzero(void)
{
    unsigned char initial[CAP];
    unsigned char a[CAP];
    unsigned char b[CAP];
    size_t debut_octets;
    size_t longueur;
    int i;
    int c;

    debut("ft_memset");
    for (i = 0; i < TOURS * 5; i++)
    {
        octets_aleatoires(initial, CAP);
        memcpy(a, initial, CAP);
        memcpy(b, initial, CAP);
        debut_octets = hasard(128);
        longueur = hasard(CAP - debut_octets + 1);
        c = (int)hasard(1025) - 512;
        verifier(ft_memset(b + debut_octets, c, longueur) == b + debut_octets,
            "retour, tour=%d", i);
        memset(a + debut_octets, c, longueur);
        verifier(memcmp(a, b, CAP) == 0,
            "tour=%d, debut=%zu, n=%zu, c=%d", i, debut_octets, longueur, c);
    }
    fin();
    debut("ft_bzero");
    for (i = 0; i < TOURS * 5; i++)
    {
        octets_aleatoires(initial, CAP);
        memcpy(a, initial, CAP);
        memcpy(b, initial, CAP);
        debut_octets = hasard(128);
        longueur = hasard(CAP - debut_octets + 1);
        memset(a + debut_octets, 0, longueur);
        ft_bzero(b + debut_octets, longueur);
        verifier(memcmp(a, b, CAP) == 0,
            "tour=%d, debut=%zu, n=%zu", i, debut_octets, longueur);
    }
    fin();
}

static void test_memcpy_move(void)
{
    unsigned char source[CAP];
    unsigned char a[CAP];
    unsigned char b[CAP];
    size_t source_debut;
    size_t dest_debut;
    size_t longueur;
    int i;

    debut("ft_memcpy");
    for (i = 0; i < TOURS * 5; i++)
    {
        octets_aleatoires(source, CAP);
        octets_aleatoires(a, CAP);
        memcpy(b, a, CAP);
        source_debut = hasard(CAP);
        dest_debut = hasard(CAP);
        longueur = hasard((CAP - source_debut < CAP - dest_debut
            ? CAP - source_debut : CAP - dest_debut) + 1);
        memcpy(a + dest_debut, source + source_debut, longueur);
        verifier(ft_memcpy(b + dest_debut, source + source_debut,
            longueur) == b + dest_debut, "retour, tour=%d", i);
        verifier(memcmp(a, b, CAP) == 0,
            "tour=%d, source=%zu, dst=%zu, n=%zu",
            i, source_debut, dest_debut, longueur);
    }
    fin();
    debut("ft_memmove");
    for (i = 0; i < TOURS * 10; i++)
    {
        octets_aleatoires(a, CAP);
        memcpy(b, a, CAP);
        source_debut = hasard(CAP);
        dest_debut = hasard(CAP);
        longueur = hasard((CAP - source_debut < CAP - dest_debut
            ? CAP - source_debut : CAP - dest_debut) + 1);
        memmove(a + dest_debut, a + source_debut, longueur);
        verifier(ft_memmove(b + dest_debut, b + source_debut,
            longueur) == b + dest_debut, "retour, tour=%d", i);
        verifier(memcmp(a, b, CAP) == 0,
            "tour=%d, src=%zu, dst=%zu, n=%zu",
            i, source_debut, dest_debut, longueur);
    }
    /* Deux allocations independantes : memmove doit aussi les gerer. */
    for (i = 0; i < 100; i++)
    {
        unsigned char *source = malloc(CAP);
        unsigned char *destination = malloc(CAP);
        if (!source || !destination)
        {
            free(source);
            free(destination);
            verifier(0, "impossible d'allouer les buffers de test");
            break ;
        }
        octets_aleatoires(source, CAP);
        memset(destination, 0x5a, CAP);
        verifier(ft_memmove(destination, source, CAP) == destination
            && memcmp(destination, source, CAP) == 0,
            "allocations distinctes, tour=%d", i);
        free(source);
        free(destination);
    }
    fin();
}

static void test_memchr_cmp(void)
{
    unsigned char a[CAP];
    unsigned char b[CAP];
    size_t longueur;
    int i;
    int c;

    debut("ft_memchr");
    for (i = 0; i < TOURS * 3; i++)
    {
        octets_aleatoires(a, CAP);
        longueur = hasard(CAP + 1);
        c = (int)hasard(1025) - 512;
        verifier(ft_memchr(a, c, longueur) == memchr(a, c, longueur),
            "tour=%d, n=%zu, c=%d", i, longueur, c);
        if (longueur > 0)
        {
            c = a[hasard(longueur)];
            verifier(ft_memchr(a, c, longueur) == memchr(a, c, longueur),
                "octet present, tour=%d, n=%zu, c=%d", i, longueur, c);
        }
    }
    fin();
    debut("ft_memcmp");
    for (i = 0; i < TOURS * 8; i++)
    {
        octets_aleatoires(a, CAP);
        memcpy(b, a, CAP);
        if (i % 4 == 0)
            octets_aleatoires(b, CAP);
        if (i % 4 == 1)
            b[hasard(CAP)] ^= 1;
        longueur = hasard(CAP + 1);
        verifier(signe(ft_memcmp(a, b, longueur))
            == signe(memcmp(a, b, longueur)),
            "tour=%d, n=%zu", i, longueur);
    }
    fin();
}

static void test_calloc(void)
{
    unsigned char *p;
    unsigned char *q;
    size_t quantite;
    size_t taille;
    size_t j;
    int i;
    int ok;

    debut("ft_calloc");
    for (i = 0; i < TOURS; i++)
    {
        quantite = hasard(64);
        taille = hasard(64);
        p = ft_calloc(quantite, taille);
        ok = (p != NULL);
        if (ok)
        {
            j = 0;
            while (j < quantite * taille)
                if (p[j++] != 0)
                    ok = 0;
        }
        verifier(ok, "tour=%d, nmemb=%zu, size=%zu", i, quantite, taille);
        free(p);
    }
    p = ft_calloc(0, 15);
    q = ft_calloc(0, 15);
    verifier(p && q && p != q, "calloc(0,15): pointeurs distincts et liberables");
    free(p);
    free(q);
    p = ft_calloc(12, 0);
    verifier(p != NULL, "calloc(12,0) doit retourner un pointeur liberable");
    free(p);
    p = ft_calloc((size_t)-1, 2);
    verifier(p == NULL, "debordement SIZE_MAX * 2");
    free(p);
    fin();
}

static void test_atoi_itoa(void)
{
    char s[64];
    char attendu[64];
    const char *cas[] = {"", "0", "-0", "   +0042x", "\t\n -2147483648",
        "2147483647", "---123", "+-12", "  12 34", "\v\f+98end", "000099", "-42"};
    int nombres[] = {0, 1, -1, INT_MIN, INT_MAX, 42, -42, 1000000000};
    char *r;
    int i;
    int v;

    debut("ft_atoi");
    for (i = 0; i < (int)(sizeof(cas) / sizeof(cas[0])); i++)
        verifier(ft_atoi(cas[i]) == atoi(cas[i]), "s='%s'", cas[i]);
    for (i = 0; i < TOURS * 3; i++)
    {
        v = (int)aleatoire();
        snprintf(s, sizeof(s), "  %drest", v);
        verifier(ft_atoi(s) == atoi(s), "tour=%d, s='%s'", i, s);
    }
    fin();
    debut("ft_itoa");
    for (i = 0; i < (int)(sizeof(nombres) / sizeof(nombres[0])); i++)
    {
        snprintf(attendu, sizeof(attendu), "%d", nombres[i]);
        r = ft_itoa(nombres[i]);
        verifier(r && strcmp(r, attendu) == 0, "n=%d", nombres[i]);
        free(r);
    }
    for (i = 0; i < TOURS * 3; i++)
    {
        v = (int)aleatoire();
        snprintf(attendu, sizeof(attendu), "%d", v);
        r = ft_itoa(v);
        verifier(r && strcmp(r, attendu) == 0, "tour=%d, n=%d", i, v);
        free(r);
    }
    fin();
}

static void test_substr_join_trim(void)
{
    char a[CAP];
    char b[CAP];
    char ref[CAP * 2];
    char *res;
    char *attendu;
    const char *ensembles[] = {"", "abc", "XyZ", " ", "_-", "abcXyZ 001_-"};
    size_t pos;
    size_t nombre;
    size_t longueur;
    int i;

    debut("ft_substr");
    for (i = 0; i < TOURS * 4; i++)
    {
        texte_aleatoire(a, hasard(CAP), 1);
        pos = hasard(CAP + 90);
        nombre = hasard(CAP + 90);
        longueur = strlen(a);
        if (pos > longueur)
            pos = longueur;
        if (nombre > longueur - pos)
            nombre = longueur - pos;
        memcpy(ref, a + pos, nombre);
        ref[nombre] = '\0';
        res = ft_substr(a, (unsigned int)pos, nombre);
        verifier(res && strcmp(res, ref) == 0,
            "tour=%d, start=%zu, len=%zu", i, pos, nombre);
        free(res);
    }
    res = ft_substr("abc", UINT_MAX, SIZE_MAX);
    verifier(res && strcmp(res, "") == 0, "start=UINT_MAX, len=SIZE_MAX");
    free(res);
    fin();
    debut("ft_strjoin");
    for (i = 0; i < TOURS * 4; i++)
    {
        texte_aleatoire(a, hasard(CAP), 1);
        texte_aleatoire(b, hasard(CAP), 1);
        strcpy(ref, a);
        strcat(ref, b);
        res = ft_strjoin(a, b);
        verifier(res && strcmp(res, ref) == 0,
            "tour=%d, longueur1=%zu, longueur2=%zu",
            i, strlen(a), strlen(b));
        free(res);
    }
    fin();
    debut("ft_strtrim");
    for (i = 0; i < TOURS * 4; i++)
    {
        texte_aleatoire(a, hasard(CAP), 1);
        attendu = elaguer_ref(a, ensembles[i % 6]);
        res = ft_strtrim(a, ensembles[i % 6]);
        verifier(res && attendu && strcmp(res, attendu) == 0,
            "tour=%d, chaine='%.30s', set='%s'", i, a, ensembles[i % 6]);
        free(attendu);
        free(res);
    }
    res = ft_strtrim("aaaa", "a");
    verifier(res && strcmp(res, "") == 0, "chaine entierement supprimee");
    free(res);
    fin();
}

static void test_split(void)
{
    char s[CAP];
    char **a;
    char **b;
    char sep;
    int i;
    const char *cas[] = {"", "a", "abc", ",", ",,", ",a,b,", "a,,b",
        "   un  deux  trois ", "aaaa", ",a,,b,,,c,", "a,b,c", "a,,"};

    debut("ft_split");
    for (i = 0; i < (int)(sizeof(cas) / sizeof(cas[0])); i++)
    {
        a = ft_split(cas[i], ',');
        b = decouper_ref(cas[i], ',');
        verifier(memes_mots(a, b), "cas fixe %d, texte='%s'", i, cas[i]);
        liberer_mots(a);
        liberer_mots(b);
    }
    for (i = 0; i < TOURS * 6; i++)
    {
        texte_aleatoire(s, hasard(220), 1);
        sep = (i % 7 == 0) ? '\0' : "a 0,_-X"[i % 7];
        a = ft_split(s, sep);
        b = decouper_ref(s, sep);
        verifier(memes_mots(a, b),
            "tour=%d, separateur=%d, chaine='%.60s'", i, (int)sep, s);
        liberer_mots(a);
        liberer_mots(b);
    }
    fin();
}

static void test_mapi_iteri(void)
{
    char s[CAP];
    char attendu[CAP];
    char *r;
    size_t j;
    int i;

    debut("ft_strmapi");
    for (i = 0; i < TOURS * 3; i++)
    {
        texte_aleatoire(s, hasard(CAP), 1);
        j = 0;
        while (s[j])
        {
            attendu[j] = application((unsigned int)j, s[j]);
            j++;
        }
        attendu[j] = '\0';
        r = ft_strmapi(s, application);
        verifier(r && strcmp(r, attendu) == 0,
            "tour=%d, taille=%zu", i, strlen(s));
        free(r);
    }
    fin();
    debut("ft_striteri");
    for (i = 0; i < TOURS * 3; i++)
    {
        texte_aleatoire(s, hasard(CAP), 1);
        strcpy(attendu, s);
        j = 0;
        while (attendu[j])
        {
            attendu[j] = application((unsigned int)j, attendu[j]);
            j++;
        }
        ft_striteri(s, iteration);
        verifier(strcmp(s, attendu) == 0, "tour=%d", i);
    }
    fin();
}

static ssize_t capturer(int mode, const char *s, int nombre, char *sortie)
{
    int descripteurs[2];
    ssize_t n;

    if (pipe(descripteurs) == -1)
        return (-1);
    if (mode == 0)
        ft_putchar_fd((char)nombre, descripteurs[1]);
    if (mode == 1)
        ft_putstr_fd((char *)s, descripteurs[1]);
    if (mode == 2)
        ft_putendl_fd((char *)s, descripteurs[1]);
    if (mode == 3)
        ft_putnbr_fd(nombre, descripteurs[1]);
    close(descripteurs[1]);
    n = read(descripteurs[0], sortie, CAP - 1);
    close(descripteurs[0]);
    if (n >= 0)
        sortie[n] = '\0';
    return (n);
}

static void test_putfd(void)
{
    char obtenu[CAP];
    char attendu[CAP];
    int valeurs[] = {0, 1, -1, INT_MIN, INT_MAX, 100, -100, 42, -42};
    int i;
    ssize_t n;

    debut("ft_putchar_fd");
    for (i = 0; i < 256; i++)
    {
        n = capturer(0, "", i, obtenu);
        verifier(n == 1 && (unsigned char)obtenu[0] == (unsigned char)i,
            "octet=%d", i);
    }
    fin();
    debut("ft_putstr_fd");
    for (i = 0; i < TOURS; i++)
    {
        texte_aleatoire(attendu, hasard(170), 1);
        n = capturer(1, attendu, 0, obtenu);
        verifier(n == (ssize_t)strlen(attendu)
            && memcmp(obtenu, attendu, strlen(attendu)) == 0,
            "tour=%d", i);
    }
    fin();
    debut("ft_putendl_fd");
    for (i = 0; i < TOURS; i++)
    {
        texte_aleatoire(attendu, hasard(170), 1);
        n = capturer(2, attendu, 0, obtenu);
        strcat(attendu, "\n");
        verifier(n == (ssize_t)strlen(attendu)
            && strcmp(obtenu, attendu) == 0, "tour=%d", i);
    }
    fin();
    debut("ft_putnbr_fd");
    for (i = 0; i < TOURS * 2; i++)
    {
        int v = i < (int)(sizeof(valeurs) / sizeof(valeurs[0]))
            ? valeurs[i] : (int)aleatoire();
        snprintf(attendu, sizeof(attendu), "%d", v);
        n = capturer(3, "", v, obtenu);
        verifier(n == (ssize_t)strlen(attendu)
            && strcmp(obtenu, attendu) == 0, "tour=%d, n=%d", i, v);
    }
    fin();
}

static int *nouvel_entier(int n)
{
    int *p;

    p = malloc(sizeof(*p));
    if (p)
        *p = n;
    return (p);
}

static void detruire_entier(void *p)
{
    g_destructions++;
    free(p);
}

static void doubler_entier(void *p)
{
    *(int *)p *= 2;
}

static void *mapper_entier(void *p)
{
    return (nouvel_entier(*(int *)p + 7));
}

static void test_lstnew(void)
{
    int valeur = 42;
    t_list *n;

    debut("ft_lstnew");
    n = ft_lstnew(&valeur);
    verifier(n && n->content == &valeur && n->next == NULL,
        "initialisation contenu/next");
    free(n);
    n = ft_lstnew(NULL);
    verifier(n && n->content == NULL && n->next == NULL,
        "contenu NULL autorise");
    free(n);
    fin();
}

static void test_lstadd_front_back(void)
{
    int a = 1;
    int b = 2;
    t_list *x;
    t_list *y;
    t_list *liste;

    debut("ft_lstadd_front");
    x = ft_lstnew(&a);
    y = ft_lstnew(&b);
    liste = x;
    if (x && y)
    {
        ft_lstadd_front(&liste, y);
        verifier(liste == y && y->next == x, "ajout devant liste non vide");
    }
    free(y);
    free(x);
    fin();
    debut("ft_lstadd_back");
    x = ft_lstnew(&a);
    y = ft_lstnew(&b);
    liste = x;
    if (x && y)
    {
        ft_lstadd_back(&liste, y);
        verifier(liste == x && x->next == y && y->next == NULL,
            "ajout derriere liste non vide");
    }
    free(y);
    free(x);
    fin();
}

static void test_lstsize_last(void)
{
    t_list *a;
    t_list *b;
    t_list *c;

    a = ft_lstnew(NULL);
    b = ft_lstnew(NULL);
    c = ft_lstnew(NULL);
    if (!a || !b || !c)
    {
        free(a);
        free(b);
        free(c);
        return ;
    }
    a->next = b;
    b->next = c;
    debut("ft_lstsize");
    verifier(ft_lstsize(NULL) == 0, "liste vide");
    verifier(ft_lstsize(a) == 3, "trois noeuds");
    verifier(ft_lstsize(c) == 1, "un noeud");
    fin();
    debut("ft_lstlast");
    verifier(ft_lstlast(NULL) == NULL, "liste vide");
    verifier(ft_lstlast(a) == c, "dernier de trois");
    verifier(ft_lstlast(c) == c, "dernier de un");
    fin();
    free(a);
    free(b);
    free(c);
}

static void test_lstdelone_clear(void)
{
    t_list *a;
    t_list *b;
    t_list *c;
    t_list *liste;

    debut("ft_lstdelone");
    g_destructions = 0;
    a = ft_lstnew(nouvel_entier(1));
    b = ft_lstnew(nouvel_entier(2));
    if (a && b)
    {
        a->next = b;
        ft_lstdelone(a, detruire_entier);
        verifier(g_destructions == 1 && *(int *)b->content == 2,
            "ne detruit pas le suivant");
        ft_lstdelone(b, detruire_entier);
    }
    fin();
    debut("ft_lstclear");
    g_destructions = 0;
    a = ft_lstnew(nouvel_entier(1));
    b = ft_lstnew(nouvel_entier(2));
    c = ft_lstnew(nouvel_entier(3));
    liste = a;
    if (a && b && c)
    {
        a->next = b;
        b->next = c;
        ft_lstclear(&liste, detruire_entier);
        verifier(liste == NULL && g_destructions == 3,
            "trois destructions et pointeur mis a NULL");
        ft_lstclear(&liste, detruire_entier);
        verifier(liste == NULL && g_destructions == 3,
            "deuxieme clear sur liste vide");
    }
    fin();
}

static void test_lstiter_map(void)
{
    t_list *a;
    t_list *b;
    t_list *c;
    t_list *nouvelle;
    int v1;
    int v2;
    int v3;

    v1 = 3;
    v2 = -5;
    v3 = 7;
    a = ft_lstnew(&v1);
    b = ft_lstnew(&v2);
    c = ft_lstnew(&v3);
    if (!a || !b || !c)
    {
        free(a);
        free(b);
        free(c);
        return ;
    }
    a->next = b;
    b->next = c;
    debut("ft_lstiter");
    ft_lstiter(a, doubler_entier);
    verifier(v1 == 6 && v2 == -10 && v3 == 14,
        "application aux trois valeurs");
    fin();
    debut("ft_lstmap");
    nouvelle = ft_lstmap(a, mapper_entier, detruire_entier);
    verifier(nouvelle && ft_lstsize(nouvelle) == 3,
        "nombre de noeuds de la copie");
    if (nouvelle && nouvelle->next && nouvelle->next->next)
        verifier(*(int *)nouvelle->content == 13
            && *(int *)nouvelle->next->content == -3
            && *(int *)nouvelle->next->next->content == 21
            && nouvelle != a && nouvelle->content != a->content,
            "valeurs mappees, allocations independantes");
    g_destructions = 0;
    ft_lstclear(&nouvelle, detruire_entier);
    verifier(nouvelle == NULL && g_destructions == 3,
        "liberation de la liste mappee");
    verifier(v1 == 6 && v2 == -10 && v3 == 14,
        "liste originale inchangee");
    free(a);
    free(b);
    free(c);
    fin();
}


/* Cas de controles : la chaine C s'arrete sur '\0', pas sur '\n'. */
static void test_controles_chaines(void)
{
    const char *cas[] = {"", "\n", "\r", "\t", "\r\n", "\n\n",
        "a\nb", "\na", "a\n", "a\r\nb", "\t\n\v\f\r", "\nA\nB\n",
        "abc\nabc", "  \n  ", "a\t\rb\n", "\n\0SUITE"};
    const char *motifs[] = {"", "\n", "\r", "\t", "a\nb", "\r\n",
        "B\n", "abc", "\n\n", "\nA", "a\r\n", "\v\f"};
    char tronquee[] = {'a', '\0', '\n', 'b', '\0'};
    char attente[CAP];
    char obtenu[CAP];
    char *copie;
    size_t i;
    size_t j;
    size_t n;
    int c;

    debut("ft_strlen (retours ligne/NUL)");
    for (i = 0; i < sizeof(cas) / sizeof(cas[0]); i++)
        verifier(ft_strlen(cas[i]) == strlen(cas[i]), "cas=%zu", i);
    verifier(ft_strlen(tronquee) == 1, "NUL avant newline");
    fin();
    debut("ft_strdup (retours ligne/NUL)");
    for (i = 0; i < sizeof(cas) / sizeof(cas[0]); i++)
    {
        copie = ft_strdup(cas[i]);
        verifier(copie != NULL && copie != cas[i]
            && strcmp(copie, cas[i]) == 0, "cas=%zu", i);
        free(copie);
    }
    fin();
    debut("ft_strchr/ft_strrchr (retours ligne/NUL)");
    for (i = 0; i < sizeof(cas) / sizeof(cas[0]); i++)
    {
        for (c = 0; c <= 255; c++)
        {
            verifier(ft_strchr(cas[i], c) == strchr(cas[i], c),
                "strchr cas=%zu, c=%d", i, c);
            verifier(ft_strrchr(cas[i], c) == strrchr(cas[i], c),
                "strrchr cas=%zu, c=%d", i, c);
        }
    }
    verifier(ft_strchr(tronquee, '\n') == NULL,
        "strchr doit s'arreter sur NUL");
    verifier(ft_strrchr(tronquee, '\n') == NULL,
        "strrchr doit s'arreter sur NUL");
    fin();
    debut("ft_strncmp (retours ligne/NUL)");
    for (i = 0; i < sizeof(cas) / sizeof(cas[0]); i++)
        for (j = 0; j < sizeof(cas) / sizeof(cas[0]); j++)
            for (n = 0; n <= 20; n++)
                verifier(signe(ft_strncmp(cas[i], cas[j], n))
                    == signe(strncmp(cas[i], cas[j], n)),
                    "a=%zu b=%zu n=%zu", i, j, n);
    fin();
    debut("ft_strnstr (retours ligne/NUL)");
    for (i = 0; i < sizeof(cas) / sizeof(cas[0]); i++)
        for (j = 0; j < sizeof(motifs) / sizeof(motifs[0]); j++)
            for (n = 0; n <= 20; n++)
                verifier(ft_strnstr(cas[i], motifs[j], n)
                    == cherche_limitee_ref(cas[i], motifs[j], n),
                    "texte=%zu motif=%zu len=%zu", i, j, n);
    {
        const char *phrase = "ab\ncd\nxy";
        verifier(ft_strnstr(phrase, "\ncd", 5) == phrase + 2,
            "LF + motif present dans la limite");
        verifier(ft_strnstr(phrase, "\ncd", 4) == NULL,
            "motif coupe par la limite");
        verifier(ft_strnstr(phrase, "\n", 2) == NULL,
            "LF juste hors de la limite");
        verifier(ft_strnstr(phrase, "\n", 3) == phrase + 2,
            "LF a la derniere position autorisee");
    }
    fin();
    debut("ft_strlcpy (retours ligne/NUL)");
    for (i = 0; i < sizeof(cas) / sizeof(cas[0]); i++)
        for (n = 0; n <= 20; n++)
        {
            memset(attente, 0xa5, CAP);
            memset(obtenu, 0xa5, CAP);
            j = copie_limitee_ref(attente, cas[i], n);
            verifier(ft_strlcpy(obtenu, cas[i], n) == j
                && memcmp(attente, obtenu, CAP) == 0,
                "cas=%zu size=%zu", i, n);
        }
    fin();
    debut("ft_strlcat (retours ligne/NUL)");
    for (i = 0; i < sizeof(cas) / sizeof(cas[0]); i++)
        for (j = 0; j < sizeof(cas) / sizeof(cas[0]); j++)
            for (n = 0; n <= 20; n++)
            {
                memset(attente, 0xa5, CAP);
                memset(obtenu, 0xa5, CAP);
                memcpy(attente, cas[i], strlen(cas[i]) + 1);
                memcpy(obtenu, cas[i], strlen(cas[i]) + 1);
                {
                    size_t retour = concat_limitee_ref(attente, cas[j], n);
                    verifier(ft_strlcat(obtenu, cas[j], n) == retour
                        && memcmp(attente, obtenu, CAP) == 0,
                        "dst=%zu src=%zu size=%zu", i, j, n);
                }
            }
    fin();
}

static void test_controles_memoire(void)
{
    unsigned char tampon[] = {'A', '\n', '\0', '\r', '\t', 'Z', '\n', 255};
    unsigned char attendu[32];
    unsigned char obtenu[32];
    size_t i;
    size_t j;
    int c;

    debut("ft_memchr (LF apres NUL)");
    for (i = 0; i <= sizeof(tampon); i++)
        for (c = -256; c <= 511; c++)
            verifier(ft_memchr(tampon, c, i) == memchr(tampon, c, i),
                "n=%zu c=%d", i, c);
    verifier(ft_memchr(tampon, '\n', 8) == tampon + 1,
        "premier LF (offset 1)");
    verifier(ft_memchr(tampon + 2, '\n', 6) == tampon + 6,
        "LF apres NUL (offset 6)");
    fin();
    debut("ft_memcmp (LF/NUL/CR)");
    for (i = 0; i < sizeof(tampon); i++)
        for (j = 0; j <= sizeof(tampon); j++)
        {
            memcpy(attendu, tampon, sizeof(tampon));
            memcpy(obtenu, tampon, sizeof(tampon));
            obtenu[i] = (unsigned char)(obtenu[i] ^ 0x0a);
            verifier(signe(ft_memcmp(attendu, obtenu, j))
                == signe(memcmp(attendu, obtenu, j)),
                "modifie=%zu n=%zu", i, j);
        }
    fin();
    debut("ft_memcpy (LF/NUL/CR)");
    for (i = 0; i <= sizeof(tampon); i++)
    {
        memset(attendu, 0x5c, sizeof(attendu));
        memset(obtenu, 0x5c, sizeof(obtenu));
        memcpy(attendu + 3, tampon, i);
        verifier(ft_memcpy(obtenu + 3, tampon, i) == obtenu + 3
            && memcmp(attendu, obtenu, sizeof(attendu)) == 0,
            "n=%zu", i);
    }
    fin();
    debut("ft_memmove (LF/NUL/CR, recouvrement)");
    for (i = 0; i <= sizeof(tampon); i++)
    {
        memset(attendu, 0x5c, sizeof(attendu));
        memcpy(attendu + 5, tampon, sizeof(tampon));
        memcpy(obtenu, attendu, sizeof(attendu));
        memmove(attendu + 7, attendu + 5, i);
        verifier(ft_memmove(obtenu + 7, obtenu + 5, i) == obtenu + 7
            && memcmp(attendu, obtenu, sizeof(attendu)) == 0,
            "deplacement droite n=%zu", i);
        memmove(attendu + 3, attendu + 5, i);
        verifier(ft_memmove(obtenu + 3, obtenu + 5, i) == obtenu + 3
            && memcmp(attendu, obtenu, sizeof(attendu)) == 0,
            "deplacement gauche n=%zu", i);
    }
    fin();
    debut("ft_memset/ft_bzero (LF/NUL/CR)");
    for (i = 0; i <= sizeof(tampon); i++)
    {
        memset(attendu, '\n', sizeof(attendu));
        memset(obtenu, '\n', sizeof(obtenu));
        memset(attendu + 3, '\r', i);
        verifier(ft_memset(obtenu + 3, '\r', i) == obtenu + 3
            && memcmp(attendu, obtenu, sizeof(attendu)) == 0,
            "memset i=%zu", i);
        memset(attendu + 3, '\0', i);
        ft_bzero(obtenu + 3, i);
        verifier(memcmp(attendu, obtenu, sizeof(attendu)) == 0,
            "bzero i=%zu", i);
    }
    fin();
}

static void test_controles_supplementaires(void)
{
    const char *texte = "\nABC\nDEF\r\n";
    const char *entrees[] = {"\n42", "  \r\n\t-17xyz", "12\n34",
        "\n\t+00023", "\n-0", "\r\n  2147483647"};
    char *res;
    char *attendu;
    char s[64];
    char ref[64];
    char **a;
    char **b;
    size_t i;
    size_t j;

    debut("ft_atoi (espaces LF/CR/TAB)");
    for (i = 0; i < sizeof(entrees) / sizeof(entrees[0]); i++)
        verifier(ft_atoi(entrees[i]) == atoi(entrees[i]),
            "chaine numero=%zu", i);
    fin();
    debut("ft_substr (multilignes)");
    for (i = 0; i <= strlen(texte) + 2; i++)
        for (j = 0; j <= 15; j++)
        {
            size_t taille = strlen(texte);
            size_t debut_s = i < taille ? i : taille;
            size_t longueur = j < taille - debut_s ? j : taille - debut_s;
            memcpy(ref, texte + debut_s, longueur);
            ref[longueur] = '\0';
            res = ft_substr(texte, (unsigned int)i, j);
            verifier(res && strcmp(res, ref) == 0,
                "start=%zu len=%zu", i, j);
            free(res);
        }
    fin();
    debut("ft_strjoin (multilignes)");
    res = ft_strjoin("Bonjour\n", "\nMonde\r\n");
    verifier(res && strcmp(res, "Bonjour\n\nMonde\r\n") == 0,
        "jonction de deux chaines multilignes");
    free(res);
    fin();
    debut("ft_strtrim (LF dans ensemble)");
    res = ft_strtrim("\n\r\t Bonjour \n\r", "\n\r\t ");
    verifier(res && strcmp(res, "Bonjour") == 0,
        "supprime LF, CR, TAB et espaces aux extremites");
    free(res);
    res = ft_strtrim("\n\r\t", "\n\r\t");
    verifier(res && strcmp(res, "") == 0,
        "entierement constituee de separateurs");
    free(res);
    res = ft_strtrim("AA\nBB\nCC", "\n");
    verifier(res && strcmp(res, "AA\nBB\nCC") == 0,
        "conserve les LF interieurs");
    free(res);
    fin();
    debut("ft_split (LF comme separateur)");
    a = ft_split("\nalpha\n\nbeta\ngamma\n", '\n');
    b = decouper_ref("\nalpha\n\nbeta\ngamma\n", '\n');
    verifier(memes_mots(a, b), "separateur = LF");
    liberer_mots(a);
    liberer_mots(b);
    a = ft_split("alpha\nbeta\n", ' ');
    b = decouper_ref("alpha\nbeta\n", ' ');
    verifier(memes_mots(a, b), "LF non separateur conserve");
    liberer_mots(a);
    liberer_mots(b);
    fin();
    debut("ft_strmapi / ft_striteri (LF)");
    attendu = malloc(strlen(texte) + 1);
    if (attendu)
    {
        for (i = 0; i < strlen(texte); i++)
            attendu[i] = application((unsigned int)i, texte[i]);
        attendu[i] = '\0';
        res = ft_strmapi(texte, application);
        verifier(res && strcmp(res, attendu) == 0,
            "transformation des caracteres speciaux (mapi)");
        free(res);
        strcpy(s, texte);
        ft_striteri(s, iteration);
        verifier(strcmp(s, attendu) == 0,
            "transformation des caracteres speciaux (iteri)");
        free(attendu);
    }
    else
        verifier(0, "echec allocation buffer de reference");
    fin();
}

static void test_controles_sorties(void)
{
    char sortie[CAP];
    const char *texte = "debut\n\r\tmilieu\nfin\n";
    ssize_t n;

    debut("ft_putchar_fd (LF/CR/TAB/NUL)");
    n = capturer(0, "", '\n', sortie);
    verifier(n == 1 && sortie[0] == '\n', "LF");
    n = capturer(0, "", '\r', sortie);
    verifier(n == 1 && sortie[0] == '\r', "CR");
    n = capturer(0, "", '\t', sortie);
    verifier(n == 1 && sortie[0] == '\t', "TAB");
    n = capturer(0, "", '\0', sortie);
    verifier(n == 1 && sortie[0] == '\0', "NUL (octet transmis)");
    fin();
    debut("ft_putstr_fd (LF multiples)");
    n = capturer(1, texte, 0, sortie);
    verifier(n == (ssize_t)strlen(texte)
        && memcmp(texte, sortie, strlen(texte)) == 0,
        "respect de tous les caracteres");
    fin();
    debut("ft_putendl_fd (LF multiples + LF final)");
    n = capturer(2, texte, 0, sortie);
    verifier(n == (ssize_t)strlen(texte) + 1
        && memcmp(texte, sortie, strlen(texte)) == 0
        && sortie[strlen(texte)] == '\n',
        "conserve LF existants et ajoute LF final");
    fin();
}


/*
** Tests FD renforces : lecture brute depuis des pipes (pas de strcmp pour NUL).
** On controle l'octet exact, la longueur, l'absence d'octets supplementaires,
** la cible fd 0/1/2, un fd eleve, deux fd independants et plusieurs appels.
** Aucune entree a comportement indefini n'est volontairement testee.
*/
#define FD_CAPACITE 4096

static void fd_appeler(int type, const char *texte, int valeur, int fd)
{
    if (type == 0)
        ft_putchar_fd((char)valeur, fd);
    else if (type == 1)
        ft_putstr_fd((char *)texte, fd);
    else if (type == 2)
        ft_putendl_fd((char *)texte, fd);
    else
        ft_putnbr_fd(valeur, fd);
}

/* Lit jusqu'a EOF afin de detecter aussi des octets surnumeraires. */
static ssize_t fd_lire_total(int fd, unsigned char *sortie, size_t capacite)
{
    unsigned char tampon[256];
    ssize_t lus;
    size_t total;
    size_t i;
    int depassement;

    total = 0;
    depassement = 0;
    while ((lus = read(fd, tampon, sizeof(tampon))) > 0)
    {
        i = 0;
        while (i < (size_t)lus)
        {
            if (total < capacite)
                sortie[total] = tampon[i];
            else
                depassement = 1;
            total++;
            i++;
        }
    }
    if (lus < 0 || depassement)
        return (-1);
    return ((ssize_t)total);
}

/*
** cible = -1 : fd ordinaire ou duplique au-dessus de 100 (haut=1)
** cible = 0/1/2 : redirection temporaire de stdin/stdout/stderr
*/
static ssize_t fd_capturer_exact(int type, const char *texte, int valeur,
    int cible, int haut, unsigned char *sortie, size_t capacite)
{
    int tube[2];
    int original;
    int fd;
    ssize_t resultat;

    if (pipe(tube) != 0)
        return (-1);
    original = -1;
    fd = tube[1];
    if (cible >= 0)
    {
        fflush(stdout);
        fflush(stderr);
        original = dup(cible);
        if (original < 0 || dup2(tube[1], cible) < 0)
        {
            if (original >= 0)
                close(original);
            close(tube[0]);
            close(tube[1]);
            return (-1);
        }
        fd = cible;
        if (tube[1] != cible)
            close(tube[1]);
    }
    else if (haut)
    {
        fd = fcntl(tube[1], F_DUPFD, 100);
        if (fd < 0)
        {
            close(tube[0]);
            close(tube[1]);
            return (-1);
        }
        close(tube[1]);
    }
    fd_appeler(type, texte, valeur, fd);
    if (cible >= 0)
    {
        if (dup2(original, cible) < 0)
        {
            close(original);
            close(tube[0]);
            return (-1);
        }
        close(original);
    }
    else
        close(fd);
    resultat = fd_lire_total(tube[0], sortie, capacite);
    close(tube[0]);
    return (resultat);
}

static void fd_tester_chaine(int type, const char *texte, const char *nom,
    int cible, int haut)
{
    unsigned char sortie[FD_CAPACITE];
    size_t taille;
    ssize_t obtenu;
    int correct;

    taille = strlen(texte);
    obtenu = fd_capturer_exact(type, texte, 0, cible, haut, sortie,
        sizeof(sortie));
    correct = (obtenu == (ssize_t)(taille + (type == 2)));
    if (correct)
        correct = (memcmp(sortie, texte, taille) == 0);
    if (correct && type == 2)
        correct = (sortie[taille] == '\n');
    verifier(correct, "%s, type=%d, taille=%zu, fd=%d, obtenu=%zd",
        nom, type, taille, cible, obtenu);
}

static void fd_tester_nombre(int valeur, const char *nom, int cible,
    int haut)
{
    unsigned char sortie[FD_CAPACITE];
    char attendu[64];
    ssize_t obtenu;
    size_t taille;

    snprintf(attendu, sizeof(attendu), "%d", valeur);
    taille = strlen(attendu);
    obtenu = fd_capturer_exact(3, "", valeur, cible, haut, sortie,
        sizeof(sortie));
    verifier(obtenu == (ssize_t)taille && memcmp(sortie, attendu, taille) == 0,
        "%s, valeur=%d, fd=%d, obtenu=%zd", nom, valeur, cible, obtenu);
}

static void test_fd_cas_fixes(void)
{
    static const char *const chaines[] = {
        "", "a", "\\n", "\\r", "\\t", "\\0", "\n", "\r", "\t",
        "\v", "\f", "\n\n", "\r\n", "\n\r", "A\nB", "\nA", "A\n",
        "A\n\nB\n", "\t A \r B\n", "A\0XYZ", "\0ABC", "\x80\xff",
        "\xc3\xa9\n\xc3\xa7", "0123456789", "--42", "+0001"
    };
    static const int nombres[] = {
        INT_MIN, INT_MIN + 1, -1000000000, -123456789, -100, -42,
        -10, -9, -1, 0, 1, 9, 10, 42, 100, 123456789,
        1000000000, INT_MAX - 1, INT_MAX
    };
    size_t i;

    debut("ft_putchar_fd (256 octets, FD bas/haut)");
    for (i = 0; i < 256; i++)
    {
        unsigned char sortie[FD_CAPACITE];
        ssize_t n = fd_capturer_exact(0, "", (int)i, -1, 0,
            sortie, sizeof(sortie));
        verifier(n == 1 && sortie[0] == (unsigned char)i,
            "octet brut=%zu, recu=%zd", i, n);
        if (i % 16 == 0)
        {
            n = fd_capturer_exact(0, "", (int)i, -1, 1,
                sortie, sizeof(sortie));
            verifier(n == 1 && sortie[0] == (unsigned char)i,
                "octet brut=%zu, FD>=100", i);
        }
    }
    fin();

    debut("ft_putstr_fd (vides, LF/CR, NUL, octets hauts)");
    for (i = 0; i < sizeof(chaines) / sizeof(chaines[0]); i++)
        fd_tester_chaine(1, chaines[i], "chaine fixe", -1, (int)(i % 2));
    fin();

    debut("ft_putendl_fd (un seul LF ajoute)");
    for (i = 0; i < sizeof(chaines) / sizeof(chaines[0]); i++)
        fd_tester_chaine(2, chaines[i], "chaine fixe", -1, (int)(i % 2));
    fin();

    debut("ft_putnbr_fd (extremes, signe, zero)");
    for (i = 0; i < sizeof(nombres) / sizeof(nombres[0]); i++)
        fd_tester_nombre(nombres[i], "entier fixe", -1, (int)(i % 2));
    fin();
}

static void test_fd_longueurs_et_aleatoires(void)
{
    static const size_t tailles[] = {
        0, 1, 2, 3, 7, 8, 15, 16, 31, 32, 63, 64, 127, 128,
        254, 255, 256, 257, 511, 512, 1023, 2047
    };
    char texte[2048];
    size_t i;
    size_t j;
    int k;

    debut("ft_putstr_fd / ft_putendl_fd (tailles frontieres)");
    for (i = 0; i < sizeof(tailles) / sizeof(tailles[0]); i++)
    {
        j = 0;
        while (j < tailles[i])
        {
            texte[j] = (j % 17 == 0) ? '\n' : (j % 19 == 0) ? '\r' :
                (j % 23 == 0) ? '\t' : (char)('a' + j % 26);
            j++;
        }
        texte[j] = '\0';
        fd_tester_chaine(1, texte, "taille frontiere", -1, 0);
        fd_tester_chaine(2, texte, "taille frontiere", -1, 1);
    }
    fin();

    debut("ft_putstr_fd / ft_putendl_fd (aleatoires binaires)");
    for (k = 0; k < TOURS; k++)
    {
        size_t taille = hasard(1024);
        j = 0;
        while (j < taille)
        {
            /* LF, CR, TAB, octets >= 128 et parfois NUL interieur */
            texte[j] = (char)(aleatoire() >> 24);
            j++;
        }
        texte[taille] = '\0';
        fd_tester_chaine(1, texte, "chaine aleatoire", -1, 0);
        fd_tester_chaine(2, texte, "chaine aleatoire", -1, 0);
    }
    fin();

    debut("ft_putnbr_fd (entiers aleatoires)");
    for (k = 0; k < TOURS * 2; k++)
        fd_tester_nombre((int)aleatoire(), "entier aleatoire", -1, 0);
    fin();
}

static void test_fd_descripteurs_speciaux(void)
{
    const char *texte = "A\n\r\tB\n";
    unsigned char sortie[FD_CAPACITE];
    ssize_t n;
    int fd;

    debut("fd=0 / fd=1 / fd=2 (redirection reelle)");
    for (fd = 0; fd <= 2; fd++)
    {
        fd_tester_chaine(1, texte, "sortie brute", fd, 0);
        fd_tester_chaine(2, texte, "sortie avec LF", fd, 0);
        fd_tester_chaine(1, "", "sortie vide", fd, 0);
        fd_tester_nombre(INT_MIN, "entier INT_MIN", fd, 0);
        fd_tester_nombre(0, "entier zero", fd, 0);
        n = fd_capturer_exact(0, "", 0, fd, 0, sortie, sizeof(sortie));
        verifier(n == 1 && sortie[0] == 0,
            "ft_putchar_fd(NUL) sur fd=%d, recu=%zd", fd, n);
    }
    fin();
}

static void test_fd_independance(void)
{
    int a[2];
    int b[2];
    unsigned char sortie_a[FD_CAPACITE];
    unsigned char sortie_b[FD_CAPACITE];
    const unsigned char attendu_a[] = {0, 'A', '\n', 'Z'};
    const unsigned char attendu_b[] = {'\n', '-', '4', '2', '\n'};
    ssize_t na;
    ssize_t nb;

    debut("ft_put*_fd (2 descripteurs, sorties independantes)");
    if (pipe(a) != 0)
    {
        verifier(0, "creation pipe A impossible");
        fin();
        return ;
    }
    if (pipe(b) != 0)
    {
        close(a[0]);
        close(a[1]);
        verifier(0, "creation pipe B impossible");
        fin();
        return ;
    }
    ft_putchar_fd('\0', a[1]);
    ft_putendl_fd("", b[1]);
    ft_putstr_fd("A\n", a[1]);
    ft_putnbr_fd(-42, b[1]);
    ft_putstr_fd("Z", a[1]);
    ft_putchar_fd('\n', b[1]);
    close(a[1]);
    close(b[1]);
    na = fd_lire_total(a[0], sortie_a, sizeof(sortie_a));
    nb = fd_lire_total(b[0], sortie_b, sizeof(sortie_b));
    close(a[0]);
    close(b[0]);
    verifier(na == (ssize_t)sizeof(attendu_a) &&
        memcmp(sortie_a, attendu_a, sizeof(attendu_a)) == 0,
        "pipe A : attendu 00 41 0A 5A, obtenu %zd octets", na);
    verifier(nb == (ssize_t)sizeof(attendu_b) &&
        memcmp(sortie_b, attendu_b, sizeof(attendu_b)) == 0,
        "pipe B : attendu 0A 2D 34 32 0A, obtenu %zd octets", nb);
    fin();
}

static void test_fd_renforces(void)
{
    printf("\n===== TESTS RENFORCES : SORTIES FD (OCTETS EXACTS) =====\n");
    test_fd_cas_fixes();
    test_fd_longueurs_et_aleatoires();
    test_fd_descripteurs_speciaux();
    test_fd_independance();
}


/*
** Cas exhaustifs de petites chaines : rien ne termine une chaine hors NUL.
** Couvre entre autres LF/CR, octets superieurs a 127, chaines imbriquees.
*/
#define PETITS_CAS 85
static void preparer_petits_cas(char cas[PETITS_CAS][5])
{
    const unsigned char alphabet[4] = {'a', '\n', '\r', 255};
    size_t longueur;
    size_t combinaisons;
    size_t choix;
    size_t i;
    size_t j;
    size_t index;
    size_t x;

    index = 0;
    for (longueur = 0; longueur <= 3; longueur++)
    {
        combinaisons = 1;
        for (i = 0; i < longueur; i++)
            combinaisons *= 4;
        for (choix = 0; choix < combinaisons; choix++)
        {
            x = choix;
            for (j = 0; j < longueur; j++)
            {
                cas[index][j] = (char)alphabet[x % 4];
                x /= 4;
            }
            cas[index][longueur] = '\0';
            index++;
        }
    }
}

static void test_exhaustif_petites_chaines(void)
{
    char cas[PETITS_CAS][5];
    char a[32];
    char b[32];
    size_t i;
    size_t j;
    size_t n;
    size_t attendu;
    int c;

    preparer_petits_cas(cas);
    debut("EXHAUSTIF ft_strchr / ft_strrchr (LF/CR/0xFF)");
    for (i = 0; i < PETITS_CAS; i++)
        for (c = -258; c <= 512; c++)
        {
            verifier(ft_strchr(cas[i], c) == strchr(cas[i], c),
                "cas=%zu caractere=%d", i, c);
            verifier(ft_strrchr(cas[i], c) == strrchr(cas[i], c),
                "cas=%zu caractere=%d", i, c);
        }
    fin();
    debut("EXHAUSTIF ft_strncmp / ft_strnstr");
    for (i = 0; i < PETITS_CAS; i++)
        for (j = 0; j < PETITS_CAS; j++)
            for (n = 0; n <= 8; n++)
            {
                verifier(signe(ft_strncmp(cas[i], cas[j], n))
                    == signe(strncmp(cas[i], cas[j], n)),
                    "strncmp i=%zu j=%zu n=%zu", i, j, n);
                verifier(ft_strnstr(cas[i], cas[j], n)
                    == cherche_limitee_ref(cas[i], cas[j], n),
                    "strnstr i=%zu j=%zu n=%zu", i, j, n);
            }
    fin();
    debut("EXHAUSTIF ft_strlcpy (tailles et canaris)");
    for (i = 0; i < PETITS_CAS; i++)
        for (n = 0; n <= 16; n++)
        {
            memset(a, 0x6d, sizeof(a));
            memset(b, 0x6d, sizeof(b));
            attendu = copie_limitee_ref(a + 4, cas[i], n);
            verifier(ft_strlcpy(b + 4, cas[i], n) == attendu
                && memcmp(a, b, sizeof(a)) == 0,
                "cas=%zu size=%zu", i, n);
        }
    fin();
    debut("EXHAUSTIF ft_strlcat (tailles et canaris)");
    for (i = 0; i < PETITS_CAS; i++)
        for (j = 0; j < PETITS_CAS; j++)
            for (n = 0; n <= 16; n++)
            {
                memset(a, 0x6d, sizeof(a));
                memset(b, 0x6d, sizeof(b));
                memcpy(a + 4, cas[i], strlen(cas[i]) + 1);
                memcpy(b + 4, cas[i], strlen(cas[i]) + 1);
                attendu = concat_limitee_ref(a + 4, cas[j], n);
                verifier(ft_strlcat(b + 4, cas[j], n) == attendu
                    && memcmp(a, b, sizeof(a)) == 0,
                    "destination=%zu source=%zu size=%zu", i, j, n);
            }
    fin();
}

static void test_exhaustif_memoire(void)
{
    unsigned char src[128];
    unsigned char a[64];
    unsigned char b[64];
    size_t i;
    size_t j;
    size_t n;
    size_t maximum;
    int c;

    debut("EXHAUSTIF ft_memcmp (256 x 256 octets)");
    for (i = 0; i < 256; i++)
        for (j = 0; j < 256; j++)
        {
            a[0] = (unsigned char)i;
            b[0] = (unsigned char)j;
            verifier(signe(ft_memcmp(a, b, 1))
                == signe(memcmp(a, b, 1)),
                "octets %zu et %zu", i, j);
            verifier(ft_memcmp(a, b, 0) == 0,
                "n=0, octets %zu et %zu", i, j);
        }
    fin();
    debut("EXHAUSTIF ft_memchr (positions / valeurs)");
    for (i = 0; i < sizeof(src); i++)
        src[i] = (unsigned char)(i * 7u);
    for (n = 0; n <= sizeof(src); n++)
        for (c = -256; c <= 511; c++)
            verifier(ft_memchr(src, c, n) == memchr(src, c, n),
                "n=%zu c=%d", n, c);
    fin();
    debut("EXHAUSTIF ft_memmove (tous decalages / canaris)");
    for (i = 0; i < 48; i++)
        for (j = 0; j < 48; j++)
        {
            maximum = 48 - (i > j ? i : j);
            for (n = 0; n <= maximum; n++)
            {
                octets_aleatoires(a, sizeof(a));
                memcpy(b, a, sizeof(a));
                memmove(a + 8 + i, a + 8 + j, n);
                verifier(ft_memmove(b + 8 + i, b + 8 + j, n)
                    == b + 8 + i && memcmp(a, b, sizeof(a)) == 0,
                    "destination=%zu source=%zu n=%zu", i, j, n);
            }
        }
    fin();
    debut("EXHAUSTIF ft_memcpy (zones distinctes / canaris)");
    for (i = 0; i <= 48; i++)
    {
        octets_aleatoires(src, sizeof(src));
        for (n = 0; n <= 48; n++)
        {
            memset(a, 0x51, sizeof(a));
            memset(b, 0x51, sizeof(b));
            memcpy(a + 8, src + i, n);
            verifier(ft_memcpy(b + 8, src + i, n) == b + 8
                && memcmp(a, b, sizeof(a)) == 0,
                "source_offset=%zu n=%zu", i, n);
        }
    }
    fin();
    debut("EXHAUSTIF ft_memset / ft_bzero (toutes tailles)");
    for (i = 0; i <= 48; i++)
        for (n = 0; n <= 48; n++)
        {
            memset(a, 0xa7, sizeof(a));
            memset(b, 0xa7, sizeof(b));
            memset(a + 8, (int)(i * 11u), n);
            verifier(ft_memset(b + 8, (int)(i * 11u), n) == b + 8
                && memcmp(a, b, sizeof(a)) == 0,
                "memset value=%zu n=%zu", i, n);
            memset(a + 8, 0, n);
            ft_bzero(b + 8, n);
            verifier(memcmp(a, b, sizeof(a)) == 0,
                "bzero value=%zu n=%zu", i, n);
        }
    fin();
}

static char cb_nul_mapi(unsigned int index, char c)
{
    if (index == 1)
        return ('\0');
    return ((char)(c + 1));
}

static void cb_nul_iteri(unsigned int index, char *c)
{
    if (index == 1)
        *c = '\0';
    else
        *c = (char)(*c + 1);
}

static void test_callbacks_nul(void)
{
    char attendu[] = {'b', '\0', 'd', 'e', '\0'};
    char original[] = "abcd";
    char *copie;

    debut("ft_strmapi (NUL dans le resultat, octets suivants)");
    copie = ft_strmapi("abcd", cb_nul_mapi);
    verifier(copie != NULL && memcmp(copie, attendu, sizeof(attendu)) == 0,
        "la transformation doit continuer apres un NUL produit");
    free(copie);
    fin();
    debut("ft_striteri (NUL genere au milieu, octets suivants)");
    ft_striteri(original, cb_nul_iteri);
    verifier(memcmp(original, attendu, sizeof(attendu)) == 0,
        "le callback ne doit pas interrompre les indices suivants");
    fin();
}

static void test_bornes_allocation(void)
{
    void *a;
    void *b;
    char *s;
    size_t i;
    const size_t produits[][2] = {
        {(size_t)-1, 2}, {(size_t)-1, 3},
        {(size_t)-1 / 2 + 1, 2}, {(size_t)-1 / 3 + 1, 3},
        {(size_t)-1 / 5 + 1, 5}, {(size_t)-1 / 16 + 1, 16}
    };

    debut("ft_calloc (zero, unicite, overflow multiplication)");
    a = ft_calloc(0, 0);
    b = ft_calloc(0, 0);
    verifier(a != NULL && b != NULL && a != b,
        "calloc(0,0): pointeurs non nuls simultanement distincts");
    free(a);
    free(b);
    for (i = 0; i < sizeof(produits) / sizeof(produits[0]); i++)
    {
        a = ft_calloc(produits[i][0], produits[i][1]);
        verifier(a == NULL, "overflow %zu * %zu",
            produits[i][0], produits[i][1]);
        free(a);
    }
    fin();
    debut("ft_substr (len=SIZE_MAX, indices hors chaine)");
    s = ft_substr("A\nBC\rD", 1, SIZE_MAX);
    verifier(s && strcmp(s, "\nBC\rD") == 0,
        "start=1 len=SIZE_MAX");
    free(s);
    s = ft_substr("abc", UINT_MAX, SIZE_MAX);
    verifier(s && strcmp(s, "") == 0, "start=UINT_MAX len=SIZE_MAX");
    free(s);
    fin();
    debut("ft_split (grand nombre de separateurs / mots)");
    {
        char texte[4096];
        size_t j;
        char **mots;

        for (j = 0; j < 4094; j++)
            texte[j] = (j % 2) ? ',' : 'a';
        texte[4094] = ',';
        texte[4095] = '\0';
        mots = ft_split(texte, ',');
        verifier(mots != NULL, "allocation grande liste mots");
        if (mots)
        {
            j = 0;
            while (j < 2047 && mots[j] && strcmp(mots[j], "a") == 0)
                j++;
            verifier(j == 2047 && mots[j] == NULL,
                "2047 mots 'a' correctement alloues et termines");
        }
        liberer_mots(mots);
    }
    fin();
}

static void test_grandes_chaines(void)
{
    const size_t taille = 65536;
    char *entree;
    char *copie;
    char *destination;
    size_t i;

    debut("STRESS : strlen/strchr/strrchr/strdup/strlcpy/strnstr");
    entree = malloc(taille + 1);
    destination = malloc(taille + 16);
    if (!entree || !destination)
    {
        verifier(0, "allocation de preparation impossible");
        free(entree);
        free(destination);
        fin();
        return ;
    }
    for (i = 0; i < taille; i++)
        entree[i] = 'a';
    entree[4096] = '\n';
    entree[taille - 2] = '\n';
    entree[taille - 1] = 'Z';
    entree[taille] = '\0';
    verifier(ft_strlen(entree) == taille, "strlen de 65536 caracteres");
    verifier(ft_strchr(entree, '\n') == entree + 4096,
        "premier LF a l'indice 4096");
    verifier(ft_strrchr(entree, '\n') == entree + taille - 2,
        "dernier LF a l'indice 65534");
    verifier(ft_strnstr(entree, "\nZ", taille) == entree + taille - 2,
        "motif dans les deux derniers octets");
    verifier(ft_strnstr(entree, "\nZ", taille - 1) == NULL,
        "motif tronque par la limite");
    copie = ft_strdup(entree);
    verifier(copie && copie != entree
        && memcmp(copie, entree, taille + 1) == 0,
        "strdup de 65536 caracteres");
    free(copie);
    memset(destination, 0x7f, taille + 16);
    verifier(ft_strlcpy(destination + 4, entree, taille + 1) == taille
        && memcmp(destination + 4, entree, taille + 1) == 0
        && (unsigned char)destination[3] == 0x7f
        && (unsigned char)destination[taille + 5] == 0x7f,
        "strlcpy integral + canaris sur 65536 caracteres");
    free(entree);
    free(destination);
    fin();
}

static void test_listes_etendues(void)
{
    t_list noeuds[256];
    t_list *liste;
    t_list *copie;
    int valeurs[256];
    size_t i;

    debut("ft_lstadd_front / ft_lstadd_back (liste vide)");
    liste = NULL;
    noeuds[0].content = NULL;
    noeuds[0].next = NULL;
    ft_lstadd_front(&liste, &noeuds[0]);
    verifier(liste == &noeuds[0] && liste->next == NULL,
        "front sur liste vide");
    liste = NULL;
    ft_lstadd_back(&liste, &noeuds[0]);
    verifier(liste == &noeuds[0] && liste->next == NULL,
        "back sur liste vide");
    fin();
    debut("ft_lstsize / ft_lstlast (256 noeuds / sous-listes)");
    for (i = 0; i < 256; i++)
    {
        valeurs[i] = (int)i;
        noeuds[i].content = &valeurs[i];
        noeuds[i].next = (i < 255) ? &noeuds[i + 1] : NULL;
    }
    for (i = 0; i < 256; i++)
    {
        verifier(ft_lstsize(&noeuds[i]) == 256 - i,
            "size a l'indice %zu", i);
        verifier(ft_lstlast(&noeuds[i]) == &noeuds[255],
            "last a l'indice %zu", i);
    }
    fin();
    debut("ft_lstmap (NULL et liste independante)");
    copie = ft_lstmap(NULL, mapper_entier, detruire_entier);
    verifier(copie == NULL, "map de NULL doit renvoyer NULL");
    verifier(ft_lstsize(&noeuds[0]) == 256,
        "liste initiale inchangee");
    fin();
}

#ifdef TEST_WRAP_MALLOC
/*
** Test d'allocations forcees en erreur par l'editeur de liens GNU.
** Ce bloc n'est compile QUE pour le second executable du script.
** __wrap_malloc enregistre chaque allocation provenant de libft.a.
*/
#define TRACES_ALLOC 8192
static void *g_pointeurs_alloc[TRACES_ALLOC];
static unsigned char g_pointeurs_vivants[TRACES_ALLOC];
static size_t g_nombre_alloc;
static size_t g_total_alloc;
static size_t g_echec_au;
static size_t g_nombres_echecs;
static int g_trace_active;
static size_t g_cb_crees;
static size_t g_cb_liberes;

void *__real_malloc(size_t taille);
void __real_free(void *ptr);

void *__wrap_malloc(size_t taille)
{
    void *ptr;

    if (g_trace_active)
    {
        g_total_alloc++;
        if (g_echec_au && g_total_alloc == g_echec_au)
        {
            g_nombres_echecs++;
            return (NULL);
        }
    }
    ptr = __real_malloc(taille);
    if (g_trace_active && ptr && g_nombre_alloc < TRACES_ALLOC)
    {
        g_pointeurs_alloc[g_nombre_alloc] = ptr;
        g_pointeurs_vivants[g_nombre_alloc] = 1;
        g_nombre_alloc++;
    }
    return (ptr);
}

void __wrap_free(void *ptr)
{
    size_t i;

    i = g_nombre_alloc;
    while (i > 0)
    {
        i--;
        if (g_pointeurs_alloc[i] == ptr && g_pointeurs_vivants[i])
        {
            g_pointeurs_vivants[i] = 0;
            break ;
        }
    }
    __real_free(ptr);
}

static void preparer_injection(size_t echec_au)
{
    g_trace_active = 0;
    g_nombre_alloc = 0;
    g_total_alloc = 0;
    g_echec_au = echec_au;
    g_nombres_echecs = 0;
    g_trace_active = 1;
}

static size_t allocations_restantes(void)
{
    size_t i;
    size_t nombre;

    nombre = 0;
    for (i = 0; i < g_nombre_alloc; i++)
        if (g_pointeurs_vivants[i])
            nombre++;
    return (nombre);
}

static void *cb_mapper_injection(void *element)
{
    int *nouveau;

    nouveau = __real_malloc(sizeof(*nouveau));
    if (nouveau)
    {
        *nouveau = *(int *)element + 7;
        g_cb_crees++;
    }
    return (nouveau);
}

static void cb_detruire_injection(void *element)
{
    if (element)
        g_cb_liberes++;
    __real_free(element);
}

static void test_injection_fonctions_simples(void)
{
    int numero;
    void *p;
    t_list *noeud;

    debut("MALLOC FAIL : allocations simples et branches speciales");
    for (numero = 0; numero < 9; numero++)
    {
        preparer_injection(1);
        if (numero == 0)
            p = ft_strdup("bonjour\n");
        else if (numero == 1)
            p = ft_substr("bonjour\n", 2, 5);
        else if (numero == 2)
            p = ft_strjoin("bonjour", "\nmonde");
        else if (numero == 3)
            p = ft_strtrim("  Bonjour  ", " ");
        else if (numero == 4)
            p = ft_strmapi("abc", application);
        else if (numero == 5)
            p = ft_itoa(INT_MIN);
        else if (numero == 6)
            p = ft_itoa(0);
        else if (numero == 7)
            p = ft_strtrim("aaaa", "a");
        else
            p = ft_substr("a", UINT_MAX, 10);
        g_trace_active = 0;
        verifier(p == NULL && g_nombres_echecs == 1
            && allocations_restantes() == 0,
            "fonction numero %d: resultat=%p, echec=%zu, alloc_vivantes=%zu",
            numero, p, g_nombres_echecs, allocations_restantes());
        free(p);
    }
    fin();
    debut("MALLOC FAIL : calloc et lstnew");
    preparer_injection(1);
    p = ft_calloc(3, 4);
    g_trace_active = 0;
    verifier(p == NULL && g_nombres_echecs == 1
        && allocations_restantes() == 0,
        "calloc sous echec d'allocation");
    free(p);
    preparer_injection(1);
    noeud = ft_lstnew(NULL);
    g_trace_active = 0;
    verifier(noeud == NULL && g_nombres_echecs == 1
        && allocations_restantes() == 0,
        "lstnew sous echec d'allocation");
    free(noeud);
    preparer_injection(1);
    p = ft_calloc(SIZE_MAX, 2);
    g_trace_active = 0;
    verifier(p == NULL && g_total_alloc == 0,
        "calloc avec overflow ne doit pas essayer malloc");
    free(p);
    fin();
}

static void test_injection_split(void)
{
    size_t indice;
    size_t fuites;
    size_t echecs;
    char **mots;
    int valide;

    debut("MALLOC FAIL : ft_split (chacune des allocations)");
    for (indice = 1; indice <= 8; indice++)
    {
        preparer_injection(indice);
        mots = ft_split("un,deux,trois,quatre", ',');
        g_trace_active = 0;
        echecs = g_nombres_echecs;
        valide = mots && mots[0] && mots[1] && mots[2] && mots[3]
            && !mots[4] && !strcmp(mots[0], "un")
            && !strcmp(mots[1], "deux") && !strcmp(mots[2], "trois")
            && !strcmp(mots[3], "quatre");
        if (echecs)
        {
            fuites = allocations_restantes();
            verifier(mots == NULL && fuites == 0,
                "echec malloc #%zu => retour=%p / allocations non liberees=%zu",
                indice, (void *)mots, fuites);
        }
        else
            verifier(valide, "malloc #%zu, aucun echec => resultat invalide",
                indice);
        liberer_mots(mots);
        verifier(allocations_restantes() == 0,
            "malloc #%zu: allocations restantes apres nettoyage=%zu",
            indice, allocations_restantes());
    }
    fin();
}

static void test_injection_lstmap(void)
{
    size_t indice;
    size_t echecs;
    t_list a;
    t_list b;
    t_list c;
    t_list *res;
    int valeurs[] = {3, 11, 29};
    int valable;

    a.content = &valeurs[0];
    b.content = &valeurs[1];
    c.content = &valeurs[2];
    a.next = &b;
    b.next = &c;
    c.next = NULL;
    debut("MALLOC FAIL : ft_lstmap (chaque nouveau noeud)");
    for (indice = 1; indice <= 6; indice++)
    {
        g_cb_crees = 0;
        g_cb_liberes = 0;
        preparer_injection(indice);
        res = ft_lstmap(&a, cb_mapper_injection, cb_detruire_injection);
        g_trace_active = 0;
        echecs = g_nombres_echecs;
        valable = res && res->next && res->next->next
            && !res->next->next->next
            && *(int *)res->content == 10
            && *(int *)res->next->content == 18
            && *(int *)res->next->next->content == 36;
        if (echecs)
            verifier(res == NULL && allocations_restantes() == 0
                && g_cb_crees == g_cb_liberes,
                "malloc #%zu: retour=%p, nœuds non liberes=%zu, contenus %zu/%zu",
                indice, (void *)res, allocations_restantes(),
                g_cb_liberes, g_cb_crees);
        else
            verifier(valable, "malloc #%zu: map invalide", indice);
        ft_lstclear(&res, cb_detruire_injection);
        verifier(allocations_restantes() == 0
            && g_cb_crees == g_cb_liberes
            && a.next == &b && b.next == &c && c.next == NULL,
            "malloc #%zu: apres nettoyage nœuds=%zu contenus %zu/%zu",
            indice, allocations_restantes(), g_cb_liberes, g_cb_crees);
    }
    fin();
}

static void test_injection_malloc(void)
{
    printf("\n===== MALLOC : ECHECS FORCES / FUITES / NETTOYAGE =====\n");
    test_injection_fonctions_simples();
    test_injection_split();
    test_injection_lstmap();
}
#endif


int main(void)
{
    printf("===== TEST COMPLET LIBFT v19.3 (43 fonctions) =====\n");
    printf("graine initiale = 0x42C0FFEE ; tours par fonction ~%d\n", TOURS);
    printf("Ne tester que les entrees definies par le sujet/man.\n\n");
    test_is_et_casse();
    test_strlen_et_dup();
    test_chr_et_ncmp();
    test_strnstr();
    test_strl();
    test_memset_bzero();
    test_memcpy_move();
    test_memchr_cmp();
    test_calloc();
    test_atoi_itoa();
    test_substr_join_trim();
    test_split();
    test_mapi_iteri();
    test_putfd();
    test_lstnew();
    test_lstadd_front_back();
    test_lstsize_last();
    test_lstdelone_clear();
    test_lstiter_map();
    printf("\n===== TESTS EXPLICITES : LF/CR/TAB/NUL =====\n");
    test_controles_chaines();
    test_controles_memoire();
    test_controles_supplementaires();
    test_controles_sorties();
    test_fd_renforces();
    printf("\n===== TESTS EXHAUSTIFS ET CAS LIMITES =====\n");
    test_exhaustif_petites_chaines();
    test_exhaustif_memoire();
    test_callbacks_nul();
    test_bornes_allocation();
    test_listes_etendues();
    test_grandes_chaines();
#ifdef TEST_WRAP_MALLOC
    test_injection_malloc();
#endif
    printf("\nRESULTAT : %lu tests, %lu erreurs\n", g_tests, g_erreurs);
    if (g_erreurs)
        return (1);
    return (0);
}
