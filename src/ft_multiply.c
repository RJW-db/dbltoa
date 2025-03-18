/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_multiply.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: rjw <rjw@student.codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/03/12 01:37:02 by rjw           #+#    #+#                 */
/*   Updated: 2025/03/15 01:28:57 by rjw           ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <dbltoa.h>

//	Static Functions
static void	init_numbers(t_nbr *nbr, char *s1, char *s2);
static void	process_digits(t_dbl *strings, t_nbr *nbr);
static void	multiply_single_digit(t_dbl *s, t_nbr *nbr, int32_t i, int32_t j);
static void	process_carry(char *result, int32_t carry, int32_t pos);

void	ft_multiply(char *s1, char *s2)
{
	char	result[MAX_DBL_STR_LEN + 1];
	char	sign;
	t_dbl	strings;
	t_nbr	nbr;

	intialize_buff(result);
	sign = '+';
	if ((s1[0] == '-' && s2[0] != '-') || (s1[0] != '-' && s2[0] == '-'))
		sign = '-';
	init_numbers(&nbr, s1, s2);
	strings = (t_dbl){.s1 = s1, .s2 = s2, .result = result};
	process_digits(&strings, &nbr);
	cpy_str(s1, result);
	s1[0] = sign;
}

static void	init_numbers(t_nbr *nbr, char *s1, char *s2)
{
	nbr->sig_s1 = find_char_not(s1 + (s1[0] == '+' || s1[0] == '-'), '0');
	nbr->sig_s2 = find_char_not(s2 + (s2[0] == '+' || s2[0] == '-'), '0');
	nbr->i_s1 = ft_strlen(s1 + nbr->sig_s1) - 1;
	nbr->j_s2 = ft_strlen(s2 + nbr->sig_s2) - 1;
}

static void	process_digits(t_dbl *strings, t_nbr *nbr)
{
	int32_t	i;
	int32_t	j;

	i = nbr->j_s2;
	j = nbr->i_s1;
	while (i >= 0)
	{
		multiply_single_digit(strings, nbr, i, j);
		--i;
	}
}

static void	multiply_single_digit(t_dbl *s, t_nbr *nbr, int32_t i, int32_t j)
{
	int32_t	pos;
	int32_t	tmp;
	int32_t	carry;

	carry = 0;
	while (j >= 0)
	{
		pos = MAX_DBL_STR_LEN - 1 - (nbr->j_s2 - i) - (nbr->i_s1 - j);
		if (pos >= 1)
		{
			tmp = ((s->s2[nbr->sig_s2 + i] - '0') * \
				(s->s1[nbr->sig_s1 + j] - '0')) + \
				(s->result[pos] - '0') + carry;
			s->result[pos] = (tmp % DECIMAL_NBR) + '0';
			carry = tmp / DECIMAL_NBR;
		}
		--j;
	}
	process_carry(s->result, carry, pos);
}

static void	process_carry(char *result, int32_t carry, int32_t pos)
{
	int32_t	tmp;

	while (carry > 0 && pos >= 1)
	{
		tmp = (result[pos] - '0') + carry;
		result[pos] = (tmp % DECIMAL_NBR) + '0';
		carry = tmp / DECIMAL_NBR;
		--pos;
	}
}
