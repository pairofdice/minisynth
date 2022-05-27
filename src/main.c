#include "minisynth.h"

int close_synth()
{
	return (0);
}

void  insert_into_buffer(SDL_AudioDeviceID *audio_device, t_context *ctx, int i)
{
    double sample;
    //uint local_time;
    t_vec track;
    track = *(t_vec *)vec_get(&ctx->tracks, i);
    t_note note;
    note = *(t_note *)vec_get(&track, ctx->track_pos[i]);
   // local_time = ctx->buffer_time = 0;
    //printf("%d %zu\n", ctx->buffer_time, ctx->track_pos[i]);
    //printf("%f\n", note.duration);
    ctx->buffer_time = 0;
    while (ctx->buffer_time < 44100 && ctx->track_pos[i] < track.len - 1 )
    {
      // printf("%g %g\n", note.frequency,  ctx->time , ctx->buffer_time));
      // printf("%g %g\n", note.frequency, sine_instrument(note.frequency, ctx->time + ctx->buffer_time));
        ctx->buffer[ctx->buffer_time] += sine_instrument(note.frequency, ctx->time + ctx->buffer_time);
        ctx->buffer_time++;
        note.duration -= 1.0/ 44100;
         if (note.duration <= 0)
        {
            ctx->track_pos[i]++;
            note = *(t_note *)vec_get(&track, ctx->track_pos[i]);
        } 
    }

   
   
    //printf("%d %u, %gHo\n", *buffer[local_time], time + local_time, sine_instrument(note->frequency, time + local_time));
    //*buffer[local_time] += sample;
    //*buffer[local_time] = log(*buffer[local_time]);
    //logdivide bym num_tracks;
}
/* 
void    play_song2(SDL_AudioDeviceID *audio_device, SDL_AudioSpec *audio_spec, t_context *ctx)
{
    int track_n = 0;
    t_vec track;
    t_note note;
    double	(*fn_ptrs[6])(double frequency, float tc);

    fn_ptrs[0] = sine_instrument;
    fn_ptrs[1] = square_instrument;
    fn_ptrs[2] = triangle_instrument;
    fn_ptrs[3] = saw_instrument;
    fn_ptrs[4] = kick_instrument;
    fn_ptrs[5] = snare_instrument;

    ctx->buffer = malloc(44100 * sizeof(double));
    ctx->track_pos = malloc(ctx->tracks.len * (sizeof(size_t) + 1));
    bzero(ctx->track_pos, ctx->tracks.len * (sizeof(size_t) + 1));

    ctx->time = 0;
    while (ctx->time < (ctx->song_duration  * audio_spec->freq))
    {
        ctx->buffer_time = 0;
        bzero(ctx->buffer, 44100 * sizeof(int16_t));

        while (ctx->buffer_time < audio_spec->freq)
        {
            track_n = 0;
            while (track_n < ctx->tracks.len)
            {
                track = *(t_vec *)vec_get(&ctx->tracks, track_n);
                note = *(t_note *)vec_get(&track, ctx->track_pos[track_n]);
                printf("%f %u %u - ", note.frequency,  ctx->time , ctx->buffer_time);
                printf(" note frequency%g sine %g\n", note.frequency, sine_instrument(note.frequency, ctx->time + ctx->buffer_time));
                ctx->buffer[ctx->buffer_time] += sine_instrument(note.frequency, ctx->time + ctx->buffer_time);
                note.duration -= 1.0/ 44100;
                if (note.duration <= 0)
                {
                    ctx->track_pos[track_n]++;
                    note = *(t_note *)vec_get(&track, ctx->track_pos[track_n]);
                } 
                track_n++;
            }
        }
        ctx->buffer_time++;
        int k = 0;
        int16_t temp;
        while (k < 44100)
        {
            temp = log(ctx->buffer[k] * 3000);
            SDL_QueueAudio(*audio_device, &temp, sizeof(int16_t));
            k++;
        }
        ctx->time += audio_spec->freq;
    }
    

} */

void    play_song(SDL_AudioDeviceID *audio_device, SDL_AudioSpec *audio_spec, t_context *ctx)
{
/*
    double *buffer;
    size_t *track_pos;
    t_vec track;
    uint  time;
    uint  buffer_time; */
    int i;
    ctx->buffer = malloc(44100 * sizeof(double));
    ctx->track_pos = malloc(ctx->tracks.len * (sizeof(size_t) + 1));
     
    bzero(ctx->track_pos, ctx->tracks.len * (sizeof(size_t) + 1));

    ctx->time = 0;
    while (ctx->time < (ctx->song_duration  * audio_spec->freq))
    {
        i = 0;
       // printf("%d\n", ctx->time);
        bzero(ctx->buffer, 44100 * sizeof(int16_t));
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
            temp = ctx->buffer[k] * 5000;
            //printf("%hd \n", temp);
            SDL_QueueAudio(*audio_device, &temp, sizeof(int16_t));
            k++;
        }
        ctx->time += audio_spec->freq;
    }    

	//float tc;
    //for (int i = 0; i < audio_spec->freq * note.duration; i++) {

        // SDL_QueueAudio expects a signed 16-bit value
        // note: "5000" here is just gain so that we will hear something
       //int16_t sample = sin(i * 0.04) * 5000;
    //   tc = (float)i / (float)audio_spec->freq;
    //   int16_t sample = sine_instrument(note.frequency, tc) * 5000;
       //printf("%d %d %f %hd\n",i, audio_spec.freq, tc, sample);

    
}

int main(int argc, char **argv)
{
	t_context ctx;
	size_t i = 0;
	t_vec *line_vec;


	handle_args(argc, argv, &ctx);
	SDL_Init(SDL_INIT_AUDIO);
    // the representation of our audio device in SDL:
    SDL_AudioDeviceID audio_device;
    // opening an audio device:
    SDL_AudioSpec audio_spec;
    SDL_zero(audio_spec);
    audio_spec.freq = 44100;
    audio_spec.format = AUDIO_S16SYS;
    audio_spec.channels = 1;
    audio_spec.samples = 1024;
    audio_spec.callback = NULL;
    audio_device = SDL_OpenAudioDevice(NULL, 0, &audio_spec, NULL, 0);
    // pushing 3 seconds of samples to the audio buffer:

    printf("%f", ctx.song_duration);
    float tc;
    for (int i = 0; i < 22050; i++) {

        // SDL_QueueAudio expects a signed 16-bit value
        // note: "5000" here is just gain so that we will hear something
       //int16_t sample = sin(i * 0.04) * 5000;
       tc = (float)i / (float)44100;
       int16_t sample = log(sine_instrument(440.0, tc) * 1000);
       //printf("%d %d %f %hd\n",i, audio_spec.freq, tc, sample);
            SDL_QueueAudio(audio_device, &sample, sizeof(int16_t));
    }
    play_song(&audio_device, &audio_spec, &ctx);

    // unpausing the audio device (starts playing):
    SDL_PauseAudioDevice(audio_device, 0);
    SDL_Delay( (int)ctx.song_duration *  1000);
    SDL_CloseAudioDevice(audio_device);
    SDL_Quit();


/*       int n = 0;
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
    }  */ 
}
