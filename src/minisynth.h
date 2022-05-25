#ifndef MINISYNTH_H
# define MINISYNTH_H

# include "../libft/libft.h"
# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>

# include	<stdio.h>

/* enum {
	BUFF_SIZE = 42
}; */

typedef struct s_context
{
	int			tempo;
	int			*instruments;
	t_vec		tracks;
}	t_context;

typedef struct s_note
{
	char	frequency;
	double	duration;
}	t_note;


int	handle_args(int argc, char **argv, t_context *ctx);
int	load_file(int fd, t_context *ctx);

#endif