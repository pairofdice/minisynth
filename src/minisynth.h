#ifndef MINISYNTH_H
# define MINISYNTH_H

# include "../libft/libft.h"
# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include "../frameworks/SDL2/SDL.h"

# include	<stdio.h>

/* enum {
	BUFF_SIZE = 42
}; */

# define N_C 16.35
# define N_D 18.35
# define N_E 20.60
# define N_F 21.83
# define N_G 24.50
# define N_A 27.50
# define N_B 30.87
# define N_SH 1.059463094359
# define N_FL 0.943874312682


typedef struct s_context
{
	int			tempo;
	int			*instruments;
	t_vec		tracks;
}	t_context;

typedef struct s_note
{
	double	frequency;
	double	duration;
}	t_note;


int	handle_args(int argc, char **argv, t_context *ctx);
int	load_file(int fd, t_context *ctx);
int16_t sine_instrument(double frequency, int time);

#endif