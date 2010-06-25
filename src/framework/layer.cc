//
// Horus Eye - Framework
// Copyright (C) 2010  Nucleo de Desenvolvimento de Jogos da USP
//
// framework/layer.h
// Implementacao da classe Layer.
//

#include <algorithm>
#include "layer.h"
#include "engine.h"

namespace framework {

void Layer::Update(float delta_t) {

    std::vector<Sprite*>::iterator it = sprite_list_.begin();

    SortSprites();

    while (it != sprite_list_.end()) {
        (*it)->Update(delta_t);
        ++it;
    }
}

void Layer::Render() {

    std::vector<Sprite*>::iterator it = sprite_list_.begin();

    if(visible_)
        while (it != sprite_list_.end()) {
            (*it)->Render(Engine::reference()->video_manager()->backbuffer(),
                          offset_);
            ++it;
        }
}

void Layer::AddSprite(Sprite* sprite)
{
    if(sprite != NULL)
        sprite_list_.push_back(sprite);
}

void Layer::RemoveSprite(Sprite* sprite)
{
    // nao use RemoveSprite() se estiver iterando pela sprite_list_!
    std::vector<Sprite*>::iterator it;

    it = std::find(sprite_list_.begin(), sprite_list_.end(), sprite);
    if(it != sprite_list_.end())
        sprite_list_.erase(it);
}

void Layer::SortSprites() {
    // ordena sprites pelo valor de zindex
    std::sort(sprite_list_.begin(), sprite_list_.end(), Sprite::CompareByZIndex);
}

}
