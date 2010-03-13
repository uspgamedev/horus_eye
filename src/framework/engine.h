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
using std::vector;

namespace framework {

class Engine {
  public:
    // Engine e' um singleton. Use Engine::reference()
    // para obter um ponteiro para o objeto desta classe.
    static Engine* reference() { static Engine *r = NULL;
                                 return r ? r : r = new Engine; }

    // Gerenciamento do motor
    bool Initialize();
    void Run();
    bool Release();

    // Gerenciamento de cenas
    void PushScene(Scene* scene);
    Scene* CurrentScene() const;
    void PopScene();

    // Saida do motor
    void quit() { quit_ = true; }

    // outros
    ~Engine() {}

  private:
    Engine() : quit_(false) {}
    bool quit_;
    vector<Scene*> scene_list;
};

}  // namespace framework

#endif

