/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_binary_to_decimal.c                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: rjw <rjw@student.codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/03/12 03:53:16 by rjw           #+#    #+#                 */
/*   Updated: 2025/03/12 16:14:26 by rde-brui      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <dbltoa.h>

static uint16_t	base_length(uint64_t nbr, uint64_t base);
static void		convert_base(uint64_t nbu, char *base, uint64_t b_val, char *result);

bool	binary_to_decimal(const char *bin_str, char *dec_str, size_t size)
{
	uint64_t	binary_value;
	uint16_t	bit_length;
	bool		sign;

	binary_value = abs_int64(atoi_base(bin_str, BINARY_BASE, &sign));
	bit_length = base_length(binary_value, DECIMAL_NBR);
	if (size < (size_t)(bit_length + 1))
		return (false);
	ft_memset(dec_str, '\0', bit_length + 1);
	if (sign == true && binary_value != 0)
		dec_str[0] = '-';
	convert_base(binary_value, DECIMAL_BASE, DECIMAL_NBR, dec_str);
	return (true);
}

static uint16_t	base_length(uint64_t nbr, uint64_t base)
{
	uint16_t	length;

	length = 1;
	while (nbr >= base)
	{
		nbr /= base;
		length += 1;
	}
	return (length);
}

static void	convert_base(uint64_t nbu, char *base, uint64_t b_val, char *result)
{
	uint64_t	i;
	uint64_t	len;
	char		tmp;

	if (b_val < 2 || b_val > 64)
		return ;
	len = 0;
	while (nbu >= b_val)
	{
		result[len++] = base[nbu % b_val];
		nbu /= b_val;
	}
	result[len++] = base[nbu];
	result[len] = '\0';
	i = 0;
	while (i < len / 2)
	{
		tmp = result[i];
		result[i] = result[len - i - 1];
		result[len - i - 1] = tmp;
		++i;
	}
}
