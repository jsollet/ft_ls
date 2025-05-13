#include "../includes/format_utils.h"
#include "../includes/buffer.h"


intmax_t get_signed_arg(va_list *args, t_printf_length length) {
    if (length == ll) return va_arg(*args, long long);
    if (length == l)  return va_arg(*args, long);
    if (length == hh) return (signed char)va_arg(*args, int);
    if (length == h)  return (short)va_arg(*args, int);
    if (length == j)  return va_arg(*args, intmax_t);
    if (length == t)  return va_arg(*args, ptrdiff_t);
    return va_arg(*args, int);
}

uintmax_t get_unsigned_arg(va_list args, t_printf_length length) {
    if (length == ll) return va_arg(args, unsigned long long);
    if (length == l)  return va_arg(args, unsigned long);
    if (length == hh) return (unsigned char)va_arg(args, unsigned int);
    if (length == h)  return (unsigned short)va_arg(args, unsigned int);
    if (length == j)  return va_arg(args, uintmax_t);
    if (length == z)  return va_arg(args, size_t);
    if (length == t)  return (uintmax_t)va_arg(args, ptrdiff_t);
    return va_arg(args, unsigned int);
}


int utoa_base(uintmax_t val, t_buffer *buf, int base, bool lower, t_format_number *format){
    const char *digits = lower ? "0123456789abcdef" : "0123456789ABCDEF";
    int i = 0;
    char tmp[64];

    if (val == 0) {
        buf_putchar(buf, '0');
        format->number_len = 1;
        return 1;
    }

    while (val > 0) {
        tmp[i++] = digits[val % base];
        val /= base;
    }

    for (int j = i - 1; j >= 0; --j) {
        buf_putchar(buf, tmp[j]);
    }

    format->number_len = i;
    return i;
}

int itoa_base(intmax_t val, t_buffer *buf, int base, bool lower, t_format_number *format) {
    const char *digits = lower ? "0123456789abcdef" : "0123456789ABCDEF";
    char tmp[64];
    int i = 0;
    bool is_negative = false;

    uintmax_t uval;

    if (val < 0) {
        is_negative = true;
        uval = (uintmax_t)(-(val + 1)) + 1;
    } else {
        uval = (uintmax_t)val;
    }

    if (uval == 0) {
        buf_putchar(buf, '0');
        format->number_len = 1;
        format->sign = is_negative;
        return 1;
    }

    while (uval > 0) {
        tmp[i++] = digits[uval % base];
        uval /= base;
    }

    for (int j = i - 1; j >= 0; --j) {
        buf_putchar(buf, tmp[j]);
    }

    format->number_len = i;
    format->sign = is_negative;
    return i;
}


int build_prefix(uintmax_t val, t_buffer *buf, t_conversion *conv , t_format_number *format){
    int len = 0;
    if (conv->specifier == 'o' && (conv->combined_flags & SHARP) && val != 0) {
        buf_putchar(buf, '0');
        format->prefix = ZERO_0;
        format->prefix_len = 1;
        return 1;
    } 
    if ((conv->specifier == 'x' || conv->specifier == 'X') && (conv->combined_flags & SHARP) && val != 0) {
        buf_putchar(buf, '0');
        buf_putchar(buf, conv->specifier);
        format->prefix_len = 2;
        format->prefix = (conv->specifier == 'x') ? ZERO_x : ZERO_X;
        return 2;
    }
    format->prefix = NO_PREFIX;
    format->prefix_len = 0;
    return len;
}

void emit_prefix(t_format_number *format, t_buffer *buf) {
    if (format->prefix == ZERO_0) {
        buf_putchar(buf, '0');
    } else if (format->prefix == ZERO_x) {
        buf_putstr(buf, "0x");
    } else if (format->prefix == ZERO_X) {
        buf_putstr(buf, "0X");
    }
}

