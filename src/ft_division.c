/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_division.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: rjw <rjw@student.codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/03/12 01:36:51 by rjw           #+#    #+#                 */
/*   Updated: 2025/03/13 20:17:43 by rde-brui      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <dbltoa.h>

//	Static Functions
static void		perform_division(char *s1, char *den, char *result, t_nbr *nbr);
static uint8_t	is_division_result_equal(char *num_str, char *den_str);
static uint8_t	find_quotient(char *s1, char *s2);

void	ft_division(char *s1, char *s2)
{
	t_nbr	nbr;
	char	result[MAX_DBL_STR_LEN + 1];

	intialize_buff(result);
	if (ft_strncmp(result, s2, MAX_DBL_STR_LEN + 1) == 0)
		return ;
	if (ft_strncmp(s1, s2, ft_strlen(s1) + 1) >= 0)
		perform_division(s1, s2, result, &nbr);
	cpy_str(s1, result);
}

static void	perform_division(char *s1, char *den, char *result, t_nbr *nbr)
{
	char	numerator[DBL_MANT_DECIMAL_DIGITS];
	char	denominator[DBL_MANT_DECIMAL_DIGITS];
	uint8_t	quotient;

	init_struct(s1, den, nbr);
	ft_memset(numerator, '\0', DBL_MANT_DECIMAL_DIGITS);
	ft_memset(denominator, '\0', DBL_MANT_DECIMAL_DIGITS);
	numerator[0] = '0';
	denominator[0] = '0';
	quotient = MAX_DBL_STR_LEN - nbr->sig_s2;
	if (quotient > DBL_MANT_DECIMAL_DIGITS - 1)
		quotient = DBL_MANT_DECIMAL_DIGITS - 1;
	ft_strlcpy(denominator + 1, den + nbr->sig_s2, quotient);
	if (nbr->sig_s1 == nbr->sig_s2)
		ft_strlcpy(numerator + 1, s1 + nbr->sig_s1, quotient);
	else
		ft_strlcpy(numerator, s1 + nbr->sig_s1, quotient + 1);
	quotient = is_division_result_equal(numerator, denominator);
	if (quotient == 0)
		quotient = find_quotient(s1, den);
	result[MAX_DBL_STR_LEN - 1] = quotient + '0';
}

static uint8_t	is_division_result_equal(char *num_str, char *den_str)
{
	const int64_t	num = atoi64(num_str);
	const int64_t	den = atoi64(den_str);
	int64_t			quotient;

	if (den > 0)
	{
		quotient = num / den;
		if (quotient == (num / (den + 1)))
			return (quotient);
	}
	return (0);
}

static uint8_t	find_quotient(char *s1, char *s2)
{
	uint8_t	quotient;
	int16_t	result;

	quotient = 0;
	while (1)
	{
		result = ft_subtraction(s1, s2);
		if (result < 0)
			break ;
		if (result == 0)
		{
			++quotient;
			break ;
		}
		++quotient;
	}
	return (quotient);
}
