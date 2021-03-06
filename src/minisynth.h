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
# define TRUE 1
# define FALSE 0

int tempo;

typedef struct s_context
{
	double		song_duration;
	int			tempo;
	uint		buffer_time;
	char		**instruments;
	t_vec		tracks;
	double		*buffer;
    size_t		*track_pos;
	int			*track_instru;
    t_vec		track;
    uint		time;
}	t_context;

typedef struct s_note
{
	double	frequency;
	double	duration;
	char	note;
}	t_note;


int	handle_args(int argc, char **argv, t_context *ctx);
int	load_file(int fd, t_context *ctx);
double sine_instrument(double frequency, float time);
double square_instrument(double frequency, float tc);
double triangle_instrument(double frequency, float tc);
double saw_instrument(double frequency, float tc);
double kick_instrument(double frequency, float tc);
double snare_instrument(double frequency, float tc);
#endif
