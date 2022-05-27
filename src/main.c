#include "minisynth.h"

int close_synth()
{
	return (0);
}

double  insert_into_buffer(t_note *note, int16_t *buffer,  uint  time, uint  buffer_time, int freq)
{
    uint  buffer_time;
    while (buffer_time < freq)
    {
        sine_instrument(note->frequency, time)
    }
    return note->duration - (freq - buffer_time) /(float)freq;
}

void    play_song(SDL_AudioDeviceID *audio_device, SDL_AudioSpec *audio_spec, t_context *ctx)
{
    t_note note;
    int16_t buffer[audio_spec->freq];
    size_t track_pos[ctx->tracks.len];
    t_vec track;
    uint  time;
    uint  buffer_time;
    t_note  note;

    int i =0;
    while (i  < ctx->tracks.len)
    {
        track_pos[i] = 0;
        i++;
    }
    while (time < (ctx->song_duration  * audio_spec->freq))
    {
        i = 0;
        while (i < ctx->tracks.len)
        {
            buffer_time = time % audio_spec->freq;
            track = *(t_vec *)vec_get(&ctx->tracks, i);
            note = *(t_note *)vec_get(&track, track_pos[i]);

            note.duration = insert_into_buffer(&note, &buffer, buffer_time, audio_spec->freq);
            if (note.duration <= 0)
                track_pos[i]++;
            i++;
        }

        time++;
    }    
    note.duration = 0.5;
    note.frequency = 440.0;
	float tc;
    for (int i = 0; i < audio_spec->freq * note.duration; i++) {

        // SDL_QueueAudio expects a signed 16-bit value
        // note: "5000" here is just gain so that we will hear something
       //int16_t sample = sin(i * 0.04) * 5000;
       tc = (float)i / (float)audio_spec->freq;
       int16_t sample = sine_instrument(note.frequency, tc) * 5000;
       //printf("%d %d %f %hd\n",i, audio_spec.freq, tc, sample);
        SDL_QueueAudio(audio_device, &sample, sizeof(int16_t));
    }
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
    audio_device = SDL_OpenAudioDevice(
        NULL, 0, &audio_spec, NULL, 0);
    // pushing 3 seconds of samples to the audio buffer:

    play_song(&audio_device, &audio_spec, &ctx);

    // unpausing the audio device (starts playing):
    SDL_PauseAudioDevice(audio_device, 0);
    SDL_Delay(ctx.song_duration);
    SDL_CloseAudioDevice(audio_device);
    SDL_Quit();


/*     int n = 0;
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
    } */
}
