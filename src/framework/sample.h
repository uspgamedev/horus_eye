//
// Horus Eye - Framework
// Copyright (C) 2010  USPGameDev
//
// framework/sample.h
// Classe Sample: representa um efeito sonoro
//

#ifndef HORUSEYE_FRAMEWORK_SAMPLE_H_
#define HORUSEYE_FRAMEWORK_SAMPLE_H_

// Nao e' possivel instanciar um Sample
// diretamente.
//
// Para tocar um som, voce deve chamar
// o metodo LoadSample() do AudioManager
// (ele ja faz todo o gerenciamento de
// memoria para voce)

#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

namespace framework {

class AudioManager;

// Efeito sonoro
class Sample {
  public:
    void Play();
    void Play(int loops);
    void Stop();
    bool IsPlaying();

    void SetVolume(float vol); // 0.0f (quiet) <= vol <= 1.0f (loud)
    float Volume();

  private:
    Sample(const std::string& filepath);
    ~Sample();
    Mix_Chunk *data;
    int channel;

  friend class AudioManager;
};

}  // namespace framework

#endif
