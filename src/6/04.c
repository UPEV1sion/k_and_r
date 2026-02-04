#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#define BUFSIZE 1024
#define MAXWORDS 1024

typedef struct Node Node;
struct Node {
    char *word;
    size_t count;
    Node *left;
    Node *right;
};

char* strdup(const char *str)
{
    char *s = malloc(strlen(str) + 1);
    strcpy(s, str);
    return s;
}

Node* alloc_node(const char *word)
{
    Node *node = malloc(sizeof(Node));
    assert(node && "Out of mem!");
    node->count = 1;
    node->word = strdup(word);
    node->left = node->right = NULL;
    return node;
}

Node* add_word(Node *tree, const char *word)
{
    int cond;
    if(tree == NULL)
    {
        tree = alloc_node(word);
    }
    else if((cond = strcmp(tree->word, word)) == 0)
    {
        tree->count++;
    }
    else if(cond < 0)
    {
        tree->left = add_word(tree->left, word);
    }
    else
    {
        tree->right = add_word(tree->right, word);
    }

    return tree;
}

void flatten_tree(Node *list[MAXWORDS], size_t *len, Node *tree)
{
    if(*len >= MAXWORDS || tree == NULL) return;

    flatten_tree(list, len, tree->left);
    list[(*len)++] = tree;
    flatten_tree(list, len, tree->right); 
}

int getword(char *word, int lim)
{
    char *w = word;
    int c;
    while((c = getchar()) != EOF && !isalpha(c))
        ;
    if(c == EOF) return 0;

    *w++ = tolower(c);
    while(--lim > 1 && (c = getchar()) != EOF && isalnum(c))
    {
        *w++ = tolower(c);
    }

    *w = 0;
    return w - word;
}

int comp_node(const void *a, const void *b)
{
    Node *this = *(Node **) a;
    Node *that = *(Node **) b;
    return that->count - this->count;
}

void free_tree(Node *tree)
{
    if(tree == NULL) return;
    free_tree(tree->left);
    free_tree(tree->right);
    free(tree->word);
    free(tree);
}

int main(void)
{
    Node *tree = NULL;
    char buffer[BUFSIZE];
    while(getword(buffer, sizeof buffer) > 0)
    {
        tree = add_word(tree, buffer);
    }

    Node *list[MAXWORDS];
    size_t len = 0;
    flatten_tree(list, &len, tree);
    qsort(list, len, sizeof(Node*), comp_node);

    for(size_t i = 0; i < len; ++i)
    {
        printf("%zu: %s\n", list[i]->count, list[i]->word);
    }

    free_tree(tree);

    return 0;
}
