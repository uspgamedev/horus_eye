//
// Horus Eye - Framework
// Copyright (C) 2010  Nucleo de Desenvolvimento de Jogos da USP
//
// framework/engine.cc
// Implementacao da classe Engine (motor do jogo).
//
#include <SDL/SDL.h>
#include "engine.h"
#include <vector>

namespace framework
{

bool Engine::Initialize() {
    quit_ = false;
    video_manager_ = new VideoManager();
    SDL_Init(SDL_INIT_EVERYTHING);
    video_manager_->Initialize("Framework", Vector2D(640,480), false);

    input_manager_ = new InputManager();
    time_handler_ = new TimeHandler();
    scene_list_.clear();
    return (time_handler_ != NULL);
}

void Engine::Run() {
    time_handler_->Update();
    while(!quit_) {
        if (scene_list_.size() == 0) {
            break;
        }
        for (int i = 0; i < static_cast<int>(scene_list_.size()); i++) {
            scene_list_[i]->Render();
        }
        CurrentScene()->Update(time_handler_->TimeDifference());
    }
}

void Engine::Release() {
    for (int i = 0; i < static_cast<int>(scene_list_.size()); i++) {
        delete scene_list_[i];
    }
    delete time_handler_;
    scene_list_.clear();
}

void Engine::PushScene(Scene* scene) {
    scene_list_.push_back(scene);
}

Scene* Engine::CurrentScene() const {
    return scene_list_[scene_list_.size() - 1];
}

void Engine::PopScene() {
    scene_list_.pop_back();
}
}
