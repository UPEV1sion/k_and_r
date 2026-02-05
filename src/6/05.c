#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

int main(void)
{
    install("Hello", "World");
    puts(lookup("Hello")->defn);

    undef("Hello");
    puts(lookup("Hello") ? "present" : "not present");

    return 0;
}

