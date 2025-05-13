#ifndef BUFFER_H
# define BUFFER_H
#include "./allheaders.h"

#define DEFAULT_FD_BUF_SIZE 100
#define INITIAL_CAPACITY 4

typedef enum e_buf_mode {
	BUF_STDOUT,             // printf
	BUF_STRING,             // snprintf
	BUF_DYNAMIC_STRING,     // asprintf
    BUF_TO_FD, 
	BUF_TO_MEMORY
}	t_buf_mode;

typedef enum e_buf_content_type {
    BUF_TEXT,
    BUF_BINARY,
    BUF_UTF8,
    BUF_JSON,
    BUF_CUSTOM // pour du traitement spécial
} t_buf_content_type;

typedef struct s_buffer {
	char	*data;
	size_t	capacity;
	size_t	length;
	int			fd;       // pour BUF_STDOUT
	t_buf_mode	mode;     // mode d'écriture
    t_buf_content_type  content_type;
	int			error;    // flag d'erreur d'écriture
	bool	is_fixed;
	bool	is_dynamic; 
}	t_buffer;

int buf_init_fd(t_buffer *buf, int fd, t_buf_content_type type);
int buf_init_fixed(t_buffer *buf, char *prealloc, size_t size, t_buf_content_type type); // non teste
int buf_init_dynamic(t_buffer *buf, t_buf_content_type type);

int	buf_flush(t_buffer *buf); // écrit dans fd si besoin
int	buf_putstr(t_buffer *buf, const char *str);
int	buf_putchar(t_buffer *buf, char c);
int	buf_putnchar(t_buffer *buf, char c, size_t n);
int buf_putstrn(t_buffer *buf, const char *str, size_t n);
int	buf_ensure_space(t_buffer *buf, size_t add_len); // realloc si dynamique 

int buf_grow(t_buffer *buf, size_t required_space);
//void *ft_memcpy_faster(void *dest, const void *src, size_t n);
#endif