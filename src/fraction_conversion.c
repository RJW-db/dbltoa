/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   fraction_conversion.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: rjw <rjw@student.codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/03/12 01:36:32 by rjw           #+#    #+#                 */
/*   Updated: 2025/03/12 16:14:26 by rde-brui      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <dbltoa.h>

#if defined(__linux__)
# define NEGATIVE_NAN_INDEX 0
#else
# define NEGATIVE_NAN_INDEX 1
#endif

//	Static Funnctions
static char	*double_to_bits(int64_t double_bits, char *bit_representation);
static void	extract_expo_mant(int16_t *exponent, uint64_t *mant, char *strbits);
static void	special_value(t_dbl *strs, double val, uint64_t mant, bool is_neg);

/*
* IEEE-754 Double Precision Format:
* SEEEEEEE EEEEMMMM MMMMMMMM MMMMMMMM MMMMMMMM MMMMMMMM MMMMMMMM MMMMMMMM
* ↑↑           ↑
* ||           └── Mantissa (52 bits) → `mantissa`
* |└── Exponent (11 bits) → `exponent`
* └── Sign bit (1 bit)
* 
* Sign Bit (1 bit): 0 for positive, 1 for negative.
* Exponent (11 bits): Stores the power of 2 (biased by 1023).
* Mantissa (52 bits): Stores the fractional part.
* 
* 		------------------------------------------------
* 
* If exponent == 972, this means dbl is either:
* Infinity (+∞ or -∞)
* NaN (Not a Number -> 0.0 / 0.0)
* Calls special_value() to return a string like "inf" or "nan".
* TEST CASES:
* 	- double test = 1.0 / 0.0; // Division by zero creates +∞
* 	- double test = -1.0 / 0.0; // Negative division by zero creates -∞
* 	- double test = 0.0 / 0.0; // 0 divided by 0 produces NaN
* 	- double test = -0.0 / 0.0; // 0 divided by 0 produces NaN
* 
* Why 972?
* In IEEE-754:
* 	- The exponent is stored as biased (actual_exp + 1023).
* 	- Infinity and NaN have an exponent of all 1s (2047 in decimal).
* 		S1111111 1111MMMM MMMMMMMM MMMMMMMM MMMMMMMM MMMMMMMM MMMMMMMM MMMMMMMM
* 	- extract_expo_mant() subtracts the bias (1023), so we get 972 (2047 - 1075)
*/
bool	fraction_conversion(double value, t_dbl *strings, bool *is_neg)
{
	char		bit_representation[DBL_BIT_COUNT + 1];
	int16_t		exp_value;
	uint64_t	mantissa_value;
	t_bitcast	cast;

	cast.d = value;
	double_to_bits(cast.i, bit_representation);
	if (bit_representation[0] == '0' && cast.i != (int64_t)NEGATIVE_NAN_BITS)
		*is_neg = false;
	extract_expo_mant(&exp_value, &mantissa_value, bit_representation);
	if (exp_value == DBL_EXP_MAX)
	{
		special_value(strings, value, mantissa_value, *is_neg);
		return (false);
	}
	populate_numerator(strings->s1, mantissa_value, exp_value);
	populate_denominator(strings->s2, exp_value, value);
	return (true);
}

static char	*double_to_bits(int64_t double_bits, char *bit_representation)
{
	char	byte_buffer[BYTE + 1];
	int16_t	byte_index;
	size_t	bit_index;
	size_t	bit_count;
	uint8_t	byte_value;

	bit_representation[DBL_BIT_COUNT] = '\0';
	byte_index = DBL_BYTES - 1;
	bit_index = 0;
	while (byte_index >= 0)
	{
		byte_value = double_bits >> (byte_index * BYTE) & BYTE_MASK;
		int64_base(byte_value, "01", byte_buffer, BYTE + 1);
		bit_count = BYTE - ft_strlen(byte_buffer);
		ft_memset(bit_representation + bit_index, '0', bit_count);
		cpy_str(bit_representation + bit_index + bit_count, byte_buffer);
		bit_index += BYTE;
		--byte_index;
	}
	return (bit_representation);
}

