#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/dir.h>
 
#define EOF (-1)
#define PATH_MAX 1024
#define NAME_MAX 14
#define OPEN_MAX 20
#define DIRSIZ 14

typedef struct {
    unsigned read  : 1;
    unsigned write : 1;
    unsigned unbuf : 1;
    unsigned eof   : 1;
    unsigned err   : 1;
} flag_t;

typedef struct iobuf {
    int cnt;        // chars left
    char *ptr;      // next char position
    char *base;     // location of buffer
    flag_t flag;    // file access mode
    int fd;         // file descriptor
} file_t;

typedef struct {
    long ino;
    char name[NAME_MAX + 1];
} Dirent;

typedef struct {
    int fd;
    Dirent d;
} DIRECTORY;

file_t iob[OPEN_MAX] = {
    {0, (char *) 0, (char *) 0, {.read = 1},              0},
    {0, (char *) 0, (char *) 0, {.write = 1},             1},
    {0, (char *) 0, (char *) 0, {.write = 1, .unbuf = 1}, 2},
};


#define standard_in  (&iob[0])
#define standard_out (&iob[1])
#define standard_err (&iob[2])

int fillbuf(file_t *);
int flushbuf(int, file_t *);

#define feof(p)   ((p)->flag.eof != 0)
#define ferror(p) ((p)->flag.err != 0)
#define fileno(p) ((p)->fd)

#define getc(p) (--(p)->cnt >= 0 ? (unsigned char) *(p)->ptr++ : fillbuf(p))
#define putc(x, p) (--(p)->cnt >= 0 ? *(p)->ptr++ = (x) : flushbuf((x), p))

#define getchar() getc(standard_in)
#define putchar(x) putc((x), standard_out)

#define PERMS 0666

int fillbuf(file_t *fp)
{
    if(fp->flag.read == 0 || fp->flag.eof == 1 || fp->flag.err == 1) 
        return EOF;

    const int bufile_size = (fp->flag.unbuf == 1) ? 1 : BUFSIZ;
    if(fp->base == NULL)
    {
            if((fp->base = malloc(bufile_size)) == NULL)
            {
                return EOF;
            }
    }

    fp->ptr = fp->base;
    fp->cnt = read(fp->fd, fp->ptr, bufile_size);
    if(--fp->cnt < 0)
    {
        if(fp->cnt == -1)
        {
            fp->flag.eof = 1;
        }
        else
        {
            fp->flag.err = 1;
        }

        fp->cnt = 0;
        return EOF;
    }
    
    return (unsigned char) *fp->ptr++;
}

int flushbuf(int x, file_t *fp)
{
    if(fp < iob || fp >= iob + OPEN_MAX) return EOF;
    if(fp->flag.write == 0 || fp->flag.err == 1) return EOF;

    const int bufile_size = (fp->flag.unbuf == 1) ? 1 : BUFSIZ;
    if(fp->base == NULL)
    {
        if((fp->base = malloc(bufile_size)) == NULL)
        {
            fp->flag.err = 1;
            return EOF;
        }
    }
    else
    {
        const size_t chars_left = fp->ptr - fp->base;
        size_t written = 0;
        while(written < chars_left)
        {
            const ssize_t wc = write(fp->fd, fp->base + written, chars_left - written);
            if(wc <= 0)
            {
                fp->flag.err = 1;
                return EOF;
            }
            written += wc;
        }
    }

    fp->ptr = fp->base;
    fp->cnt = bufile_size - 1;
    *fp->ptr++ = x;

    return x;
}


file_t* file_open(const char *name, char *mode)
{
    if(*mode != 'r' && *mode != 'w' && *mode != 'a') return NULL;

    file_t *fp;
    for(fp = iob; fp < iob + OPEN_MAX; fp++)
    {
        if(fp->flag.read == 0 && fp->flag.write == 0)
            break;
    }

    if(fp >= iob + OPEN_MAX) return NULL;
    
    int fd;
    if(*mode == 'w') 
    {
        fd = creat(name, PERMS);
        fp->flag.write = 1;
    }
    else if(*mode == 'a')
    {
        if((fd = open(name, O_WRONLY, 0)) == -1)
        {
            fd = creat(name, PERMS);
        }
        lseek(fd, 0L, 2);
        fp->flag.write = 1;
    }
    else
    {
        fd = open(name, O_RDONLY, 0);
        fp->flag.read = 1;
    }

    if(fd == -1) return NULL;
    
    fp->fd = fd;
    fp->cnt = 0;
    fp->base = NULL;
     
    return fp;
}

