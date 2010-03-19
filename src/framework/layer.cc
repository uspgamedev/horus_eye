//
// Horus Eye - Framework
// Copyright (C) 2010  Nucleo de Desenvolvimento de Jogos da USP
//
// framework/layer.h
// Implementacao da classe Layer.
//

#include "layer.h"
#include "engine.h"

namespace framework {

using namespace std;

void Layer::Update(float delta_t) {

    list<Sprite*>::iterator it = sprite_list_.begin();

    while (it != sprite_list_.end()) {
        (*it)->Update(delta_t);
        ++it;
    }
}

void Layer::Render() {

    list<Sprite*>::iterator it = sprite_list_.begin();

        while (it != sprite_list_.end()) {
            (*it).render(Engine::reference->video_manager()->backbuffer());
            ++it;
        }
}

}
