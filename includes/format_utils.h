#ifndef FORMAT_UTILS_H
# define  FORMAT_UTILS_H

#include "./allheaders.h"
#include <errno.h>
#include "../libft/includes/libft.h"

#include "./string_utils.h"
#include "./printf_parser.h"
#include "./buffer.h"



typedef enum e_prefix {
	NO_PREFIX,
	ZERO_0,
	ZERO_x,
	ZERO_X
} t_prefix;

typedef struct s_format_number {
	int         number_len;       // Longueur du nombre en caractères (sans préfixe, padding ni signe)
	char        sign_char;        // '-', '+', ' ', ou '\0' si pas de signe
	t_prefix 	prefix;           // NULL, "0", "0x", "0X"
	int     	prefix_len;
	size_t		padding_zero;     // Nombre de '0' à insérer (précision ou '0' flag)
	size_t		padding_space;    // Nombre de ' ' à insérer (pour width)
	bool		sign;
	bool		emit_number;
	bool		is_zero_value;
} t_format_number;

typedef struct s_format_string {
	int			original_len; // le ft_strlen(str)
	int			precision_len; 
	size_t		padding_left;
	size_t		padding_right;
	bool		is_null;
	const char	*str;
} t_format_string;

typedef struct s_format_char {
    char    c;
    size_t  padding_left;
    size_t  padding_right;
    bool    is_null_char;
} t_format_char;

int		utoa_base(uintmax_t val, t_buffer *buf, int base, bool lowercase, t_format_number *format);
int		itoa_base(intmax_t val, t_buffer *buf, int base, bool lower, t_format_number *format);
int		build_prefix(uintmax_t val, t_buffer *buf, t_conversion *conv, t_format_number *format);
void	emit_prefix(t_format_number *format, t_buffer *buf);
void	compute_padding_zero(t_conversion *conv, t_format_number *format);
void	emit_padding_zeros(t_format_number *format, t_buffer *buf);
void	build_zeros_leading(t_buffer *buf, t_conversion *conv, t_format_number *format);
void	compute_padding_space(t_conversion *conv, t_format_number *format);
void	emit_padding_space(t_format_number *format, t_buffer *buf);
void	emit_sign(intmax_t value, t_conversion *conv, t_format_number *format, t_buffer *buf);
void 	emit_sign_u(uintmax_t value, t_conversion *conv, t_format_number *format, t_buffer *buf);
void	emit_padding_after(t_conversion *conv, t_format_number *format, t_buffer *buf);
void	compute_sign(intmax_t value, t_conversion *conv, t_format_number *format);
void    compute_sign_u(uintmax_t value, t_conversion *conv, t_format_number *format);

void	emit_integer(intmax_t value, t_conversion *conv, t_buffer *buf);
void	emit_unsigned_integer(uintmax_t value, t_conversion *conv, t_buffer *buf);

void    emit_string(const char *str, t_conversion *conv, t_buffer *buf);
void 	emit_char( int c, t_conversion *conv, t_buffer *buf);

uintmax_t get_unsigned_arg(va_list args, t_printf_length length);

#endif