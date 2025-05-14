#include "../includes/buffer.h"
#include "../includes/utils.h"



int buf_grow(t_buffer *buf, size_t required_space) {
    size_t new_capacity = buf->capacity;

    while (new_capacity - buf->length < required_space) {
        new_capacity *= 2;
        if (new_capacity < required_space) {
            new_capacity = required_space;
        }
    }

    char *new_data = malloc(new_capacity);
    if (!new_data) {
        perror("buf_grow: malloc failed\n");
        return -1;
    }
    if (buf->data) {
        ft_memcpy_faster(new_data, buf->data, buf->length);
        free(buf->data);
    }
    buf->data = new_data;
    buf->capacity = new_capacity;
    return 0;
}

int buf_ensure_space(t_buffer *buf, size_t required_space) {
    while (buf->capacity - buf->length < required_space) {
        if (buf->is_fixed){
            return -1;
        }
        if (buf->mode == BUF_TO_FD) {
            if (buf_flush(buf) < 0)
                return -1;
            if (buf->capacity - buf->length < required_space) {
                if (buf_grow(buf, required_space) < 0)
                    return -1;
            }
        }
        else {
            if (buf_grow(buf, required_space) < 0)
                return -1;
        }
    }
    return 0;
}

int buf_init_fixed(t_buffer *buf, char *prealloc, size_t size, t_buf_content_type type) {
    buf->is_dynamic = false;
    buf->data = prealloc;
    buf->capacity = size;
    buf->length = 0;
    buf->fd = -1;
    buf->mode = BUF_TO_MEMORY;
    buf->content_type = type;
    buf->is_fixed = true;
    return 0;
}

int	buf_init_fd(t_buffer *buf, int fd, t_buf_content_type type){
    buf->is_dynamic = true;
    buf->data = malloc(DEFAULT_FD_BUF_SIZE);
    if (!buf->data)
        return -1;
    buf->length = 0;
    buf->capacity = DEFAULT_FD_BUF_SIZE;
    buf->mode = BUF_TO_FD;
    buf->is_fixed = false;
    buf->fd = fd;
    buf->content_type = type;
    return 0;
}

int buf_init_dynamic(t_buffer *buf, t_buf_content_type type) {
    buf->data = malloc(INITIAL_CAPACITY);
    if (!buf->data)
        return -1;
    buf->capacity = INITIAL_CAPACITY;
    buf->length = 0;
    buf->is_fixed = false;
    buf->is_dynamic = true;
    buf->mode = BUF_TO_MEMORY;
    buf->fd = -1;
    buf->content_type = type;
    return 0;
}

int buf_flush(t_buffer *buf) {
    if (buf->length == 0)
        return 0;
    ssize_t written = write(buf->fd, buf->data, buf->length);
    if (written < 0)
        return -1;
    buf->length = 0;
    return 0;
}

int buf_putchar(t_buffer *buf, char c) {
    if (buf_ensure_space(buf, 1) < 0)
        return -1;
    buf->data[buf->length++] = c;
    return 0;
}

int buf_putnchar(t_buffer *buf, char c, size_t remaining) {
    while (remaining > 0) {
        if (buf_ensure_space(buf, remaining) < 0)
            return -1;

        size_t space = buf->capacity - buf->length;
        size_t to_write = (remaining < space) ? remaining : space;

        if (to_write == 0) {
            if (buf->is_fixed)
                return -1;
            if (buf_flush(buf) < 0)
                return -1;
            continue;
        }

        if (buf_ensure_space(buf, to_write) < 0)
            return -1;
        memset(buf->data + buf->length, c, to_write);
        buf->length += to_write;
        remaining -= to_write;
    }
    return 0;
}

int buf_putstrn(t_buffer *buf, const char *str, size_t remaining) {
    while (remaining > 0) {
        if (buf_ensure_space(buf, remaining) < 0)
            return -1;

        size_t space = buf->capacity - buf->length;
        size_t to_write = (remaining < space) ? remaining : space;

        if (to_write > 0) {
            if (ft_memcpy_faster(buf->data + buf->length, str, to_write) == NULL)
                return -1;
            buf->length += to_write;
            str += to_write;
            remaining -= to_write;
        }
    }
    return 0;
}

int buf_putstr(t_buffer *buf, const char *str) {
    size_t len = ft_strlen(str);
    return buf_putstrn(buf, str, len);
}