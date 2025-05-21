#include "../includes/buffer.h"
#include "../includes/ft_printf1.h"

int ft_printf1(const char *fmt, ...) {
    va_list args;
    int printed = 0;
    va_start(args, fmt);
    printed = ft_vprintf(fmt, args);
    va_end(args);
    return printed;
}

int ft_vprintf(const char *fmt, va_list args) {
	t_buffer buf;
	char *res;
    buf_init_fd(&buf, STDOUT_FILENO,  BUF_TEXT);
	size_t pos = 0;
	t_format_block block;
	
	while (fmt[pos]) {
        if (fmt[pos] != '%') {
            res = find_text_block(fmt, &pos, &block);
            buf_putstrn(&buf, res, block.end - block.start +1);
            free(res);
        }
        else {
			t_conversion conv = {0};
            res = find_format_block(fmt, &pos, &block);
			is_valid_specifier_and_parse(res, 0, &conv);

            if (conv.width.is_star) {
                conv.width.value = va_arg(args, int);
                if (conv.width.value < 0) {
                    conv.width.value = -conv.width.value;
                    conv.combined_flags |= MINUS;
                }
            }
            if (conv.precision.is_star) {
                conv.precision.value = va_arg(args, int);
                if (conv.precision.value < 0) {
                    conv.has_precision = false;
                }
            }
			if (conv.specifier == 'i' || conv.specifier == 'd'){
				intmax_t value = get_int_arg(args, conv.length_mod);
				t_buffer buf_temp ;
   				char temp[1024];
    			buf_init_fixed(&buf_temp, temp, sizeof(temp), BUF_TEXT);
				emit_integer(value, &conv, &buf_temp);
				buf_putstrn(&buf, buf_temp.data, buf_temp.length);
				
			} else if (conv.specifier == 'u' || conv.specifier == 'x' || conv.specifier == 'X' || conv.specifier == 'o'){
                uintmax_t value = get_unsigned_arg(args, conv.length_mod);
    			t_buffer buf_temp;
    			char temp[1024];
    			buf_init_fixed(&buf_temp, temp, sizeof(temp), BUF_TEXT);
    			emit_unsigned_integer(value, &conv, &buf_temp);
    			buf_putstrn(&buf, buf_temp.data, buf_temp.length);
			} else if (conv.specifier == percent){
                buf_putchar(&buf, '%');
            } else if (conv.specifier == 's') {
				char *str = va_arg(args, char *);
                t_buffer buf_temp;
                buf_init_dynamic(&buf_temp, BUF_TEXT);
                emit_string(str, &conv, &buf_temp);
                buf_putstrn(&buf, buf_temp.data, buf_temp.length);
                free(buf_temp.data);
			} else if (conv.specifier == 'c'){
                char c = va_arg(args, int);
                t_buffer buf_temp;
                char temp[4096];
    			buf_init_fixed(&buf_temp, temp, sizeof(temp), BUF_TEXT);
                emit_char(c, &conv, &buf_temp);
                buf_putstrn(&buf, buf_temp.data, buf_temp.length); 
                }
            free(res);
        } 
    }
	ssize_t _unused = write(STDOUT_FILENO, buf.data, buf.length);
	(void)_unused;
    if (buf.is_dynamic){
        free(buf.data);
    }
    return buf.length;
}