/*
* IEEE-754 Double Precision Format:
* SEEEEEEE EEEEMMMM MMMMMMMM MMMMMMMM MMMMMMMM MMMMMMMM MMMMMMMM MMMMMMMM
* ↑↑           ↑
* ||           └── Mantissa (52 bits) → `mantissa`
* |└── Exponent (11 bits) → `exponent`
* └── Sign bit (1 bit)
* 
* Sign Bit (1 bit): 0 for positive, 1 for negative.
* Exponent (11 bits): Stores the power of 2 (biased by 1023).
* Mantissa (52 bits): Stores the fractional part.
*
*		------------------------------------------------
*
* What the function does?
*	- Extracts the 11-bit exponent from the binary string.
*	- Extracts the 52-bit mantissa from the binary string.
*	- Converts exponent and mantissa to decimal.
*	- Adjusts for normalized/denormalized numbers.
*
*		------------------------------------------------
*
* Adjusts the exponent bias?
* 	IEEE-754 exponent bias is 1023.
* 	Denormalized numbers (exponent 0) use -1074 instead of -1075.
* 	Normalized numbers subtract 1075 to get the actual exponent.
*
* 	Example:
* 	| Binary 	    | Exponent | Decimal (Biased) | Adjusted (Actual Exponent) |
* 	| "10000000000" |	1024   | 	  1075		  |	   1024 - 1075   =   -51   |
*	| "01111111111"	|	1023   |	  1075		  |	   1023 - 1075   =   -52   |
* 	| "00000000000"	|	0	   | 	  1075		  |	   0 	- 1074   =   -1074 |
*
*
* Why 1074 and 1075?
* 	{
* 		// Adjusts the exponent bias
* 		if (*exponent == 0)
*			*exponent -= 1074;
*		else
*			*exponent -= 1075;
* 	 }
* 	- The exponent is stored with a bias of 1023.
* 	- The smallest exponent for a normalized number is -1022.
* 	- If the exponent is not zero, we adjust it using:
*		actual exponent = stored exponent − 1023 − 1 = stored exponent − 1075
*	- When the exponent is all zeros (00000000000), the number is denormalized.
*	- The actual exponent for denormals is always
		-1022 (minimum exponent) - 52 (mantissa shift):
*		− 1022 − 52 = − 1074
* 	- This is why for denormalized numbers, we use:
*		exponent -= 1074;
* 	- For normalized numbers, we use:
*		exponent -= 1075;
*		Because the implicit leading 1 bit in the mantissa
*		shifts the exponent by an extra 1.
* 
*
* Adjusts the mantissa bias?
* 	if (atoi64(exp_bits) && (atoi64(exp_str) - 1075) != 972)
*     	*mantissa += (1UL << 52); 	// Adds 2^52 to restore the implicit 1
* 	If exponent is NOT 972, add 2^52 to the mantissa. (Bit-shift 52 to left)
* 	This accounts for the implicit leading 1 in normalized numbers.
*	In other words:
*		If the exponent is not zero & not 972, the number is normalized, 
*		and implicit leading 1 is added
*		(mantissa becomes 1.xxxxx instead of 0.xxxxx).
*
*		Example:
*		| Mantissa Bits	| Decimal Mantissa | Adjusted Mantissa (with 2^52)	   |
*		| "100100..."	| 31897010349248   | 31897010349248 + 4503599627370496 |
*
* Why is 972 used?
* 	The 972 is related to special cases of floating-point numbers 
* 	like Infinity (Inf) and NaN (Not-a-Number).
* 
* 	Special IEEE-754 Exponent Values
* 	- The largest possible exponent in 11-bit storage is 2047
*		(11111111111 in binary).
* 	- The largest normal exponent is 1023.
* 	- A value of 1024 could mean infinity (Inf).
* 	- Exponent value 972 is an indicator of Inf or NaN.
* 
* 	Why Check if (exponent - 1075) != 972?
* 	- If the biased exponent is 2047, the number is Inf or NaN.
* 	- To detect this, we check:
* 		if (exponent - 1075 != 972)
* 		- When exponent - 1075 == 972 → It is Inf or NaN.
* 		- Otherwise, it is a regular number.
* 
* Steps:
* 		1):
* 			exp_bits : Extracts 11 bits of the exponent
*			(starts at 1 to skip the sign bit)

* 			mant_bits : Extracts mantissa bits
			(starts at 12, because exponent goes till 11 bits)
* 		2):
* 			Converts binary exponent to exp_str/mant_str
*			(exponent "10000000000" → decimal "1024")
* 		3):
* 			Converts string to int (string "1024" to integer 1024)
* 		4):
* 			Adjusts the exponent bias normalize number (if not zero)
*/
static void	extract_expo_mant(int16_t *exponent, uint64_t *mant, char *strbits)
{
	char	exp_bits[DBL_EXP_BITS + 1];
	char	exp_str[DBL_EXP_DECIMAL_DIGITS + 1];
	char	mant_bits[DBL_MANT_BITS + 1];
	char	mant_str[DBL_MANT_DECIMAL_DIGITS + 1];

	extract_substr(strbits, 1, DBL_EXP_BITS, exp_bits);
	extract_substr(strbits, DBL_EXP_BITS + 1, DBL_MANT_BITS, mant_bits);
	binary_to_decimal(exp_bits, exp_str, sizeof(exp_str));
	binary_to_decimal(mant_bits, mant_str, sizeof(mant_str));
	*exponent = atoi64(exp_str);
	*mant = atoi64(mant_str);
	if (*exponent != 0)
		*exponent -= DBL_EXP_NORMAL_BIAS;
	else
		*exponent -= DBL_EXP_SUBNORMAL_BIAS;
	if (atoi64(exp_bits) > 0
		&& (atoi64(exp_str) - DBL_EXP_NORMAL_BIAS) != DBL_EXP_MAX)
	{
		*mant += (1UL << DBL_MANT_BITS);
	}
}

/* special_value()
* This function handles errors related to floating-point numbers, 
* specifically when dealing with special cases like "infinity" or "NaN".
* Example:
* 		value = 1.0 and mantissa = 0, return "inf".
*		value = -1.0 and mantissa = 0, return "-inf".
*		value = 0.0 and mantissa = 0, return "nan".
*		value = -1.0 and mantissa = 0, return "-nan".
*/
static void	special_value(t_dbl *strs, double val, uint64_t mant, bool is_neg)
{
	const char	nan_strings[2][5] = {"-nan", "nan"};

	if (mant == 0 && val > 0)
		cpy_str(strs->result, "inf");
	else if (mant == 0 && val < 0)
		cpy_str(strs->result, "-inf");
	else
	{
		if (is_neg == false)
			cpy_str(strs->result, "nan");
		else
			cpy_str(strs->result, nan_strings[NEGATIVE_NAN_INDEX]);
	}
}
