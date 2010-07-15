//
// Horus Eye - Framework
// Copyright (C) 2010  USPGameDev
//
// framework/audiomanager.h
// Definicao da classe de gerenciamento de audio.
//

// tutoriais de SDL Mixer:
// http://gpwiki.org/index.php/C:Playing_a_WAV_Sound_File_With_SDL_mixer
// http://www.kekkai.org/roger/sdl/mixer/

#ifndef HORUSEYE_FRAMEWORK_AUDIOMANAGER_H_
#define HORUSEYE_FRAMEWORK_AUDIOMANAGER_H_

#include <string>
#include <map>

#define AUDIO_MANAGER() framework::Engine::reference()->audio_manager()

namespace framework {

class Sample;
class Music;

// Gerenciador de audio
class AudioManager {
  public:
    AudioManager();
    ~AudioManager();

    bool Initialize();
    bool Release();
    void Update();

    Sample* LoadSample(const std::string& filepath);
    Music* LoadMusic(const std::string& filepath);

  private:
    std::map<std::string, Sample*> sample_data_;
    std::map<std::string, Music*> music_data_;
    static const int NUM_CHANNELS = 16;

    void ReleaseSamples();
    void ReleaseMusics();
};

}  // namespace framework

#endif
