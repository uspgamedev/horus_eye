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

    // Gerenciamento do motor
    bool Initialize();
    void Run();
    void Release();

    // Gerenciamento de cenas
    void PushScene(Scene* scene);
    Scene* CurrentScene() const;
    void PopScene();

    // Saida do motor
    void quit() { quit_ = true; }

    // outros
    ~Engine() {}

  private:
    VideoManager *video_manager_;
    InputManager *input_manager_;
    TimeHandler *time_handler_;
    bool quit_;
    vector<Scene*> scene_list_;
};

}  // namespace framework

#endif
