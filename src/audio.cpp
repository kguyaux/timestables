#include "audio.hpp"


Audio::Audio()
{
    int returnval = init();
    if (returnval < 0) {
        log_err("Audio init failed");
        errorcode = -1;
    }
}


int Audio::init()
{
    // Initialize the audiodevice ("OpenAl Soft")
    device = alcOpenDevice(defaultDeviceName);
	if (!device) {
		fprintf(stderr, "unable to open default device\n");
		return -1;
	}

	fprintf(stdout, "Device: %s\n", alcGetString(device, ALC_DEVICE_SPECIFIER));
	alGetError(); // clear error state?

    //... and create a default context, and use it..
	context = alcCreateContext(device, NULL);
	if (!alcMakeContextCurrent(context)) {
		fprintf(stderr, "failed to make default context\n");
		return -1;
	}
	TEST_ERROR("make default context");

    int soundinitsuccess = init_Sounds();
    if (soundinitsuccess < 0)
        return -1;

    return 0;
}


int Audio::init_Sounds()
{
    for (unsigned int i = 0; i < soundPaths.size(); i++) {
        ALuint buffer; 
        alGenBuffers(1, &buffer); // we generate a buffer one at a time, since amount is not a const
        TEST_ERROR("buffer generation");

        Load(soundPaths[i], buffer);
        // tsja, if it fails, just add the buffer.. althiugh it is unusable .. make better in future!
        soundBuffers.push_back(buffer);
    }
    return 0;
}


int Audio::Load(const std::string audiofilepath, ALuint bufferID)
{

    int channel, sampleRate, bps, size;
    char* data = loadWAV(audiofilepath.c_str(), channel, sampleRate, bps, size);
    unsigned int format;
    if (channel == 1)
    {
        if (bps == 8)
        {
            format = AL_FORMAT_MONO8;
        }
        else {
            format = AL_FORMAT_MONO16;
        }
    }
    else {
        if (bps == 8)
        {
            format = AL_FORMAT_STEREO8;
        }
        else {
            format = AL_FORMAT_STEREO16;
        }
    }
    alBufferData(bufferID, format, data, size, sampleRate);
    delete[] data;
	return 0;
}

int Audio::setup_listener()
{
	/* set orientation */
	alListener3f(AL_POSITION, 0, 0, -1.0f);
	TEST_ERROR("listener position");

    alListener3f(AL_VELOCITY, 0, 0, 0);
	TEST_ERROR("listener velocity");

	alListenerfv(AL_ORIENTATION, listenerOri);
	TEST_ERROR("listener orientation");

    return 0;
}

int Audio::PlayEffect(ALuint buffer, ALuint* sourcenum)
{
    ALuint source;
    if (sndEffectIndex == NUM_EFFECT_SOURCES)
        sndEffectIndex = 0;
    if (soundSources[sndEffectIndex] > 0)
	    alDeleteSources(1, &soundSources[sndEffectIndex]);

    alGenSources((ALuint)1, &source);
    TEST_ERROR("source generation");
    //printf("now using source %d\n", source);
    *sourcenum = source;
    soundSources[sndEffectIndex] = source;
    sndEffectIndex++;

	alSourcef(source, AL_PITCH, 1.0);
	TEST_ERROR("source pitch");
	alSourcef(source, AL_GAIN, 1);
	TEST_ERROR("source gain");
	alSource3f(source, AL_POSITION, 1, 0, 0);
	TEST_ERROR("source position");
	alSource3f(source, AL_VELOCITY, 0, 0, 0.0);
	TEST_ERROR("source velocity");
	alSourcei(source, AL_LOOPING, AL_FALSE);
	TEST_ERROR("source looping");

    // bind the buffer to the source...
	alSourcei(source, AL_BUFFER, buffer);
	TEST_ERROR("buffer binding");

    // and play it
	alSourcePlay(source);
	TEST_ERROR("source playing");
    return 0;
}

int Audio::StopEffect(ALuint sourcenum)
{
    alSourceStop(sourcenum);
    TEST_ERROR("stop playing soundeffect");
    return 0;
}


int Audio::get_source_state(ALuint source, ALint* source_state)
{
	alGetSourcei(source, AL_SOURCE_STATE, source_state);
	TEST_ERROR("source state get");
    return 0;
}



Audio::~Audio()
/* Destructor for the audio-class, cleanup the audiostuff*/
{
    for (int i= 0; i < NUM_EFFECT_SOURCES; i++) {
        if (soundSources[i] > 0) {
            //printf("Freeing source: %d\n", soundSources[i]);
            alDeleteSources(1, &soundSources[i]);
        }
    }

    for (auto sbuf = soundBuffers.begin(); sbuf != soundBuffers.end(); ++sbuf) {
        ALuint bbb = *sbuf;
        //printf("Freeing buffer: %d\n", bbb);
	    alDeleteBuffers(1, &bbb);
    }

	/* exit context */
	device = alcGetContextsDevice(context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);
}



