/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils_dbl.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: rjw <rjw@student.codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/03/16 02:52:35 by rjw           #+#    #+#                 */
/*   Updated: 2025/03/17 14:34:43 by rde-brui      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <dbltoa.h>

void	intialize_buff(char *buff)
{
	ft_memset(buff, '0', MAX_DBL_STR_LEN);
	buff[MAX_DBL_STR_LEN] = '\0';
	buff[0] = '+';
}

void	init_struct(char *s1, char *s2, t_nbr *num)
{
	num->sig_s1 = (s1[0] == '+' || s1[0] == '-');
	while (s1[num->sig_s1] == '0')
		++num->sig_s1;
	num->i_s1 = num->sig_s1 + ft_strlen(s1 + num->sig_s1);
	num->i_s1 -= (num->i_s1 > 0);
	num->sig_s2 = (s2[0] == '+' || s2[0] == '-');
	while (s2[num->sig_s2] == '0')
		++num->sig_s2;
	num->j_s2 = num->sig_s2 + ft_strlen(s2 + num->sig_s2);
	num->j_s2 -= (num->j_s2 > 0);
}

size_t	strlen_and_dot(const char *str, bool *is_dot)
{
	const char	*tmp = str;

	*is_dot = false;
	while (*tmp != '\0')
	{
		if (*tmp == '.')
			*is_dot = true;
		++tmp;
	}
	return (tmp - str);
}

uint16_t	trim_trailing_zeros(char *result, uint16_t res_len)
{
	while (result[res_len - 1] == '0')
		--res_len;
	result[res_len] = '\0';
	if (result[res_len - 1] == '.')
	{
		--res_len;
		result[res_len] = '\0';
	}
	return (res_len);
}
