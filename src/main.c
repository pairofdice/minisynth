#include "minisynth.h"

int close_synth()
{
	return (0);
}


int main(int argc, char **argv)
{
	t_context ctx;

	handle_args(argc, argv, &ctx);
		

	size_t i = 0;
	t_vec *line_vec;
	t_note note;
		printf("YOOO\n");

	while (i < ctx.tracks.len)
	{
		line_vec = (t_vec *)vec_get(&ctx.tracks, i);
		size_t k = 0;
		while (k < line_vec->len)
		{
			//p = *(char *)vec_get(line_vec, k++);
			note = *(t_note *)vec_get(line_vec, k);
			printf("%c", note.frequency);
			k++;
		}
		ft_putchar('\n');
		i++;
	}
		ft_putchar('\n');
}
