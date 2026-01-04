#include <stdio.h>
#include <ctype.h>

#define LINELEN 80

void fold(void) {
    int c;
    int pos = 0;
    int last_blank = -1;
    char line[LINELEN];

    while ((c = getchar()) != EOF) {
        line[pos] = c;

        if (isblank(c)) {
            last_blank = pos;
        }

        pos++;

        if (pos == LINELEN) {
            if (last_blank >= 0) {
                for (int i = 0; i < last_blank; i++) putchar(line[i]);
                putchar('\n');

                int remaining = pos - (last_blank + 1);
                for (int i = 0; i < remaining; i++) line[i] = line[last_blank + 1 + i];

                pos = remaining;
                last_blank = -1;
                for (int i = 0; i < pos; i++) if (isblank(line[i])) last_blank = i;
            } else {
                for (int i = 0; i < LINELEN - 1; i++) putchar(line[i]);
                putchar('-');
                putchar('\n');
                line[0] = line[LINELEN - 1];
                pos = 1;
                last_blank = -1;
            }
        }

        if (c == '\n') {
            for (int i = 0; i < pos; i++) putchar(line[i]);
            pos = 0;
            last_blank = -1;
        }
    }

    for (int i = 0; i < pos; i++) putchar(line[i]);
}

int main(void) {
    fold();
    return 0;
}
