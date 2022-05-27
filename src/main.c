#include "minisynth.h"

int close_synth()
{
	return (0);
}

void  insert_into_buffer(SDL_AudioDeviceID *audio_device, t_context *ctx, int track_n)
{
    t_vec track;
    track = *(t_vec *)vec_get(&ctx->tracks, track_n);
    t_note note;
    note = *(t_note *)vec_get(&track, ctx->track_pos[track_n]);
        double	(*fn_ptrs[6])(double frequency, float tc);

    fn_ptrs[0] = sine_instrument;
    fn_ptrs[1] = square_instrument;
    fn_ptrs[2] = triangle_instrument;
    fn_ptrs[3] = saw_instrument;
    fn_ptrs[4] = kick_instrument;
    fn_ptrs[5] = snare_instrument;
    ctx->buffer_time = 0;
    while (ctx->buffer_time < 44100 && ctx->track_pos[track_n] < track.len )
    {
        printf("%f\n", note.duration);
        ctx->buffer[ctx->buffer_time] += sine_instrument(note.frequency, ctx->time + ctx->buffer_time);
        ctx->buffer_time++;
        note.duration -= (1.0  / 44100 );
        if (note.duration <= 0)
        {
            ctx->track_pos[track_n]++;
            note = *(t_note *)vec_get(&track, ctx->track_pos[track_n]);
        }
    }
}

void  init_instruments(t_context *ctx)
{
    int i = 0;
    while (i < ctx->tracks.len)
    {
        if (ft_strncmp(ctx->instruments[i], "sine", 4))
            ctx->track_instru[i] = 0;
        if (ft_strncmp(ctx->instruments[i], "square", 6))
            ctx->track_instru[i] = 1;
        if (ft_strncmp(ctx->instruments[i], "triangle", 8))
            ctx->track_instru[i] = 2;
        if (ft_strncmp(ctx->instruments[i], "saw", 3))
            ctx->track_instru[i] = 3;
        if (ft_strncmp(ctx->instruments[i], "kick", 4))
            ctx->track_instru[i] = 4;
        if (ft_strncmp(ctx->instruments[i], "snare", 5))
            ctx->track_instru[i] = 5;
        i++;
    }
}

void    play_song(SDL_AudioDeviceID *audio_device, SDL_AudioSpec *audio_spec, t_context *ctx)
{
    int i;
    ctx->buffer = malloc(44100 * sizeof(double));
    ctx->track_pos = malloc(ctx->tracks.len * (sizeof(size_t) + 1));
    ctx->track_instru = malloc(ctx->tracks.len * (sizeof(int) + 1));
    init_instruments(ctx);
     
    bzero(ctx->track_pos, ctx->tracks.len * (sizeof(size_t) + 1));

    ctx->time = 0;
    while (ctx->time < (ctx->song_duration  * audio_spec->freq))
    {
       // printf("%d\n", ctx->time);
        bzero(ctx->buffer, 44100 * sizeof(double));
        i = 0;
        while (i < ctx->tracks.len)
        {
            // printf("%zu %f %d %f\n",ctx->tracks.len, ctx->song_duration, audio_spec->freq, ctx->song_duration  * audio_spec->freq);
            insert_into_buffer(audio_device, ctx, i);
            i++;
        }
        int k = 0;
        int16_t temp;
        while (k < 44100)
        {
            temp = ctx->buffer[k]  *  5000;
            SDL_QueueAudio(*audio_device, &temp, sizeof(int16_t));
            k++;
        }
        ctx->time += audio_spec->freq;
    }
}

int main(int argc, char **argv)
{
	t_context ctx;
	size_t i = 0;
	t_vec *line_vec;


	handle_args(argc, argv, &ctx);
	SDL_Init(SDL_INIT_AUDIO);
    SDL_AudioDeviceID audio_device;
    SDL_AudioSpec audio_spec;
    SDL_zero(audio_spec);
    audio_spec.freq = 44100;
    audio_spec.format = AUDIO_S16SYS;
    audio_spec.channels = 1;
    audio_spec.samples = 1024;
    audio_spec.callback = NULL;
    audio_device = SDL_OpenAudioDevice(NULL, 0, &audio_spec, NULL, 0);

    play_song(&audio_device, &audio_spec, &ctx);

    SDL_PauseAudioDevice(audio_device, 0);
    SDL_Delay( (int)ctx.song_duration *  1000);
    SDL_CloseAudioDevice(audio_device);
    SDL_Quit();


    int n = 0;
    t_vec track;
    t_note no;
    while (n < ctx.tracks.len)
    {
        track = *(t_vec *)vec_get(&ctx.tracks, n);
        int k = 0;
        while (k < track.len)
        {
            no = *(t_note *)vec_get(&track, k);
            printf("%g %g\n", no.frequency, no.duration);

            k++;
        }
        n++;
            printf("\n");
    }  
}
