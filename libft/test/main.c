
#include "../includes/libft.h"
#include <stdio.h>
#include <fcntl.h>

int main(void)
{
    char *str = "Hello, world!";
    int num = 42;
    printf("test libft ft_strlen xxx = %zu\n", ft_strlen("xxx"));
    printf("Test printf normal: %s, %d\n", str, num);
    ft_printf("Test ft_printf: %s, %d\n", str, num);
    ft_printf_fd(2,"coucou stderr\n");

    int fd = open("main.c", O_RDONLY);
    if (fd < 0)
    {
        perror("open");
        return (1);
    }

    char *line;
    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);
    }
    close(fd);
    return (0);

    return 0;
}
