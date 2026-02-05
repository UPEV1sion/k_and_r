#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#define BUFSIZE 1024
#define ARRAY_LEN(a) (sizeof(a)/sizeof(a[0]))

typedef struct LNode LNode;
struct LNode {
    size_t line;
    LNode *next;
};

typedef struct TNode TNode;
struct TNode{
    char *word;
    LNode *lines;
    TNode *left;
    TNode *right;
};

static char *noise_words[] = {
    "a",
    "an",
    "are",
    "at",
    "but",
    "by",
    "for",
    "in",
    "is",
    "nor",
    "of",
    "on",
    "or",
    "so",
    "the",
    "was",
    "were",
    "with",
    "yet",
};

char* strdup(const char *str)
{
    char *s = malloc(strlen(str) + 1);
    strcpy(s, str);
    return s;
}

LNode* alloc_line_node(const size_t line)
{
    LNode *node = malloc(sizeof(LNode));
    assert(node && "Out of mem!");
    node->line = line;
    node->next = NULL;
    return node;
}

TNode* alloc_tree_node(const char *word, const size_t line)
{
    TNode *node = malloc(sizeof(TNode));
    assert(node && "Out of mem!");
    node->word = strdup(word);
    node->lines = alloc_line_node(line);
    node->left = node->right = NULL;
    return node;
}

TNode* add_word(TNode *tree, char *word, size_t line)
{
    int cond;
    if(tree == NULL)
    {
        tree = alloc_tree_node(word, line);
    }
    else if((cond = strcmp(word, tree->word)) == 0)
    {
        LNode *node = alloc_line_node(line);
        node->next = tree->lines;
        tree->lines = node;
    }
    else if(cond < 0)
    {
        tree->left = add_word(tree->left, word, line);
    }
    else
    {
        tree->right = add_word(tree->right, word, line);
    }

    return tree;
}

int getword(char *word, size_t *line, int lim)
{
    char *w = word;
    int c;
    while((c = getchar()) != EOF)
    {
        if(c == '\n') (*line)++;
        if (isalpha(c)) break;
    }
    if(c == EOF) return 0;

    *w++ = tolower(c);
    while(--lim > 1 && (c = getchar()) != EOF && isalnum(c))
    {
        *w++ = tolower(c);
    }

    *w = 0;
    return w - word;
}

void dump_tree(TNode *tree)
{
    if(tree == NULL) return;

    dump_tree(tree->left);
    printf("%s", tree->word);
    LNode *lines = tree->lines;
    while(lines) 
    {
        printf(":%zu", lines->line);
        lines = lines->next; 
    }
    puts("");
    dump_tree(tree->right);
}

void free_lines(LNode *lines)
{
    if(lines == NULL) return;

    free_lines(lines->next);
    free(lines);
}

void free_tree(TNode *tree)
{
    if(tree == NULL) return;

    free_tree(tree->right);
    free_tree(tree->left);
    free_lines(tree->lines);
    free(tree->word);
    free(tree);
}

int comp_str(const void *s1, const void *s2)
{
    const char *k = s1; 
    const char *e = *(const char **)s2;
    return strcmp(k, e);
}

bool is_noise(char *word)
{
    return bsearch(word, noise_words, ARRAY_LEN(noise_words), sizeof(char*), comp_str) != NULL;
}

int main(void)
{
    TNode *tree = NULL;
    char buffer[BUFSIZE];
    size_t line = 1;
    while(getword(buffer, &line, sizeof(buffer)) > 0)
    {
        if(is_noise(buffer)) continue;
        tree = add_word(tree, buffer, line); 
    }

    dump_tree(tree);
    free_tree(tree);

    return 0;
}
