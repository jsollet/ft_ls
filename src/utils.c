
#include "../includes/utils.h"


char	*ft_strjoin_multiple(const char *first, ...){
	if (!first)
		return NULL;
	size_t total_length = 0;
	const char *current_str = first;
	va_list args;
	va_start(args, first);
	while (current_str) {
		total_length += ft_strlen(current_str);
		current_str = va_arg(args, const char *);
	}
	va_end(args);

	char *result = malloc(total_length + 1);
	if (!result)
		return NULL;
	
	va_start(args, first);
	char *current_pos = result;
	current_str = first;
	while (current_str) {
		size_t len = ft_strlen(current_str);
		ft_memcpy(current_pos, current_str, len);
		current_pos += len;
		current_str = va_arg(args, const char *);
	}
	va_end(args);
	
	*current_pos = '\0';
	return result;
}

void *ft_memcpy_faster(void *dest, const void *src, size_t n)
{
	if (!dest && !src)
		return NULL;

	unsigned char *d = dest;
	const unsigned char *s = src;

	if (((uintptr_t)d % sizeof(size_t) == 0) && ((uintptr_t)s % sizeof(size_t) == 0)) {
		size_t *dw = (size_t *)d;
		const size_t *sw = (const size_t *)s;

		while (n >= 4 * sizeof(size_t)) {
			dw[0] = sw[0];
			dw[1] = sw[1];
			dw[2] = sw[2];
			dw[3] = sw[3];
			dw += 4;
			sw += 4;
			n -= 4 * sizeof(size_t);
		}
		while (n >= sizeof(size_t)) {
			*dw++ = *sw++;
			n -= sizeof(size_t);
		}
		d = (unsigned char *)dw;
		s = (const unsigned char *)sw;
	}
	while (n--)
		*d++ = *s++;

	return dest;
}

t_fileData	*malloc_fileData(void)
{
	t_fileData *file;
	file = malloc(sizeof(t_fileData));
	if (!file)
		return NULL;
	ft_bzero(file, sizeof(t_fileData));
	file->valid = true;
	return (file);
}

void clean_path(char *path) {
	char *src = path, *dst = path;
	int was_slash = 0;

	while (*src) {
		if (*src == '/') {
			if (!was_slash) {
				*dst++ = *src;
				was_slash = 1;
			}
		} else {
			*dst++ = *src;
			was_slash = 0;
		}
		src++;
	}
	*dst = '\0';
}

int	is_printable(const char *buf, ssize_t size)
{
	for (ssize_t i = 0; i < size; i++) {
		if (!ft_isprint((unsigned char)buf[i]) && buf[i] != '\n' && buf[i] != '\t')
			return (0);
	}
	return (1);
}

void	print_xattr_value(const char *buf, ssize_t size)
{
	if (is_printable(buf, size)) {
		ft_printf1("%.*s\n", (int)size, buf);
	} else {
		ft_printf1("\n\t    ");
		for (ssize_t i = 0; i < size; i++) {
			ft_printf("%02x ", (unsigned char)buf[i]);
			if ((i + 1) % 16 == 0)
				ft_printf1("\n\t    ");
		}
		if (size % 16 != 0)
			ft_printf1("\n");
	}
}