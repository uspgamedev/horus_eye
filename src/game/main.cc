//
// Horus Eye

// Copyright (C) 2010  Nucleo de Desenvolvimento de Jogos da USP
//
// game/main.cc
// Arquivo principal do game (inclui a funcao main())
//
#include <string>
#include "../framework/engine.h"
#include "../framework/audiomanager.h"
#include "../framework/vector2D.h"
#include "utils/levelmanager.h"
#include "utils/settings.h"
#include "utils/textloader.h"

// funcao principal do programa

using namespace utils;

utils::LevelManager* level_manager() {
    return utils::LevelManager::reference();
}

utils::TextLoader* text_loader() {
    return utils::TextLoader::reference();
}

framework::Engine* engine() {
    return framework::Engine::reference();
}

int main(int argc, char *argv[]) {
    Settings settings = Settings(std::string("settings.duh"));
    engine()->Initialize("Horus Eye", settings.resolution(), settings.fullscreen());
    //engine()->Initialize("Horus Eye", framework::Vector2D(800, 600), false);
    engine()->audio_manager()->LoadMusic("data/musics/bgmusic.wav")->PlayForever();

    text_loader()->Initialize("data/text/english.txt");

    level_manager()->Initialize();

    engine()->Run();

    level_manager()->Finish();
    engine()->Release();
    return 0;
}
