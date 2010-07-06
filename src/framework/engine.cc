//
// Horus Eye - Framework
// Copyright (C) 2010  Nucleo de Desenvolvimento de Jogos da USP
//
// framework/engine.cc
// Implementacao da classe Engine (motor do jogo).
//
#include <SDL/SDL.h>
#include <string>
#include <algorithm>
#include "engine.h"
using std;

namespace framework
{

bool Engine::Initialize(string windowTitle, Vector2D windowSize, bool fullscreen) {
    quit_ = false;
    video_manager_ = new VideoManager();
    SDL_Init(SDL_INIT_EVERYTHING);
    video_manager_->Initialize(windowTitle, windowSize, fullscreen);
    input_manager_ = new InputManager();
    time_handler_ = new TimeHandler();
    scene_list_.clear();
    window_size_ = windowSize;
    return (time_handler_ != NULL);
}

void Engine::Run() {
    Key key;
    SDL_Event event;
    float delta_t;

    while(!quit_) {
        // gerenciamento de tempo
        time_handler_->Update();
        delta_t = (time_handler_->TimeDifference())/1000.0f;

        // Verifica se o FPS nao esta baixo demais.
        // Impede que os personagens atravessem paredes.
        delta_t = min(delta_t, 0.1);

        // gerenciador de input
        input_manager()->Update(delta_t);
        
        // tratamento de eventos
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT:
                    quit();
                    break;
                    
                case SDL_KEYDOWN:
                    key = (Key)event.key.keysym.sym;
                    input_manager()->SimulateKeyPress(key);
                    break;
                    
                case SDL_KEYUP:
                    key = (Key)event.key.keysym.sym;
                    input_manager()->SimulateKeyRelease(key);
                    break;

                default:
                    break;
            }
        }

        // gerenciamento das cenas
        if (scene_list_.size() == 0) {
            break;
        }

        CurrentScene()->Update(delta_t);
        for (int i = 0; i < static_cast<int>(scene_list_.size()); i++) {
            scene_list_[i]->Render();
        }

        // gerenciamento de video
        video_manager_->Render();
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
