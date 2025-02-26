#ifndef TYPES_H
# define TYPES_H
# include <stddef.h>
# include <stdbool.h>
#include <sys/ioctl.h>

typedef struct s_flags
{
    bool    bigR;
    bool    r;
    bool    a;
    bool    l;
    bool    t;

    // bonus a voir...
    bool    u;
    bool    f;
    bool    g;
    bool    d;

}   t_flags;

typedef struct s_term
{
    struct winsize  w;
    int             term_width;
    int             col_width;
    size_t          count;
    size_t          max_len;
}   t_term;

#endif