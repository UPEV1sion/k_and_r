#include <assert.h>
#include <stdio.h>
#include <stddef.h>

//
// Created by escha on 24.12.25.
//
struct FileStats {
    size_t blanks, tabs, nl;
};

struct FileStats get_file_stats(const char *filename)
{
    struct FileStats stats = {0};

    FILE *f;
    assert((f = fopen(filename, "r")) && "Could not open file");
    int c;
    while((c = fgetc(f)) != EOF)
    {
        switch (c)
        {
            case ' ':  stats.blanks++; break;
            case '\t': stats.tabs++;   break;
            case '\n': stats.nl++;     break;
            default:                   break;
        }
    }

    return stats;
}

int main(int argc, char *argv[])
{
    assert(argc > 1 && "Usage ./8 <file>");

    struct FileStats stats = get_file_stats(argv[1]);
    printf("blanks: %zu\ntabs: %zu\nnl: %zu\n", stats.blanks, stats.tabs, stats.nl);

    return 0;
}