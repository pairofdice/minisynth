# include "minisynth.h"

double sine_instrument(double frequency, float tc)
{
	return (sin(tc /44100.0 * frequency * 2.0 * M_PI));
}

double square_instrument(double frequency, float tc)
{
	if (sin(tc/44100.0 * frequency * 2.0 * M_PI) > 0)
		return (1);
	else
		return (-1);
}

double triangle_instrument(double frequency, float tc)
{
	double res, x;
	int	factor;

	x = tc * frequency * 2.0 * M_PI;
	res = sin(x);
	res += ((pow(3, -2)) * -(sin(x * 3)));
	factor = 5;
	while (factor < 12)
	{
		res += ((pow(factor, -2)) * (sin(x * factor)));
		factor += 2;
	}
	return res;
}

double saw_instrument(double frequency, float tc)
{
	double res, x;
	int factor;

	x = tc * frequency * 2.0 * M_PI;
	res = sin(x);
	factor = 2;
	while (factor < 26)
	{
		if (factor % 2 == 0)
			res -= ((1.0 / factor) * sin(x * factor));
		else	
			res += ((1.0 / factor) * sin(x * factor));
		factor++;
	}
	return (res);
}

double kick_instrument(double frequency, float tc)
{
	return (sin(tc /44100.0 * frequency * 2.0 * M_PI));
}

double snare_instrument(double frequency, float tc)
{
	return (sin(tc /44100.0 * frequency * 2.0 * M_PI));
}

//double	(*fn_ptrs[ctx.tracks.len])(double frequency, float tc);


//sin_x + ((pow(3, -2)) * (0 - (sin_x * 3))) + ((pow(5, -2)) * (sin_x * 5)) + ((pow(7, -2)) * (sin_x * 7)) + ((pow(9, -2)) * (sin_x * 9)) + ((pow(11, -2)) * (sin_x * 11));
