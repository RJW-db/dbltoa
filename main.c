#include <stdio.h>
#include <libft.h>
#include <dbltoa.h>

int	main(void)
{
	double i = -__DBL_DENORM_MIN__;

	char *str = dbltoa(i);
	printf("%s\n", str);
	free(str);

	str = dbltoa_precision(i, MAX_DBL_BUFF, true);
	printf("%s\n", str);
	free(str);
	return (0);
}
