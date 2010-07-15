//
// Horus Eye - Framework
// Copyright (C) 2010  USPGameDev
//
// framework/music.h
// Classe Music: representa uma musica
//

#ifndef HORUSEYE_FRAMEWORK_MUSIC_H_
#define HORUSEYE_FRAMEWORK_MUSIC_H_

#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

namespace framework {

class AudioManager;

// Musica
class Music {
  public:
    void Play();
    void Play(int loops);
    void Stop();
    bool IsPlaying();
    void Pause();
    void Resume();

    void SetVolume(float vol); // 0.0f (quiet) <= vol <= 1.0f (loud)
    float Volume();

  private:
    Music(const std::string& filepath);
    ~Music();
    Mix_Music *data;
    static void MusicDone();

  friend class AudioManager;
};

}  // namespace framework

#endif
