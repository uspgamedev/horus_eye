//
// Horus Eye - Framework
// Copyright (C) 2010  USPGameDev
//
// framework/audiomanager.cc
// Classe de gerenciamento de audio.
//

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include "audiomanager.h"
#include "sample.h"
#include "music.h"

namespace framework {

AudioManager::AudioManager()
{
}

AudioManager::~AudioManager()
{
}

bool AudioManager::Initialize()
{
    // inicializa SDL_mixer
    if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096) != 0)
        return false;

    // aloca canais de audio
    Mix_AllocateChannels(NUM_CHANNELS);
    for(int i=0; i<NUM_CHANNELS; i++)
        Mix_Volume(i, MIX_MAX_VOLUME);

    // sucesso ;)
    return true;
}

bool AudioManager::Release()
{
    ReleaseSamples();
    ReleaseMusics();
    Mix_CloseAudio();

    // sucesso ;)
    return true;
}

void AudioManager::Update()
{
}

void AudioManager::ReleaseSamples()
{
}

void AudioManager::ReleaseMusics()
{
}

Sample* AudioManager::LoadSample(const std::string& filepath)
{
    // TODO
    return NULL;
}

Music* AudioManager::LoadMusic(const std::string& filepath)
{
    // TODO
    return NULL;
}

}  // namespace framework

