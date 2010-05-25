//
// Horus Eye
// Copyright (C) 2010  Nucleo de Desenvolvimento de Jogos da USP
//
// game/main.cc
// Arquivo principal do game (inclui a funcao main())
//
#include "../framework/engine.h"
#include "../framework/vector2D.h"
#include "scenes/world.h"
#include "sprites/worldobject.h"
using framework::Engine;
using framework::Scene;
using framework::Vector2D;
using scene::World;

// funcao principal do programa
// atualmente ela simplesmente inicia
// a engine com um World
int main(int argc, char *argv[]) {
    Engine *engine = Engine::reference();
    engine->Initialize("Horus Eye", Vector2D(1024, 768), false);

    Scene *world = new World;
    engine->PushScene(world);
    engine->Run();

    engine->Release();
    return 0;
}
