/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   double_to_string.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: rjw <rjw@student.codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/03/12 01:35:36 by rjw           #+#    #+#                 */
/*   Updated: 2025/03/18 03:07:09 by rjw           ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <dbltoa.h>

//	Static Functions
static uint16_t	initialize_strings(t_dbl *s, int16_t *digitexp);
static void		process_first_digit(char *result, char *num, char *deno);
static void		insert_dec_point(char *res, char *num, uint16_t *len, int16_t *exp);
static uint16_t	process_number_string(char *result, bool is_neg);

uint16_t	double_to_string(t_dbl *s, int16_t digit_exponent, bool is_neg)
{
	char		tmp[MAX_DBL_STR_LEN + 1];
	char		digit[MAX_DBL_STR_LEN + 1];
	char		zero[MAX_DBL_STR_LEN + 1];
	uint16_t	len;

	len = initialize_strings(s, &digit_exponent);
	intialize_buff(digit);
	digit[MAX_DBL_STR_LEN - 2] = '1';
	intialize_buff(zero);
	while (ft_strncmp(s->s1, zero, MAX_DBL_STR_LEN) != 0 && len < MAX_DBL_STR_LEN)
	{
		
		// intialize_buff(tmp);
		cpy_str(tmp, s->s1);
		ft_division(tmp, s->s2);
		s->result[len++] = tmp[MAX_DBL_STR_LEN - 1];
		while (tmp[MAX_DBL_STR_LEN - 1]-- != '0')
			ft_subtraction(s->s1, s->s2);
		ft_multiply(s->s1, digit);
		insert_dec_point(s->result, s->s1, &len, &digit_exponent);
	}
	return (process_number_string(s->result, is_neg));
}

static uint16_t	initialize_strings(t_dbl *s, int16_t *digitexp)
{
	int16_t		pos_exp;
	uint16_t	len;

	s->result[MAX_DBL_STR_LEN] = '\0';
	ft_memset(s->result, '0', MAX_DBL_STR_LEN);
	if (*digitexp > 0)
		return (1);
	s->result[2] = '.';
	if (*digitexp < 0)
	{
		len = 0;
		pos_exp = -(*digitexp);
		while (len < pos_exp)
			++len;
		len += 2;
		*digitexp = 0;
		return (len);
	}
	process_first_digit(s->result, s->s1, s->s2);
	return (3);
}

/*
* --tmp[MAX_DBL_STR_LEN - 1];
* if tmp[] = '5' it would become tmp[] = '4'
*/
static void	process_first_digit(char *result, char *num, char *deno)
{
	char	tmp[MAX_DBL_STR_LEN + 1];
	char	digit[MAX_DBL_STR_LEN + 1];

	intialize_buff(tmp);
	intialize_buff(digit);
	digit[MAX_DBL_STR_LEN - 2] = '1';
	cpy_str(tmp, num);
	ft_division(tmp, deno);
	result[1] = tmp[MAX_DBL_STR_LEN - 1];
	while (tmp[MAX_DBL_STR_LEN - 1] != '0')
	{
		ft_subtraction(num, deno);
		--tmp[MAX_DBL_STR_LEN - 1];
	}
	ft_multiply(num, digit);
}

static void	insert_dec_point(char *res, char *num, uint16_t *len, int16_t *exp)
{
	char		zero[MAX_DBL_STR_LEN + 1];
	uint16_t	i;

	i = 0;
	intialize_buff(zero);
	if (*len - 1 > *exp && *exp != 0)
	{
		res[(*len)++] = '.';
		*exp = 0;
	}
	if (ft_strncmp(num, zero, MAX_DBL_STR_LEN + 1) == 0)
	{
		while (res[i] != '\0' && res[i] != '.')
			++i;
		if (res[i] == '\0')
			res[*exp + 2] = '.';
	}
}

static uint16_t	process_number_string(char *result, bool is_neg)
{
	uint16_t	offset;
	uint16_t	len;
	bool		is_dot;

	offset = 1;
	len = strlen_and_dot(result, &is_dot) - 1;
	while (len > 0 && result[len] == '0')
		--len;
	if (result[len] == '.' && result[0] == '0')
		--len;
	if (result[1] == '0' && result[2] >= '1' && result[2] <= '9')
	{
		is_dot = false;
		offset = 2;
		--len;
	}
	if (is_neg == true)
		result[0] = '-';
	charmove(result + is_neg, result + offset, len);
	result[len + is_neg] = '\0';
	if (is_dot == true)
		return (0);
	return (len + is_neg + (offset == 0));
}