void compute_padding_zero_old(t_conversion *conv, t_format_number *format) {
    if (conv->precision.is_star || conv->precision.value >= 0) {
        format->padding_zero = (conv->precision.value > format->number_len)
            ? conv->precision.value - format->number_len : 0;
    } else if ((conv->combined_flags & ZERO) && !(conv->combined_flags & MINUS)) {
        size_t total_len = format->prefix_len + format->number_len + (format->sign_char ? 1 : 0);
        format->padding_zero = (conv->width.value > (int)total_len)
            ? conv->width.value - total_len : 0;
    } else {
        format->padding_zero = 0;
    }
}

void compute_padding_zero(t_conversion *conv, t_format_number *format) {
    if (format->is_zero_value && conv->precision.value == 0) {
        format->number_len = 0;
        format->emit_number = false;
        format->padding_zero = 0;
        return;
    }


    if (conv->has_precision) {
        format->padding_zero = (conv->precision.value > format->number_len)
            ? conv->precision.value - format->number_len : 0;
    }
    else if ((conv->combined_flags & ZERO) && !(conv->combined_flags & MINUS)) {
        size_t total_len = format->prefix_len + format->number_len + (format->sign_char ? 1 : 0);
        format->padding_zero = (conv->width.value > (int)total_len)
            ? conv->width.value - total_len : 0;
    }
    else {
        format->padding_zero = 0;
    }
}




void emit_padding_zeros(t_format_number *format, t_buffer *buf) {
    buf_putnchar(buf, '0', format->padding_zero);
}

void compute_padding_space(t_conversion *conv, t_format_number *format) {
    size_t total_len = format->prefix_len + format->number_len
                     + format->padding_zero + (format->sign_char ? 1 : 0);

    if (conv->width.value >(int) total_len)
        format->padding_space = conv->width.value - total_len;
    else
        format->padding_space = 0;
}

void emit_padding_space(t_format_number *format, t_buffer *buf) {
    buf_putnchar(buf, ' ', format->padding_space);
}

void emit_sign(intmax_t value, t_conversion *conv, t_format_number *format, t_buffer *buf) {
    if (value < 0) {
        buf_putchar(buf, '-');
        format->sign_char = '-';
    } else if (conv->combined_flags & PLUS) {
        buf_putchar(buf, '+');
        format->sign_char = '+';
    } else if (conv->combined_flags & SPACE) {
        buf_putchar(buf, ' ');
        format->sign_char = ' ';
    } else {
        format->sign_char = 0;
    }
}

void emit_sign_u(uintmax_t value, t_conversion *conv, t_format_number *format, t_buffer *buf) {
    if (value < 0) {
        buf_putchar(buf, '-');
        format->sign_char = '-';
    } else if (conv->combined_flags & PLUS) {
        buf_putchar(buf, '+');
        format->sign_char = '+';
    } else if (conv->combined_flags & SPACE) {
        buf_putchar(buf, ' ');
        format->sign_char = ' ';
    } else {
        format->sign_char = 0;
    }
}

void    compute_sign(intmax_t value, t_conversion *conv, t_format_number *format){
    if (value < 0) {
        format->sign_char = '-';
    } else if (conv->combined_flags & PLUS) {
        format->sign_char = '+';
    } else if (conv->combined_flags & SPACE) {
        format->sign_char = ' ';
    } else {
        format->sign_char = 0;
    }
}

void    compute_sign_u(uintmax_t value, t_conversion *conv, t_format_number *format){
    (void)value;

    if (conv->combined_flags & PLUS) {
        format->sign_char = '+';
    } else if (conv->combined_flags & SPACE) {
        format->sign_char = ' ';
    } else {
        format->sign_char = 0;
    }
}

