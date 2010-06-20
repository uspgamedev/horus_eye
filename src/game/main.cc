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
#include "sprites/worldobject.h"
#include "utils/levelloader.h"
using framework::Engine;
using framework::Scene;
using framework::Vector2D;
using scene::World;
using utils::LevelLoader;

// funcao principal do programa
// atualmente ela simplesmente inicia
// a engine com um World
int main(int argc, char *argv[]) {
    Engine *engine = Engine::reference();
    engine->Initialize("Horus Eye", Vector2D(1024, 768), false);

    World *world = new World;
    //sprite::Hero *hero = new sprite::Hero; TODO essa linha ta' dando segfault.
    LevelLoader *loader = new LevelLoader(world);
    loader->Load("level_test");
    //world->AddWorldObject(hero);
    //world->set_hero(hero);
    engine->PushScene(world);
    engine->Run();

    engine->Release();
    return 0;
}