int file_flush(file_t *fp)
{
    if(fp < iob || fp >= iob + OPEN_MAX) return EOF;
    if(fp->flag.err == 1) return EOF;

    int wc = 0;
    if(fp->flag.write == 1)
    {
        wc = flushbuf(0, fp);
    }

    fp->ptr = fp->base;
    fp->cnt = (fp->flag.unbuf == 1) ? 1 : BUFSIZ;

    return wc;
}

int file_close(file_t *fp)
{
    if(fp < iob || fp >= iob + OPEN_MAX) return EOF;
    const int wc = file_flush(fp);
    free(fp->base);
    close(fp->fd);

    return wc;
}

int file_seek(file_t *fp, long offset, int origin)
{
    if(fp < iob || fp > iob + OPEN_MAX) return -1;

    if(fp->flag.unbuf == 0)
    {
        if(fp->flag.read == 1)
        {
            fp->cnt = 0;
            fp->ptr = fp->base;
        }
        else if(fp->flag.write == 1)
        {
            file_flush(fp);
        }
    }

    return lseek(fp->fd, offset, origin) < 0;
}

DIRECTORY* opendirectoryectory(char *directoryname)
{
    int fd;
    struct stat stbuf;
    DIRECTORY *dp;

    if((fd = open(directoryname, O_RDONLY, 0)) == -1
            || fstat(fd, &stbuf) == -1
            || !S_ISDIR(stbuf.st_mode)
            || (dp = malloc(sizeof(DIRECTORY))) == NULL)
        return NULL;

    dp->fd = fd;
    return dp;
}

Dirent* readdirectory(DIRECTORY *dp)
{
    struct dirent dirbuf;
    static Dirent d;

    while(read(dp->fd, (char*) &dirbuf, sizeof(dirbuf)) == sizeof(dirbuf))
    {
        if(dirbuf.d_ino == 0) continue;

        d.ino = dirbuf.d_ino;
        strncpy(d.name, dirbuf.d_name, DIRSIZ);
        d.name[DIRSIZ] = 0;
        return &d;
    }

    return NULL;
}

void closedirectory(DIRECTORY *dp)
{
    if(dp)
    {
        close(dp->fd);
        free(dp);
    }
}

void directorywalk(char *directory, void (*fcn)(char *))
{
    DIRECTORY *dfd;
    if((dfd = opendirectoryectory(directory)) == NULL)
    {
        fprintf(stderr, "directorywalk: can't opne %s\n", directory);
        return;
    }

    char name[PATH_MAX];
    Dirent *dp;
    while((dp = readdirectory(dfd)) != NULL)
    {
        if(0 == strcmp(dp->name, ".") || 0 == strcmp(dp->name, "..")) continue;

        if(strlen(directory) + strlen(dp->name) + 2 > sizeof(name))
        {
            fprintf(stderr, "directorywalk: name %s %s too long\n", directory, dp->name);
        }
        else
        {
            sprintf(name, "%s/%s", directory, dp->name);
            fcn(name);
        }
    }

    closedirectory(dfd);
}

void file_size(char *name)
{
    struct stat stbuf;
    if(stat(name, &stbuf) == -1)
    {
        fprintf(stderr, "file_size: can't access %s\n", name);
        return;
    }

    if(S_ISDIR(stbuf.st_mode))
    {
        directorywalk(name, file_size);
    }

    printf("%8ld %s\n", stbuf.st_size, name);
}

int main(void)
{
    file_t *f = file_open("Makefile", "r");
    const char c = getc(f); 
    putchar(c);

    file_close(f);


    return 0;
}
