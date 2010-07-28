//
// Horus Eye - Framework
// Copyright (C) 2010  Nucleo de Desenvolvimento de Jogos da USP
//
// framework/scene.h
// Definicao da classe Scene.
//

#ifndef HORUSEYE_FRAMEWORK_SCENE_H_
#define HORUSEYE_FRAMEWORK_SCENE_H_

#include "layer.h"
#include <list>

namespace framework {

// Classe Scene
// Representa uma CENA, que contem varias camadas de graficos.
// Uma cena e' uma "tela" inteira do jogo, como por exemplo o main menu,
// ou o jogo em si (onde ocorre a acao).
class Scene {
  public:
    // The Evul Constructor!
    Scene() : finished_(false) {}
    // The Good Destructor!
    virtual ~Scene();

    // Funcoes basicas pra adicionar e remover layers.
    void AddLayer(Layer *layer) { layers_.push_back(layer); };
    void RemoveLayer(Layer *layer) { layers_.remove(layer); };

    // Termina a cena.
    void Finish() { End(); finished_ = true; }

    // Atualiza a logica da cena.
    virtual void Update(float delta_t);

    // Redesenha o grafico da cena.
    void Render();

    // Getters.
    bool finished() { return finished_; }

  protected:

    // Encerra a atividade da cena.
    virtual void End() {}

    // Lista das camadas.
    ::std::list<Layer*> layers_;
    // Bool que diz se a cena esta' terminada ou nao.
    bool finished_;

}; // class Scene.

}

#endif /* HORUSEYE_FRAMEWORK_SCENE_H_ */
