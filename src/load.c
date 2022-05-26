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
	float		n;
	int			i;
	char *s;

	s = **notes;

	n = 0;
	if (ft_isalpha(s[0]))
	{

				note->note = *s;
				//if ()
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
	int setup;

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
				read_instruments();
				setup++;
				continue ;
			}
		}
		vec_new(&linevec, ft_strlen(line) / 2 + 1, sizeof(t_note));
		notes = ft_strsplit(line, ' ');
		temp = notes;
		//notes++;
		while (*notes != 0)
		{
			process_note(&note, &notes, &linevec);
			notes++;
		}
		if (linevec.len > 0)
			vec_push(&ctx->tracks, &linevec);
		free_array((void *)&temp);
		ft_strdel(&line);
	}
	return (1);
}
