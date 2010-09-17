//
// Horus Eye
// Copyright (C) 2010  Nucleo de Desenvolvimento de Jogos da USP
//
// game/main.cc
// Arquivo principal do game (inclui a funcao main())
//
#include "../framework/engine.h"
#include "../framework/audiomanager.h"
#include "../framework/vector2D.h"
#include "utils/levelmanager.h"

// funcao principal do programa

utils::LevelManager* level_manager() {
    return utils::LevelManager::reference();
}

framework::Engine* engine() {
    return framework::Engine::reference();
}

int main(int argc, char *argv[]) {
    engine()->Initialize("Horus Eye", framework::Vector2D(1024, 768), false);
    engine()->audio_manager()->LoadMusic("data/musics/bgmusic.wav")->PlayForever();

    level_manager()->Initialize();

    engine()->Run();

    level_manager()->Finish();
    engine()->Release();
    return 0;
}