void emit_padding_after(t_conversion *conv, t_format_number *format, t_buffer *buf) {
    if (!(conv->combined_flags & MINUS))
        return;

    int content_len = format->prefix_len + format->number_len + format->padding_zero;
    if (format->sign_char)
        content_len += 1;

    if ((int)conv->width.value > content_len) {
        int padding = conv->width.value - content_len;
        buf_putnchar(buf, ' ', padding);
        format->padding_space = padding;
    } else {
        format->padding_space = 0;
    }
}

void emit_integer(intmax_t value, t_conversion *conv, t_buffer *buf) {
    t_format_number format = {0};

    t_buffer number_buf;
    char temp[32] = {0};
    buf_init_fixed(&number_buf, temp, sizeof(temp), BUF_TEXT);


    if (conv->specifier == 'd' || conv->specifier == 'i') {
        format.number_len = itoa_base((uintmax_t)value, &number_buf, 10, false, &format);
    } else if (conv->specifier == 'x' || conv->specifier == 'X') {
        format.number_len = utoa_base((uintmax_t)value, &number_buf, 16, conv->specifier == 'x', &format);
    } else if (conv->specifier == 'o') {
        format.number_len = utoa_base((uintmax_t)value, &number_buf, 8, false, &format);
    }

    format.is_zero_value = (value == 0);
    format.emit_number = true;

    build_prefix((uintmax_t)value, &number_buf, conv, &format);
    compute_sign(value, conv, &format);
    compute_padding_zero(conv, &format);
    compute_padding_space(conv, &format);

    if (!(conv->combined_flags & MINUS)) {
        if ((conv->combined_flags & ZERO) && conv->precision.value < 0) {
            emit_sign(value, conv, &format, buf);
            emit_prefix(&format, buf);
            emit_padding_zeros(&format, buf);
        } else {
            emit_padding_space(&format, buf);
            emit_sign(value, conv, &format, buf);
            emit_prefix(&format, buf);
            emit_padding_zeros(&format, buf);
        }
    } else {

        emit_sign(value, conv, &format, buf);
        emit_prefix(&format, buf);
        emit_padding_zeros(&format, buf);
    }
    

    buf_putstrn(buf, number_buf.data, format.number_len);
    if (conv->combined_flags & MINUS)
        emit_padding_after(conv, &format, buf);

}

void emit_unsigned_integer(uintmax_t value, t_conversion *conv, t_buffer *buf) {
    t_format_number format = {0};

    t_buffer number_buf;
    char temp[32] = {0};
    buf_init_fixed(&number_buf, temp, sizeof(temp), BUF_TEXT);

    if (conv->specifier == 'u' || conv->specifier == 'x' || conv->specifier == 'X' || conv->specifier == 'o') {
        int base = (conv->specifier == 'o') ? 8 : (conv->specifier == 'x' || conv->specifier == 'X') ? 16 : 10;
        bool lower = (conv->specifier == 'x');
        format.number_len = utoa_base(value, &number_buf, base, lower, &format);
    } 

    build_prefix(value, &number_buf, conv, &format);
    compute_sign_u(value, conv, &format);
    compute_padding_zero(conv, &format);
    compute_padding_space(conv, &format);

    if (!(conv->combined_flags & MINUS)) {
        if ((conv->combined_flags & ZERO) && conv->precision.value < 0) {

            emit_sign_u(value, conv, &format, buf);
            emit_prefix(&format, buf);
            emit_padding_zeros(&format, buf);
        } else {
            emit_padding_space(&format, buf);
            emit_sign_u(value, conv, &format, buf);
            emit_prefix(&format, buf);
            emit_padding_zeros(&format, buf);
        }
    } else {
        emit_sign_u(value, conv, &format, buf);
        emit_prefix(&format, buf);
        emit_padding_zeros(&format, buf);
    }
    

    buf_putstrn(buf, number_buf.data, format.number_len);

    if (conv->combined_flags & MINUS)
        emit_padding_after(conv, &format, buf);
}

