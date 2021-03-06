#include "minisynth.h"

int	is_note(char *note)
{
	if (note[0] == 'a' || note[0] == 'b'|| note[0] == 'c' || note[0] == 'd'
			|| note[0] == 'e' || note[0] == 'f'  || note[0] == 'g' || note[0] == 'r')
	{
		if (note[1] == '\0' || note[1] == '\n' || note[1] == '/' || ft_isdigit(note[1]))
			return (1);
	}
	return (0);
}

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
		ft_putstr("Usage: ./minisynth <.synth file>\n");
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

void	get_song_duration(t_context *ctx)
{
	//printf("HELLOOOOOOOOO\n");
	int i = 0;
	double result = 0;
	t_vec track;
	t_note note;

	track = *(t_vec *)vec_get(&ctx->tracks, 0);
	while (i < track.len)
	{
		note = *(t_note *)vec_get(&track, i);
		//printf("%f \n", note.duration);
		result += note.duration;
		i++;
	}
	ctx->song_duration = result;
		//printf("song duration: %f\n", result);

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

void	process_note(t_note *note, char ***notes, t_vec *linevec, int not_nl)
{
// <pitch>[<alteration>][<octave>][/<duration>]
//	pitch		a to g or r		-mandatory
//	alteration	# or b			-optional
//	octave		0 to 9			-optional
//	duration	/0.0			-optional
	//if no octave or duration value given, take value from prev note, on that line.
	//if first note on that line and no value specifed, octave = 4 and duration = 1
	static int 		prev_octave;
	static float	prev_duration;
	char			*s;
	double			n, d;
	int				i, oct_parsed, dur_parsed;


	s = **notes;
//	printf("%s %d\n", s, not_nl);
	n = 0;
	d = 0;
	oct_parsed = dur_parsed = FALSE;
	s = **notes;
	if (ft_isalpha(s[0]))
	{
		n = initialise_note(s[0]);		
		i = 1;
		while (s[i])
		{
			if (ft_isdigit(s[i]))
			{	
				prev_octave = ft_atoi(s + i);
				n *= pow(2, prev_octave);
				oct_parsed = TRUE;
			}
			else if (s[i] == 'b')
				n *= N_FL;
			else if (s[i] == '#')
				n *= N_SH;
			else if (s[i] == '/')
			{	
				d = set_duration(s + i + 1);
				dur_parsed = TRUE;
				prev_duration = d;
				break ;
			}
			i++;
		}
		//if (!not_nl)
		//	printf("\nNEWLINE\n\n");
		if (!dur_parsed && not_nl)
			d = prev_duration;
		else if (!dur_parsed && !not_nl)
			d = prev_duration = 1;
		if (!oct_parsed && not_nl)
			n *= pow(2, prev_octave);
		else if (!oct_parsed && !not_nl)
		{	
			n *= pow(2, 4);
			prev_octave = 4;
		}
		//printf("%s %g %g\n ", **notes, n, d);
		note->frequency = n;
		note->duration = d;
		note->duration *= 60.0/tempo;
		vec_push(linevec, note);
		//prev_octave = prev_duration = 0;
	}
}

int	load_file(int fd, t_context *ctx)
{
	char	*line;
	char	**notes;
	char	**temp;
	t_note	note;
	t_vec	linevec;
	t_vec	*test;
	int setup, not_newline;
	int track_n;
	int appending;

	setup = 0;

	vec_new(&ctx->tracks, BUFF_SIZE * 2, sizeof(t_vec));

	while (get_next_line(fd, &line))
	{
		if (setup < 2)
		{
			if (ft_strncmp("tempo", line, 5) == 0)
			{
				tempo = ft_atoi(&line[6]);
				setup++;
				continue ;
			}
			if (ft_strncmp("tracks", line, 6) == 0)
			{
				ctx->instruments = ft_strsplit(line + 7, ',');
				setup++;
				continue ;
			}
		}
	
		if (ft_isdigit(*line))
		{
			test = NULL;
			track_n = ft_atoi(line);
			//printf("%d %zu \n", track_n, ctx->tracks.len);
			notes = ft_strsplit(line, ' ');
			temp = notes;
			//printf("%s\n", *notes);
			if (*notes)
			{	
				appending = track_n <= ctx->tracks.len;
				if (appending)
					linevec = *(t_vec *)vec_get(&ctx->tracks, track_n - 1);
				else
					vec_new(&linevec, ft_strlen(line) / 2 + 1, sizeof(t_note));
				notes++;
				not_newline = FALSE;
				while (*notes != 0)
				{
					if (is_note(*notes))
					{
						 /* if (appending)
							process_note(&note, &notes, test, not_newline);
						else  */
							process_note(&note, &notes, &linevec, not_newline);
						if (!not_newline)
							not_newline = TRUE;
					}
					notes++;
				}
	/* 			 int i = 0;
				while (i < linevec.len)
				{	
					printf("%c %g \n", *line, *(double *)vec_get(&linevec, i));
					i++;
				}
					printf("---\n");  */
				if (linevec.len > 0)
				{
					if (!appending)
						vec_push(&ctx->tracks, &linevec);
				}
				free_array((void *)&temp);
				ft_strdel(&line);
			}
		}

	}
	get_song_duration(ctx);
	return (1);
}
