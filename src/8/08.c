#define _DEFAULT_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#define MAXBYTES (unsigned int) 1000000

typedef long Align;
typedef union header Header;
union header {
    struct {
        union header *ptr;
        unsigned size;
    } s;
    Align x;
};

#define NALLOC 1024
static Header base;
static Header *freep = NULL;

static Header* morecore(unsigned nu);

void xfree(void *ap)
{
    if(ap == NULL) return;

    Header *bp = (Header *) ap - 1;

    Header *p;
    for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
    {
        if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
        {
            break;
        }
    }

    if(bp + bp->s.size == p->s.ptr)
    {
        bp->s.size += p->s.ptr->s.size;
        bp->s.ptr = p->s.ptr->s.ptr;
    }
    else
    {
        bp->s.ptr = p->s.ptr;
    }

    if(p + p->s.size == bp)
    {
        p->s.ptr = bp->s.ptr;
        p->s.size += bp->s.size;
    }
    else
    {
        p->s.ptr = bp;
    }

    freep = p;
}

void bfree(void *p, unsigned n)
{
    if(n < sizeof(Header)) return;

    if(freep == NULL)
    {
        base.s.ptr = freep = &base;
        base.s.size = 0;
    }

    Header *h = p;
    h->s.size = n / sizeof(Header);
    if(h->s.size < 2) return;

    xfree((void *) (h + 1));
}

void* xmalloc(unsigned nbytes)
{
    if(nbytes == 0 || nbytes > MAXBYTES) return NULL;

    const unsigned nunits = (nbytes + sizeof(Header) - 1)/sizeof(Header) + 1;
    Header *prevp;
    if((prevp = freep) == NULL)
    {
        base.s.ptr = freep = prevp = &base;
        base.s.size = 0;
    }

    for(Header *p = prevp->s.ptr; ; prevp = p, p = p->s.ptr)
    {
        if(p->s.size >= nunits)
        {
            if(p->s.size == nunits)
            {
                prevp->s.ptr = p->s.ptr;
            }
            else
            {
                p->s.size -= nunits;
                p += p->s.size;
                p->s.size = nunits;
            }

            freep = prevp;
            return (void *) (p + 1);
        }

        if(p == freep)
        {
            if((p = morecore(nunits)) == NULL)
            {
                return NULL;
            }
        }
    }

    return NULL;
}

void* xcalloc(const unsigned n, const unsigned size)
{
    const unsigned nbytes = n * size;
    char *mem = xmalloc(nbytes);

    memset(mem, 0, nbytes);

    return mem;
}

static Header* morecore(unsigned nu)
{
    if(nu < NALLOC) nu = NALLOC;

    char *cp = sbrk(nu * sizeof(Header));
    if(cp == (char *) - 1) return NULL;

    Header *up = (Header *) cp;
    up->s.size = nu;
    xfree((void *) (up + 1));

    return freep;
}

#define BUFSIZE 4096
static char buffer[BUFSIZE];

int main(void)
{
    bfree(buffer, BUFSIZE);

    void *p1 = xmalloc(1000);
    void *p2 = xmalloc(3000);
    void *p3 = xmalloc(2000);

    printf("buf=%p, p1=%p, p2=%p, p3=%p\n", buffer, p1, p2, p3);

    return 0;
}
