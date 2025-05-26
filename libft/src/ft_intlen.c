#include "libft.h"

size_t	ft_intlen(long long int n)
{
	long long int		result;
	size_t	count;

	result = n;
	count = 0;
	if (n <= 0)
		count++;
	while (result != 0)
	{
		result = result / 10;
		count++;
	}
	return (count);
}