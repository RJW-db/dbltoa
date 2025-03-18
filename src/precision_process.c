/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   precision_process.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: rjw <rjw@student.codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/03/11 20:02:40 by rjw           #+#    #+#                 */
/*   Updated: 2025/03/18 01:46:55 by rjw           ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <dbltoa.h>

//	Static functions
static uint16_t	adjust_prec(char *res, uint16_t res_len, uint16_t prec, bool is_neg);
static bool		is_only_zero_decimals(char *result);
uint16_t		zero_padding(char *result, bool is_dot, uint16_t prec);

uint16_t	process_precision(char *result, uint16_t prec)
{
	uint16_t	res_index;
	uint16_t	res_len;
	uint16_t	padding;
	bool		is_neg;

	if (result[0] == '-' && result[1] == '0' && result[2] == '\0')
		cpy_str(result, "0");
	is_neg = (*result == '-');
	if (is_neg == true)
		result += is_neg;
	res_index = 0;
	while (result[res_index] != '\0')
	{
		if (result[res_index] == '.')
		{
			res_len = set_precision(result, prec, res_index);
			return (adjust_prec(result, res_len, prec, is_neg));
		}
		++res_index;
	}
	padding = 0;
	if (prec > 0)
		padding = zero_padding(result + res_index, true, prec);
	return (res_index + is_neg + padding);
}

static uint16_t	adjust_prec(char *res, uint16_t res_len, uint16_t prec, bool is_neg)
{
	uint16_t	dec_count;
	uint16_t	dot_idx;

	if (is_neg == true && res[0] == '0' && (res[1] != '.' || \
	is_only_zero_decimals(res + 2) == true))
	{
		cpy_str(res - is_neg, res);
		is_neg = false;
		--res;
	}
	if (prec == 0)
		return (res_len + is_neg);
	dot_idx = 0;
	while (res[dot_idx] != '\0' && res[dot_idx] != '.')
		++dot_idx;
	if (res[dot_idx] == '\0')
		return (res_len + is_neg + zero_padding(res + dot_idx, true, prec));
	dec_count = 0;
	while (res[dot_idx + 1 + dec_count] != '\0')
		++dec_count;
	if (prec > dec_count)
		return (res_len + is_neg + \
		zero_padding(res + ++dot_idx + dec_count, false, prec - dec_count));
	return (res_len + is_neg);
}

uint16_t	zero_padding(char *result, bool is_dot, uint16_t prec)
{
	char		*tmp;
	uint16_t	i;

	tmp = result;
	if (is_dot == true)
	{
		tmp[0] = '.';
		++tmp;
	}
	i = 0;
	while (i < prec)
	{
		tmp[i] = '0';
		++i;
	}
	tmp[i] = '\0';
	return (i + is_dot);
}

static bool	is_only_zero_decimals(char *result)
{
	uint16_t	zeros;

	zeros = 0;
	while (result[zeros] == '0')
		++zeros;
	if (result[zeros] != '\0')
		return (false);
	return (true);
}
