#ifndef PRINTF_PARSER_H
# define  PRINTF_PARSER_H
#include "./allheaders.h"
#include <errno.h>
#include "../libft/includes/libft.h"
#include "./string_utils.h"
#include "./buffer.h"

#define TOTAL_SPECIFIERS "fcspdiuxXeEgGo%"
#define SPECIFIERS  "fcspdiuxXeEgGo"
#define LENGTH_1 "hljztL"

typedef enum e_printf_flags{
    NONE = 0,
    MINUS = 1,
    PLUS = 2,
    SPACE = 4,
    ZERO = 8,
    SHARP = 16
} t_printf_flags;

typedef enum e_printf_specifier{
    // "cspdiuxX%" + f (a voir)
    c, s,p,d,i,u,x,X,f, e, E, g, G, percent, o
} t_printf_specifier;

typedef enum e_printf_length{
    NONE_LENGTH,
    hh,
    ll,
    h,
    l,
    L,
    z,
    j,
    t
} t_printf_length;

typedef struct s_width_precision {
    int value;  // Valeur entière
    char is_star;  // Indicateur pour '*' (astérisque)
} t_width_precision;



typedef struct s_conversion {
    t_printf_specifier  specifier;   // Le type de spécificateur (%d, %s, etc.)

    int                 combined_flags;
    t_printf_length     length_mod;
    t_width_precision   width;     // La largeur, soit un entier soit '*'
    t_width_precision   precision;
    bool                has_precision; 
} t_conversion;

typedef void (*t_formatter)(va_list, t_conversion*, t_buffer*);



int     get_flags(const char *format, size_t *i);

bool    is_valid_specifier(const char *format, size_t start);
bool    is_valid_specifier_and_parse(const char *format, size_t start, t_conversion *conv);



intmax_t get_int_arg(va_list args, t_printf_length length);
uintmax_t get_uint_arg(va_list args, t_printf_length length);
long double get_float_arg(va_list args, t_printf_length length);

void handle_sign(intmax_t value, t_conversion *conv, t_buffer *buf);


#endif