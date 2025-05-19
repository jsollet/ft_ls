#include "../includes/printf_parser.h"
#include "../includes/buffer.h"


intmax_t get_int_arg(va_list args, t_printf_length length) {
    if (length == ll) return va_arg(args, long long); // a voir
    if (length == l) return va_arg(args, long);
    if (length == hh) return (char)va_arg(args, int); 
    if (length == h) return (short)va_arg(args, int);
    if (length == z) return (size_t)va_arg(args, int);
    if (length == j) return va_arg(args, intmax_t);;
    if (length == t) return va_arg(args, ptrdiff_t);
    return va_arg(args, int);
}

uintmax_t get_uint_arg(va_list args, t_printf_length length) {
    if (length == ll) return va_arg(args, unsigned long long);
    if (length == l)  return va_arg(args, unsigned long);
    if (length == h)  return (unsigned short)va_arg(args, unsigned int);
    if (length == hh) return (unsigned char)va_arg(args, unsigned int);
    if (length == z)  return (uintmax_t)va_arg(args, size_t);
    if (length == j)  return (uintmax_t)va_arg(args, uintmax_t);
    if (length == t)  return (uintmax_t)va_arg(args, ptrdiff_t);
    return va_arg(args, unsigned int);
}

long double get_float_arg(va_list args, t_printf_length length) {
    if (length == L) {
        return va_arg(args, long double);
    }
    return (long double)va_arg(args, double);  // %f → promote float to double
}

void handle_sign(intmax_t value, t_conversion *conv, t_buffer *buf){
    if (value < 0) {
        buf_putchar(buf,  '-');
    } else if (conv->combined_flags & PLUS) {
        buf_putchar(buf,  '+');
    } else if (conv->combined_flags & SPACE) {
        buf_putchar(buf,  ' ');
    }
}


int get_flags(const char *format, size_t *i) {
    int flags = NONE;

    while ((format[*i] != '\0') && format[*i] && ft_strchr("-+ 0#", format[*i])){
        if (format[*i] == '-') flags |= MINUS;
        else if (format[*i] == '+') flags |= PLUS;
        else if (format[*i] == ' ') flags |= SPACE;
        else if (format[*i] == '0') flags |= ZERO;
        else if (format[*i] == '#') flags |= SHARP;
        (*i)++;
    }
    return flags;
}



bool is_valid_specifier(const char *format, size_t start) {
    if (format[start] == '%' && format[start + 1] == '%')
		return true;
	if (format[start] != '%') return false;
    else if (format[start + 1] == '\0') return false;
	size_t i = start + 1;

	i += skip_while(format + i, "-+ 0#", 0);

	while ((format[i] != '\0') && (ft_isdigit(format[i]) || format[i] == '*')) i++;

	if (format[i] == '.') {
		i++;
		while ((format[i] != '\0') && (ft_isdigit(format[i]) || format[i] == '*')) i++;
	}

	if (format[i] != '\0' &&(ft_strncmp(format + i, "hh", 2) == 0 || ft_strncmp(format + i, "ll", 2) == 0))
		i += 2;
	else if (format[i] != '\0' && ft_strchr(LENGTH_1, format[i]))
		i++;

	if (format[i] != '\0' && ft_strchr(SPECIFIERS, format[i]))
		return true;
	return false;
}



bool is_valid_specifier_and_parse(const char *format, size_t start, t_conversion *conv) {
	if (format[start] == '%' && format[start + 1] == '%')
    {
        conv->specifier = percent;
        return true;
    }
	conv->has_precision = false;
    	
    if (format[start] != '%') return false;
    else if (format[start + 1] == '\0') return false;
	size_t i = start + 1;

	// skip flags
    conv->combined_flags = get_flags(format, &i);

	// skip width
    conv->width.is_star = 0;
    conv->width.value = 0; 
    if ((format[i] != '\0') && format[i] == '*'){
        conv->width.is_star = 1;
        
        i++;
    } else {
        while ((format[i] != '\0') && ft_isdigit(format[i])) {
            conv->width.value = conv->width.value * 10 + (format[i] - '0');
            i++;
        }
    }

	// skip precision
    conv->precision.is_star = 0;
    conv->precision.value = 0; 
	if ((format[i] != '\0') && format[i] == '.') {
        conv->has_precision = true;
		i++;
        if ((format[i] != '\0') && format[i] == '*') {
            conv->precision.is_star = 1;
            i++;
        } else {
            conv->precision.value = 0;
            while ((format[i] != '\0') && ft_isdigit(format[i])) {
                conv->precision.value = conv->precision.value * 10 + (format[i] - '0');
                i++;
            }
        }
	}

	// skip length modifiers
    conv->length_mod = NONE_LENGTH;
    if ((format[i] != '\0') && ft_strncmp(format + i, "hh", 2) ==  0){
        conv->length_mod = hh;
        i += 2;
    } 
    else if (format[i] != '\0' && ft_strncmp(format + i, "ll", 2) == 0){
        conv->length_mod = ll;
        i += 2;
    } else if (format[i] != '\0' && ft_strchr(LENGTH_1, format[i])){
        switch (format[i]) {
            case 'h': conv->length_mod = h; break;
            case 'l': conv->length_mod = l; break;
            case 'j': conv->length_mod = j; break;
            case 'z': conv->length_mod = z; break;
            case 't': conv->length_mod = t; break;
            case 'L': conv->length_mod = L; break;
            default: break;
        }
        i++;
    }
		


	if (format[i] != '\0' && ft_strchr(SPECIFIERS, format[i])){
        conv->specifier = format[i];
       
    } else if (format[i] == '%'){
        conv->specifier = percent;
    } else {
        return false;
    }
    // normalisation 

    if ((conv->combined_flags & ZERO) && (conv->precision.value >= 0 || (conv->combined_flags & MINUS))) 
    {
        conv->combined_flags &= ~ZERO;
    }

    if ((conv->combined_flags & PLUS) && (conv->combined_flags & SPACE)) {
        conv->combined_flags &= ~SPACE;
    }
    if (!(conv->specifier == 'o' || conv->specifier == 'x' ||
        conv->specifier == 'X' || conv->specifier == 'a' ||
        conv->specifier == 'A' || conv->specifier == 'e' ||
        conv->specifier == 'E' || conv->specifier == 'f' ||
        conv->specifier == 'g' || conv->specifier == 'G')) {
        conv->combined_flags &= ~SHARP;
    }
	return true;
}