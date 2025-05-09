#include <stddef.h>
#include <stdint.h>
#include "../includes/utils.h"





void	*ft_memcpy_fast(void *dest, const void *source, size_t n)
{
	if (!dest && !source)
		return (NULL);

	unsigned char			*dst = dest;
	const unsigned char		*src = source;

	// Optimisation par blocs de size_t
	if (((uintptr_t)dest % sizeof(size_t) == 0) &&
		((uintptr_t)source % sizeof(size_t) == 0))
	{
		size_t *dst_word = (size_t *)dest;
		const size_t *src_word = (const size_t *)source;

		while (n >= sizeof(size_t))
		{
			*dst_word++ = *src_word++;
			n -= sizeof(size_t);
		}
		dst = (unsigned char *)dst_word;
		src = (const unsigned char *)src_word;
	}

	// Finir octet par octet
	while (n--)
		*dst++ = *src++;

	return dest;
}




void *ft_memcpy_faster(void *dest, const void *src, size_t n)
{
	if (!dest && !src)
		return NULL;

	unsigned char *d = dest;
	const unsigned char *s = src;

	// Si les pointeurs sont bien alignés
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

	// Reste octet par octet
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
	file->valid = false; // a voir
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