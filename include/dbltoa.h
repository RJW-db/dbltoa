/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   dbltoa.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: rjw <rjw@student.codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/03/12 01:38:30 by rjw           #+#    #+#                 */
/*   Updated: 2025/03/18 04:12:21 by rjw           ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef DBLTOA_H
# define DBLTOA_H

# include <math.h>		//	INFINITY, NAN
# include <stddef.h>	//	size_t
# include <stdint.h>	//	int16_t, uint16_t
# include <stdbool.h>
# include <libft.h>

# include <stdlib.h>
# include <stdio.h>

// //	longest numbers -__DBL_DENORM_MIN__ is 1077 characters long + terminator
# define MAX_DBL_STR_LEN 1078
//	DBL_DENORM_MIN (1078) + DBL_MAX (310) + dot + terminator = 1388
# define MAX_DBL_BUFF 1388
# define MAX_PRECISION 1074

# define BYTE_MASK 0xFF
# define DBL_BYTES 8
# define BYTE 8
# define DBL_BIT_COUNT 64

# define DECIMAL_NBR 10
# define DECIMAL_BASE "0123456789"
# define BINARY_NBR 2
# define BINARY_BASE "01"
# define HEXADECIMAL_BASE "0123456789ABCDEF"

// IEEE 754 representation of -NAN
# define NEGATIVE_NAN_BITS 0xFFF8000000000000
# define DBL_EXP_BITS 11
# define DBL_EXP_DECIMAL_DIGITS 5
# define DBL_MANT_BITS 52
# define DBL_MANT_DECIMAL_DIGITS 16  // Maximum digits needed for mantissa
// 1023 (IEEE 754 exponent bias) + 52 (mantissa bit count)
# define DBL_EXP_NORMAL_BIAS 1075
// 1023 (IEEE 754 exponent bias) + 52 - 1 (subnormal adjustment)
# define DBL_EXP_SUBNORMAL_BIAS 1074
// 2047 (max exponent value in IEEE 754) - 1075 (DBL_EXP_NORMAL_BIAS)
# define DBL_EXP_MAX 972

typedef struct s_dbltoa_params
{
	double		value;
	char		*buff;
	uint16_t	buff_size;
	uint16_t	precision;
	bool		trim_trailing_zeros;
}	t_dbltoa_params;

/**
 * struct s_dbl - Structure to hold double conversion information
 * @param[in] s1 Pointer to the numerator string
 * @param[in] s2 Pointer to the denominator string
 * @param[in] result Pointer to the result string
 * @param[in] prec Precision value for the conversion
 *
 * This structure is used to store information related to the conversion
 * of a double value to its string representation, including the numerator,
 * denominator, result, and precision.
 */
typedef struct s_dbl
{
	char		*s1;
	char		*s2;
	char		*result;
	uint16_t	prec;
}	t_dbl;

/**
 * struct s_nbr - Structure to hold numerical string information
 * @param[in] i_s1 Index of the last character in s1
 * @param[in] j_s2 Index of the last character in s2
 * @param[in] sig_s1 Index of the first significant digit in s1
 * @param[in] sig_s2 Index of the first significant digit in s2
 *
 * This structure is used to store information about two numerical strings,
 * including the indices of their last characters and the indices of their
 * first significant digits. It helps in various numerical operations where
 * such information is required.
 */
typedef struct s_nbr
{
	int32_t	i_s1;
	int32_t	j_s2;
	int32_t	sig_s1;
	int32_t	sig_s2;
}	t_nbr;

typedef union u_double_bitcast
{
	double	d;
	int64_t	i;
}	t_bitcast;

char		*dbltoa(double dbl);
char		*dbltoa_precision(double value, uint16_t prec, bool round);
uint16_t	dbltoa_buff(double value, char *buff, uint16_t b_size);
uint16_t	dbltoa_buff_prec(t_dbltoa_params dbl);

//	Conversions
bool		fraction_conversion(double value, t_dbl *strings, bool *is_neg);
bool		binary_to_decimal(const char *bin_str, char *dec_str, size_t size);
char		*populate_numerator(char *num_str, uint64_t mant, int16_t expo);
char		*populate_denominator(char *denom_str, int16_t expo, double value);
void		scientific_notation(char *num, char *den, int16_t *digitexpo, double val);
uint16_t	double_to_string(t_dbl *s, int16_t digit_exponent, bool is_neg);
uint16_t	process_precision(char *result, uint16_t prec);
uint16_t	set_precision(char *result, uint16_t prec, uint16_t dot_index);

//	Math
void		ft_addition(char *s1, char *s2);
int16_t		ft_subtraction(char *s1, char *s2);
void		ft_multiply(char *s1, char *s2);
void		ft_division(char *s1, char *s2);

//	Utils
void		intialize_buff(char *buff);
void		init_struct(char *s1, char *s2, t_nbr *num);
size_t		strlen_and_dot(const char *str, bool *is_dot);
uint16_t	trim_trailing_zeros(char *result, uint16_t res_len);
#endif