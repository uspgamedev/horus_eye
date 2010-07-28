//
// Horus Eye
// Copyright (C) 2010  Nucleo de Desenvolvimento de Jogos da USP
//
// game/main.cc
// Arquivo principal do game (inclui a funcao main())
//
#include "../framework/engine.h"
#include "sprites/hero.h"
#include "../framework/vector2D.h"
#include "scenes/world.h"
#include "scenes/imagescene.h"
#include "sprites/worldobject.h"
#include "utils/levelloader.h"
#include <iostream>
using framework::Engine;
using framework::Scene;
using framework::Vector2D;
using scene::World;
using scene::ImageScene;
using utils::LevelLoader;


// funcao principal do programa
// atualmente ela simplesmente inicia
// a engine com um World
int main(int argc, char *argv[]) {
    Engine *engine = Engine::reference();
    engine->Initialize("Horus Eye", Vector2D(1024, 768), false);

    World *world = new World;
    engine->PushScene(world);
    LevelLoader *loader = new LevelLoader(world);
    loader->Load("data/levels/level_test.txt");
    engine->audio_manager()->LoadMusic("data/musics/bgmusic.wav")->PlayForever();

    engine->Run();

    //engine->Release();
    return 0;
}
