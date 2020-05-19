#ifndef __AUDIO_HPP__
#define __AUDIO_HPP__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <inttypes.h>

#ifdef WIN32
    #include <process.h>
#endif

#ifdef UNIX
    #include <unistd.h>
#endif

#include <stdbool.h>

#include <AL/al.h>
#include <AL/alc.h>

#include <vector>
#include <string>
#include "debug.hpp"


#define NUM_EFFECT_SOURCES 16


#define TEST_ERROR(_msg)		\
	error = alGetError();		\
	if (error != AL_NO_ERROR) {	\
		fprintf(stderr, _msg "\n");	\
		return -1;		\
	}


// a class for handling the audio
// OpenAL-soft is used for playback
class Audio {
    public:
        Audio();
        ~Audio();
        int init();
        std::vector<ALuint> soundBuffers;
        std::vector<ALuint> soundSources = std::vector<ALuint>(NUM_EFFECT_SOURCES, 0);
        int errorcode = 0;
        int PlayEffect(ALuint buffer, ALuint* sourcenum);
        int StopEffect(ALuint sourcenum);
        int get_source_state(ALuint source, ALint* source_state);

    private:
        //ALboolean enumeration;
        //const ALCchar *devices;
        //
        int init_Sounds();
        int setup_listener();
        const std::vector<std::string> soundPaths = {
            "resources/intro.wav",
            "resources/starquake_death.wav",
            "resources/brrr.wav",
            "resources/hero_woosh.wav",
            "resources/hero_xplode.wav",
            "resources/buzz.wav",
            "resources/die.wav"};
        int Load(const std::string audiofilepath, ALuint buffer);
        const ALCchar *defaultDeviceName = "OpenAL Soft";
        unsigned int sndEffectIndex = 0;
        int ret;
        ALCdevice *device;
        ALCcontext *context;
        ALfloat listenerOri[6] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
        ALvoid *data;
        ALsizei size, freq;
        ALenum format;
        ALCenum error;

};

class SoundEffect {
    private:
		ALuint soundsrc;
        bool isPlaying = false;
        Audio* audio;
        ALuint soundID;

    public:
        SoundEffect(Audio* _audio, ALuint _soundID) : audio(_audio), soundID(_soundID)
        {};
        ~SoundEffect() {};

        void start() 
        {
            if (!isPlaying) {
		        this->audio->PlayEffect(soundID, &soundsrc);
                isPlaying = true;
            }
                
        };

        void check_playing()
        {
            ALint audiostate;
            int err = this->audio->get_source_state(soundsrc, &audiostate);
            if ((err > -1) && (audiostate != AL_PLAYING) ) {
                isPlaying = true;
            } else {
                isPlaying = false;
            }
        };

        void stop_playing()
        {
            alSourceStop(soundsrc);
        }
};

// a handy extra wav-loader function
char* loadWAV(const char* fn, int& chan, int& samplerate, int& bps, int& size);

#endif
