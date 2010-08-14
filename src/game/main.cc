//
// Horus Eye
// Copyright (C) 2010  Nucleo de Desenvolvimento de Jogos da USP
//
// game/main.cc
// Arquivo principal do game (inclui a funcao main())
//
#include "../framework/engine.h"
#include "../framework/vector2D.h"
#include "utils/levelmanager.h"

// funcao principal do programa
int main(int argc, char *argv[]) {
	framework::Engine *engine = framework::Engine::reference();
    engine->Initialize("Horus Eye", framework::Vector2D(1280, 720), true);
    engine->audio_manager()->LoadMusic("data/musics/bgmusic.wav")->PlayForever();
	utils::LevelManager::reference()->Initialize();
    engine->Run();
    return 0;
}
