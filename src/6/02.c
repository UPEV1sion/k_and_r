#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#define MATCH_FLAG "-m"
#define ARRAY_LEN(a) (sizeof(a)/sizeof(a[0]))
#define BUFSIZE 1024

typedef enum {
    TOK_ID,
    TOK_EOF,
} TokenType;

typedef struct NameNode NameNode;
struct NameNode {
    char *name;
    NameNode *left;
    NameNode *right;   
};

typedef struct GroupNode GroupNode;
struct GroupNode {
    char *group_name;
    NameNode *names;
    GroupNode *left;
    GroupNode *right;
};

static char *keywords[] = {
    "#define",
    "#elif",
    "#else",
    "#endif",
    "#error",
    "#if",
    "#ifdef",
    "#ifndef",
    "#include",
    "#line",
    "#pragma",
    "#undef",
    "auto",
    "break",
    "case",
    "char",
    "const",
    "continue",
    "default",
    "do",
    "double",
    "else",
    "enum",
    "extern",
    "float",
    "for",
    "goto",
    "if",
    "inline",
    "int",
    "long",
    "register",
    "restrict",
    "return",
    "short",
    "signed",
    "sizeof",
    "static",
    "struct",
    "switch",
    "typedef",
    "union",
    "unsigned",
    "void",
    "volatile",
    "while"
};

int getch(void);
void ungetch(int);

char* strdup(char *str)
{
    char *s = malloc(strlen(str) + 1);
    strcpy(s, str);
    return s;
}

char* strndup(char *str, const size_t n)
{
    char *s = malloc(n + 1);
    strncpy(s, str, n);
    s[n] = 0;
    return s;
}

NameNode* name_alloc(char *word)
{
    NameNode *node = malloc(sizeof(NameNode));
    assert(node && "out of mem!");
    node->name = strdup(word);
    node->left = node->right = NULL;

    return node;
}

GroupNode* group_alloc(char *word, const int prefix_len)
{
    GroupNode *group = malloc(sizeof(GroupNode));
    assert(group && "out of mem!");
    group->group_name = strndup(word, prefix_len);
    group->left = group->right = NULL;
    group->names = name_alloc(word); 

    return group;
}

NameNode* addname(NameNode *node, char *word)
{
    int cond;
    if(node == NULL)
    {
        node = name_alloc(word);
    }
    else if((cond = strcmp(word, node->name)) < 0)
    {
        node->left = addname(node->left, word); 
    }
    else if(cond > 0)
    {
        node->right = addname(node->right, word);
    }

    return node;
}

GroupNode* addtree(GroupNode *node, char *word, const int prefix_len)
{
    int cond;
    if(node == NULL)
    {
        node = group_alloc(word, prefix_len);
    }
    else if((cond = strncmp(word, node->group_name, prefix_len)) == 0)
    {
        addname(node->names, word);    
    }
    else if(cond < 0)
    {
        node->left = addtree(node->left, word, prefix_len);
    }
    else
    {
        node->right = addtree(node->right, word, prefix_len);
    }

    return node;
}

void print_group(NameNode *node)
{
    if(node == NULL) return;
    print_group(node->left);
    printf("\t%s\n", node->name);
    print_group(node->right);
}

void print_tree(GroupNode *node)
{
    if(node == NULL) return;
    print_tree(node->left);
    printf("%s:\n", node->group_name);
    print_group(node->names);
    puts("");
    print_tree(node->right);
}

void free_group(NameNode *node)
{
    if(node == NULL) return;
    free_group(node->left);
    free_group(node->right);
    free(node->name);
    free(node);
}

void free_tree(GroupNode *node)
{
    if(node == NULL) return;
    free_tree(node->left);
    free_tree(node->right);
    free_group(node->names);
    free(node->group_name);
    free(node);
}

int comp_str(const void *s1, const void *s2)
{
    const char *k = s1; 
    const char *e = *(const char **)s2;
    return strcmp(k, e);
}

bool iskeyword(char *str)
{
    return bsearch(str, keywords, ARRAY_LEN(keywords), sizeof(char*), comp_str) != NULL;
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

int next_id(char *buffer)
{
    int c;
    while(isspace(buffer[0] = c = getch()));
    if(c == EOF) return TOK_EOF;

    if(isalpha(c) || c == '_' || c == '#')
    {
        size_t i = 1;
        for(; i < BUFSIZE; ++i)
        {
            if(!isalnum(buffer[i] = c = getch()) && c != '_')
            {
                ungetch(c);
                break;
            }
        }
        buffer[i] = 0;
        if(iskeyword(buffer)) return next_id(buffer);
        if((c = getch()) == '(') return next_id(buffer);
        else ungetch(c);
        return TOK_ID;
    }
    if(c == '"' || c == '\'')
    {
        int quote = c;
        while((c = getch()) != quote && c != EOF)
        {
            if(c == '\\') getch();
        }
        return next_id(buffer);
    }
    else if(c == '/')
    {
        int d;
        if((d = getch()) == '*')
        {
            comment();
            return next_id(buffer);
        }
        else if(d == '/')
        {
            while((c = getch()) != '\n' && c != EOF);
            return next_id(buffer);
        }
        else
        {
            ungetch(d);
        }
    }
    else
    {
        return next_id(buffer);
    }

    return TOK_EOF;
}


int main(int argc, char **argv)
{
    int prefix_len = 6;
    if(argc > 1 && 0 == strncmp(argv[1], MATCH_FLAG, 2))
    {
        prefix_len = atoi(argv[1] + 2);
    }

    GroupNode *groups = NULL;
    
    char buffer[BUFSIZE];
    while(next_id(buffer) != TOK_EOF)
    {
        groups = addtree(groups, buffer, prefix_len);
    }

    print_tree(groups);
    free_tree(groups);

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
