/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   fraction_operations.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: rjw <rjw@student.codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/03/12 02:52:56 by rjw           #+#    #+#                 */
/*   Updated: 2025/03/18 03:01:44 by rjw           ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <dbltoa.h>

//	Static Function
static char		*pow2(char *exp_str, int64_t exponent);

char	*populate_numerator(char *num_str, uint64_t mant, int16_t expo)
{
	char	mant_bits[DBL_MANT_BITS + 1];
	char	exp_str[MAX_DBL_STR_LEN + 1];
	size_t	mant_len;

	mant_len = int64_base(mant, DECIMAL_BASE, mant_bits, sizeof(mant_bits));
	if (mant_len >= MAX_DBL_STR_LEN)
		return (NULL);
	if (expo > 0)
	{
		cpy_str(num_str + MAX_DBL_STR_LEN - mant_len, mant_bits);
		intialize_buff(exp_str);
		exp_str[MAX_DBL_STR_LEN - 1] = '2';
		if (pow2(exp_str, expo) == NULL)
			return (NULL);
		ft_multiply(num_str, exp_str);
	}
	else
		cpy_str(num_str + MAX_DBL_STR_LEN - mant_len, mant_bits);
	return (num_str);
}

char	*populate_denominator(char *denom_str, int16_t expo, double value)
{
	if (expo <= 0 && value != 0)
	{
		denom_str[MAX_DBL_STR_LEN - 1] = '2';
		if (pow2(denom_str, -expo) == NULL)
			return (NULL);
		return (denom_str);
	}
	denom_str[MAX_DBL_STR_LEN - 1] = '1';
	return (denom_str);
}

static char	*pow2(char *exp_str, int64_t exponent)
{
	char	pow2[MAX_DBL_STR_LEN + 1];

	if (exponent != 0)
	{
		while (exponent > 1)
		{
			cpy_str(pow2, exp_str);
			ft_addition(exp_str, pow2);
			--exponent;
		}
	}
	else
	{
		exp_str[MAX_DBL_STR_LEN - 1] = '1';
	}
	return (exp_str);
}
