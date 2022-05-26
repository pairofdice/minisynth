#include "minisynth.h"

int	handle_args(int argc, char **argv, t_context *ctx)
{
	int	fd;

	if (argc == 2)
	{
		fd = open(argv[1], O_RDONLY);
		if (fd < 0)
			exit(1);
	}
	else
	{
		ft_putstr("Usage: ./fdf <map>\n");
		exit(1);
	}
	load_file(fd, ctx);
	close(fd);
	return (1);
}

 static void	free_array(void **array)
{
	void	**temp;

	temp = *array;
	while (*temp)
		ft_memdel(temp++);
	free(*array);
	*array = NULL;
}

float	initialise_note(char note)
{
	float	res;
	switch (note)
	{
		case 'a':
			res = N_A;
			break;
		case 'b':	
			res = N_B;	
			break;
		case 'c':
			res = N_C;
			break;
		case 'd':	
			res = N_D;	
			break;
		case 'e':
			res = N_E;
			break;
		case 'f':	
			res = N_F;	
			break;
		case 'g':
			res = N_G;
			break;
		case 'r':	
			res = 0;	
			break;
	}
	return (res);
}

float	set_duration(char *s)
{
	float res;
	char *point;

	res = (float) ft_atoi(s);
	point = ft_strchr(s, '.');
	if (point)
		res += (ft_atoi(point + 1) / pow(10, ft_strlen(point + 1)));
	return (res);
}

void	process_note(t_note *note, char ***notes, t_vec *linevec)
{
// <pitch>[<alteration>][<octave>][/<duration>]
//	pitch		a to g or r		-mandatory
//	alteration	# or b			-optional
//	octave		0 to 9			-optional
//	duration	/0.0			-optional
	//if no octave or octave value given, take value from prev note, on that line.
	//if first note on that line and no value specifed, octave = 4 and duration = 1
	static int 	prev_octave;
	static int	prev_duration;
	static int	newline_start;
	char		*s;
	float		n, d;
	int			i;

	n = 0;
	d = 0;
	s = **notes;
	if (ft_isalpha(s[0]))
	{
		n = initialise_note(s[0]);		
		i = 1;
		while (s[i])
		{
			if (ft_isdigit(s[i]))
				n *= pow(2, ft_atoi(s + i));
			else if (s[i] == 'b')
				n *= N_FL;
			else if (s[i] == '#')
				n *= N_SH;
			else if (s[i] == '/')
			{	
				d = set_duration(s + i + 1); 
				break ;
			}
			i++;
		}
		//printf("%s %f\n ", **notes, d);
		note->frequency = n;
		note->duration = d;
		vec_push(linevec, note);
	}
	
}

int	load_file(int fd, t_context *ctx)
{
	char	*line;
	char	**notes;
	char	**temp;
	t_note	note;
	t_vec	linevec;

	vec_new(&ctx->tracks, BUFF_SIZE * 2, sizeof(t_vec));

	while (get_next_line(fd, &line))
	{
		if (!ft_isdigit(*line))
			continue;
		vec_new(&linevec, ft_strlen(line) / 2 + 1, sizeof(t_note));
		notes = ft_strsplit(line, ' ');
		temp = notes;
		while (*notes != 0)
		{
			process_note(&note, &notes, &linevec);
			notes++;
		}
		if (linevec.len > 2)
		{
			vec_push(&ctx->tracks, &linevec);
		}
		free_array((void *)&temp);
		ft_strdel(&line);
	}
	return (1);
}
