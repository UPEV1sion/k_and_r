// Since the compiler does not care about whitespace, i omit the user formatting

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#define BUFSIZE 1024

int getch(void);
void ungetch(int);

struct nlist {
    char *name;
    char *defn;
    struct nlist *next;
};

#define HASHSIZE 101
struct nlist *hashtab[HASHSIZE];

char* strdup(const char *str)
{
    char *s = malloc(strlen(str) + 1);
    if(!s) return NULL;
    strcpy(s, str);
    return s;
}

unsigned hash(const char *word)
{
    unsigned hashval;
    for(hashval = 0; *word; word++)
    {
        hashval = hashval * 31 + *word;
    }

    return hashval % HASHSIZE;
}

struct nlist* lookup(const char *name)
{
    for(struct nlist *node = hashtab[hash(name)]; node; node = node->next)
    {
        if(strcmp(name, node->name) == 0) return node;
    }
    
    return NULL;
}

struct nlist* install(const char *name, const char *defn)
{
    struct nlist *node;
    if((node = lookup(name)) == NULL)
    {   
        node = malloc(sizeof(struct nlist));
        if(node == NULL || (node->name = strdup(name)) == NULL)
            return NULL;
        const unsigned hashval = hash(name);
        node->next = hashtab[hashval];
        hashtab[hashval] = node;
    }   
    else
    {
        free(node->defn);
    }

    if((node->defn = strdup(defn)) == NULL) return NULL;
    return node;
}

int undef(const char *name)
{
    const unsigned hashval = hash(name);
    struct nlist *prev = NULL;
    struct nlist *cur = hashtab[hashval];

    while(cur)
    {
        if(strcmp(cur->name, name) == 0)
        {
            if(prev == NULL)
            {
                hashtab[hashval] = cur->next;
            }
            else
            {
                prev->next = cur->next;
            }

            free(cur->name);
            free(cur->defn);
            free(cur);
            return 0;
        }

        prev = cur;
        cur = cur->next;
    }

    return -1;
}

void comment(void)
{
    int c;
    while((c = getch()) != EOF)
    {
        if(c == '*')
        {
            if((c = getch()) == '/')
            {
                break;
            }
            else
            {
                ungetch(c);
            }
        }
    }
}

void skip_space(void)
{
    int c;
    while(isspace((unsigned char) (c = getch())))
        ;
    ungetch(c);
}

int getword(char *word, int lim)
{
    char *w = word;
    skip_space();

    int c = getch();
    if(c == EOF) return EOF;

    *w++ = c;

    if(isalpha((unsigned char) c) || c == '_' || c == '#')
    {
        for(; --lim > 0; w++)
        {
            if(!isalnum((unsigned char) (*w = getch())) && *w != '_')
            {
                ungetch(*w);
                break;
            }
        }
    }
    else if(c == '\'' || c == '"')
    {
        for(; --lim > 0; w++)
        {
            if((*w = getch()) == '\\')
            {
                if(--lim <= 0) break;
                *++w = getch();
            }
            else if(*w == c)
            {
                w++;
                break;
            }
            else if(*w == EOF)
            {
                break;
            }
        }
    }
    else if(c == '/')
    {
        int d;
        if((d = getch()) == '*')
        {
            comment();
            return getword(word, lim);
        }
        else if(d == '/')
        {
            while((c = getch()) != '\n' && c != EOF);
            return getword(word, lim);
        }
        else
        {
            ungetch(d);
        }
    }
    else if(isdigit((unsigned char) c))
    {
        for(; --lim > 0; w++)
        {
            if(!isdigit((unsigned char) (*w = c = getch()))) break;
        }
        if(c == '.')
        {
            for(; --lim > 0; w++)
            {
                if(!isdigit((unsigned char) (*w = c = getch()))) break;
            }
        }

        if(c != EOF) ungetch(c);
    }

    *w = 0;
    return word[0];
}

int getdefn(char *buffer, int lim)
{
    skip_space();
    int c = getch();
    if(c == EOF) return EOF;
    *buffer++ = c;
    while(--lim > 1 && (c = getch()) != EOF)
    {
        if(c == '\n') break;

        if(c == '\\')
        {
            int next;
            if((next = getch()) == EOF) return EOF;
            
            if(next != '\n' && (lim - 2) > 1)
            {
                *buffer++ = c;
                *buffer++ = next;
                lim -= 2;
            }
            else 
            {
                skip_space();
                ungetch(' ');
            }
        }
        else
        {
            *buffer++ = c;
        }
    }

    *buffer = 0;

    return 0;
}

int preprocessor_directive(const char *token)
{
    char name[BUFSIZE];
    char defn[BUFSIZE];

    if(strcmp("#define", token) == 0)
    {
        if(getword(name, sizeof name) == EOF || getdefn(defn, sizeof defn) == EOF)
        {
            fprintf(stderr, "error: while processing \"#define\" EOF occured!\n");
            return 1;
        }

        if(install(name, defn) == NULL)
        {
            fprintf(stderr, "error: too many preprocessor macros!\n");
            return 1;
        }
    }
    else if(strcmp("#undef", token) == 0)
    {
        if(getword(name, sizeof name) == EOF)
        {
            fprintf(stderr, "error: while processing \"#undef\" EOF occured\n");
            return 1;
        }

        if(undef(name) != 0)
        {
            fprintf(stderr, "error: could not undef \"%s\": macro is not defined!\n", name);
            return 1;
        }
    }
    else
    {
        printf("%s ", token);
    }

    return 0;
}

int main(void)
{
    char token[BUFSIZE];
    int type;
    while((type = getword(token, sizeof token)) != EOF)
    {
        if(type == '#')
        {
            if(preprocessor_directive(token) != 0) return 1;
        }
        else if(isalpha((unsigned char) type) || type == '_')
        {
            struct nlist *replacement;
            if((replacement = lookup(token)) != NULL)
            {
                printf("%s ", replacement->defn);
            }
            else
            {
                printf("%s ", token);
            }   
        }
        else
        {
            printf("%s ", token);
        }
    }

    puts("");

    return 0;
}

static int buf[BUFSIZE];
static size_t bufp;

int getch(void)
{
    return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(const int c)
{
    if(bufp < BUFSIZE) buf[bufp++] = c;
    else fprintf(stderr, "error: buffer overflow!\n");
}
