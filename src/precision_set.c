/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   precision_set.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: rjw <rjw@student.codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/03/15 01:55:10 by rjw           #+#    #+#                 */
/*   Updated: 2025/03/15 01:56:06 by rjw           ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <dbltoa.h>

//	Static functions
static void		handle_carry(char *s1, int16_t *len, int16_t i, int16_t *carry);
static uint16_t	add_precision(char *s1, const char *s2);

uint16_t	set_precision(char *result, uint16_t prec, uint16_t dot_index)
{
	char		rounding_increment[MAX_DBL_STR_LEN + 1];
	uint16_t	prec_index;
	char		rounding_nbr;

	prec_index = 0;
	while (result[dot_index + prec_index] != '\0' && prec_index <= prec)
		++prec_index;
	rounding_nbr = result[dot_index + prec_index];
	result[dot_index + prec_index] = '\0';
	if (rounding_nbr >= '5')
	{
		ft_memset(rounding_increment, '0', prec_index);
		rounding_increment[1] = '.';
		rounding_increment[prec_index] = '1';
		rounding_increment[prec_index + 1] = '\0';
		return (add_precision(result, rounding_increment));
	}
	if (result[dot_index + prec_index - 1] == '.')
	{
		--prec_index;
		result[dot_index + prec_index] = '\0';
	}
	return (dot_index + prec_index);
}

static uint16_t	add_precision(char *s1, const char *s2)
{
	int16_t	s1_len;
	int16_t	carry;
	int16_t	i;
	int16_t	j;

	s1_len = ft_strlen(s1) - 1;
	j = ft_strlen(s2) - 1;
	carry = 0;
	i = s1_len;
	while (i >= 0 || j >= 0 || carry != 0)
	{
		if (i >= 0 && s1[i] != '.')
			carry += s1[i] - '0';
		if (j >= 0 && s2[j] != '.')
			carry += s2[j] - '0';
		handle_carry(s1, &s1_len, i, &carry);
		--i;
		--j;
	}
	i = s1_len;
	while (i > 0 && s1[i] == '0')
		--i;
	i -= (s1[i] == '.');
	s1[i + 1] = '\0';
	return (i + 1);
}

static void	handle_carry(char *s1, int16_t *len, int16_t i, int16_t *carry)
{
	if (i < 0 || s1[i] != '.')
	{
		if (i >= 0)
			s1[i] = (*carry % DECIMAL_NBR) + '0';
		else
		{
			charmove(s1 + 1, s1, ++(*len));
			s1[0] = (*carry % DECIMAL_NBR) + '0';
		}
		*carry /= DECIMAL_NBR;
	}
	else
		s1[i] = '.';
}
