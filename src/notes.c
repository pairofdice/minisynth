# include "minisynth.h"

int16_t sine_instrument(double frequency, int i)
{
	return (sin(((float)i/ frequency) * 2.0 * M_PI));
	
}