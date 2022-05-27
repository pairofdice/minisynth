#include "minisynth.h"

int close_synth()
{
	return (0);
}


int main(int argc, char **argv)
{
	t_context ctx;
	size_t i = 0;
	t_vec *line_vec;
	t_note note;

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
    note.duration = 3;
    note.frequency = 220.0;
	float tc;
    for (int i = 0; i < audio_spec.freq * note.duration; i++) {

        // SDL_QueueAudio expects a signed 16-bit value
        // note: "5000" here is just gain so that we will hear something
       //int16_t sample = sin(i * 0.04) * 5000;
       tc = (float)i / (float)audio_spec.freq;
       int16_t sample = saw_instrument(note.frequency, tc) * 5000;
	   printf("%d %d %f %hd\n",i, audio_spec.freq, tc, sample);
       SDL_QueueAudio(audio_device, &sample, sizeof(int16_t));
    }
    // unpausing the audio device (starts playing):
    SDL_PauseAudioDevice(audio_device, 0);
    SDL_Delay(3000);
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
