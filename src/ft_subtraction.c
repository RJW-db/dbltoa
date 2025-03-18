/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_subtraction.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jdobos <jdobos@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/15 16:27:18 by rde-brui      #+#    #+#                 */
/*   Updated: 2025/03/17 20:14:56 by rde-brui      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <dbltoa.h>

//	Static Functions
static bool		set_to_zero(char *s1);
static bool		subtract_values(char *s1, char *s2, t_nbr *nbr);
static int8_t	swap_and_negate(char *s1, char *s2, t_nbr *nbr);

int16_t	ft_subtraction(char *s1, char *s2)
{
	int16_t	comparison_result;
	t_nbr	nbr;

	init_struct(s1, s2, &nbr);
	comparison_result = ft_strncmp(s1, s2, ft_strlen(s1) + 1);
	if (comparison_result == 0)
		return (set_to_zero(s1));
	else if (comparison_result > 0)
		return (subtract_values(s1, s2, &nbr));
	return (swap_and_negate(s1, s2, &nbr));
}

static bool	set_to_zero(char *s1)
{
	ft_memset(s1, '0', MAX_DBL_STR_LEN);
	s1[0] = '+';
	return (false);
}

static bool	subtract_values(char *s1, char *s2, t_nbr *nbr)
{
	while (nbr->j_s2 >= nbr->sig_s2 && nbr->i_s1 != 0)
	{
		if (s1[nbr->i_s1] < s2[nbr->j_s2])
		{
			s1[nbr->i_s1] += DECIMAL_NBR;
			s1[nbr->i_s1 - 1] -= 1;
		}
		s1[nbr->i_s1] -= (s2[nbr->j_s2] - '0');
		--nbr->j_s2;
		--nbr->i_s1;
	}
	return (true);
}

static int8_t	swap_and_negate(char *s1, char *s2, t_nbr *nbr)
{
	cpy_str(s1, s2);
	subtract_values(s1, s2, nbr);
	if (s1[0] == '+')
		s1[0] = '-';
	else
		s1[0] = '+';
	return (-1);
}
