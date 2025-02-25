#include "libft.h"

int	power_10(int expo)
{
	int	result;

	result = 1;
	while (expo != 0)
	{
		result *= 10;
		expo--;
	}
	return (result);
}