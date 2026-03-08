#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#define BUFSIZE 1024
#define shift_args(argc, argv) ((argc)--, *(argv)++)

void filecopy(const int fd1, const int fd2)
{
    char buffer[1024];
    ssize_t rc;

    while((rc = read(fd1, buffer, sizeof buffer)) > 0)
    {
        ssize_t total = 0;

        while(total < rc)
        {
            ssize_t wc = write(fd2, buffer, rc);
            if(wc < 0) return;
            total += wc;
        }
    }
}

int main(int argc, char **argv)
{
    const char *program = shift_args(argc, argv);
    if(argc == 0)
    {
        filecopy(0, 1);
    }
    else
    {
        while(argc > 0)
        {
            const char *path = shift_args(argc, argv);
            int fd;
            if((fd = open(path, O_RDONLY)) < 0)
            {
                fprintf(stderr, "%s: can't open %s\n: %s", program, path, strerror(errno));
                return 1;
            }

            filecopy(fd, 1);
            close(fd);
        }
    }


    return 0;
}
