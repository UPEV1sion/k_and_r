#define _DEFAULT_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

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

void* xmalloc(unsigned nbytes)
{
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

int main(void)
{
    char test[] = "lorem ipsum dolor";
    char *dup = xmalloc(sizeof(test));
    for(size_t i = 0; i < sizeof(test); ++i)
    {
        dup[i] = test[i];
    }

    char *arr = xcalloc(42, 1);
    bool all_zero = true;
    for(size_t i = 0; i < 42; ++i)
    {
        if(arr[i] != 0)
        {
            all_zero = false;
            break;
        }
    }
    puts(all_zero ? "true" : "false");
    xfree(arr);

    puts(dup);
    xfree(dup);

    return 0;
}
