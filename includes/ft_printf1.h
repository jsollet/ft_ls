#ifndef FT_PRINTF1_H
# define FT_PRINTF1_H
#include "./allheaders.h"
#include "./buffer.h"
#include "./string_utils.h"
#include "./format_utils.h"

int		ft_printf1(const char *fmt, ...);
int		ft_vprintf(const char *fmt, va_list args);

#endif