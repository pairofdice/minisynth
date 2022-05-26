# include "minisynth.h"

double sine_instrument(double frequency, float tc)
{
	return (sin(tc * frequency * 2.0 * M_PI));
}

double square_instrument(double frequency, float tc)
{
	return (sin(tc * frequency * 2.0 * M_PI));
}