void build_zeros_leading(t_buffer *buf, t_conversion *conv, t_format_number *format) {
    if (conv->precision.is_star || conv->precision.value > 0) {
        if (conv->precision.value > format->number_len)
            format->padding_zero = conv->precision.value - format->number_len;
        else
            format->padding_zero = 0;
    } else if ((conv->combined_flags & ZERO) && !(conv->combined_flags & MINUS)) {
        int total_len = format->prefix_len + format->number_len + (format->sign_char ? 1 : 0);
        if ((int)conv->width.value > total_len)
            format->padding_zero = conv->width.value - total_len;
        else
            format->padding_zero = 0;
    } else {
        format->padding_zero = 0;
    }
    if (format->padding_zero > 0){
        buf_putnchar(buf, '0', format->padding_zero);
    } 
}



int format_unsigned_base(uintmax_t val, char *buf, int base, bool lower, t_conversion *conv) {
    const char *digits = lower ? "0123456789abcdef" : "0123456789ABCDEF";
    char tmp[65];
    int i = 0;

    if (val == 0) {
        tmp[i++] = '0';
    } else {
        while (val > 0) {
            tmp[i++] = digits[val % base];
            val /= base;
        }
    }

    
    int prefix_len = 0;
    if (conv && (conv->combined_flags & SHARP)) {
        if (base == 16) {
            buf[0] = '0';
            buf[1] = lower ? 'x' : 'X';
            prefix_len = 2;
        } else if (base == 8 && tmp[i - 1] != '0') {
            buf[0] = '0';
            prefix_len = 1;
        }
    }

    
    for (int j = 0; j < i; ++j)
        buf[prefix_len + j] = tmp[i - 1 - j];

    return prefix_len + i;
}


// string...
void    prepare_format_string(const char *str, t_format_string *format,  t_conversion *conv){
    if (!str){
        format->str = "(null)";
        format->is_null = true;
    } else {
        format->str = str;
        format->is_null = false;
    }

    format->original_len = ft_strlen(format->str);
    format->precision_len = format->original_len;


    if (conv->has_precision && conv->precision.value >= 0) {
        if (conv->precision.value < format->original_len)
            format->precision_len = conv->precision.value;
        else
            format->precision_len = format->original_len;
    }

    int display_len = format->precision_len;

    int total_padding = (conv->width.value > display_len)
        ? conv->width.value - display_len
        : 0;


    if (conv->combined_flags & MINUS){
        format->padding_left = 0;
        format->padding_right = total_padding;
    } else {
        format->padding_left = total_padding;
        format->padding_right = 0;
    }
}

void    emit_string(const char *str, t_conversion *conv, t_buffer *buf){
    t_format_string format = {0};
    prepare_format_string(str, &format, conv);
    if (conv->combined_flags & MINUS) {
        // left-justify: text then padding
        buf_putstrn(buf, format.str, format.precision_len);
        buf_putnchar(buf, ' ', format.padding_right);
    } else {
        // right-justify: padding then text
        buf_putnchar(buf, ' ', format.padding_left);
        buf_putstrn(buf, format.str, format.precision_len);
    }
}


// char
void prepare_format_char( int c, t_format_char *fmt, t_conversion *conv) {
    fmt->c = (char)c;
    fmt->is_null_char = (fmt->c == '\0');

    int total_padding = conv->width.value > 1 ? conv->width.value - 1 : 0;
    
    if (conv->combined_flags & MINUS) {
        fmt->padding_left = 0;
        fmt->padding_right = total_padding;
    } else {
        fmt->padding_left = total_padding;
        fmt->padding_right = 0;
    }
    
}

void emit_char(int c, t_conversion *conv, t_buffer *buf) {
    t_format_char fmt;
    prepare_format_char(c, &fmt, conv);
    buf_putnchar(buf, ' ', fmt.padding_left);
    buf_putnchar(buf, fmt.c, 1);
    buf_putnchar(buf, ' ', fmt.padding_right);
}