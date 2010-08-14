//
// Horus Eye - Framework
// Copyright (C) 2010  Nucleo de Desenvolvimento de Jogos da USP
//
// framework/engine.h
// Definicao da classe Engine (motor do jogo).
//
#ifndef HORUSEYE_FRAMEWORK_ENGINE_H_
#define HORUSEYE_FRAMEWORK_ENGINE_H_

#include <vector>
#include "inputmanager.h"
#include "videomanager.h"
#include "audiomanager.h"
#include "scene.h"
#include "timehandler.h"

using std::vector;

namespace framework {

class Engine {
  public:
    // Engine e' um singleton. Use Engine::reference()
    // para obter um ponteiro para o objeto desta classe.
    static Engine* reference() { static Engine *r = NULL;
                                 return r ? r : r = new Engine; }

    VideoManager *video_manager() { return video_manager_; }
    InputManager *input_manager() { return input_manager_; }
    TimeHandler *time_handler() { return time_handler_; }
    AudioManager *audio_manager() { return audio_manager_; }
    Vector2D window_size() { return window_size_; }

    // Gerenciamento do motor
    bool Initialize(string windowTitle, Vector2D windowSize, bool fullscreen);

    void Run();
    void Release();

    // Gerenciamento de cenas
    void PushScene(Scene* scene);
    Scene* CurrentScene() const;
    void PopScene();

    int current_fps() { return reported_fps_; }

    // Saida do motor
    void quit() { quit_ = true; }

    // outros
    ~Engine() {}

  private:
    VideoManager *video_manager_;
    InputManager *input_manager_;
    TimeHandler *time_handler_;
    AudioManager *audio_manager_;
    Vector2D window_size_;
    bool quit_;
    vector<Scene*> scene_list_;
    int reported_fps_, frames_since_reset_, last_fps_report_;
};

}  // namespace framework

#endif

