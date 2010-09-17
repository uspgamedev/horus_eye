//
// Horus Eye
// Copyright (C) 2010  Nucleo de Desenvolvimento de Jogos da USP
//
// game/editor.cc
// Arquivo principal do editor (inclui a funcao main())
//
#include "../framework/engine.h"
#include "sprites/hero.h"
#include "../framework/vector2D.h"
#include "scenes/world.h"
#include "sprites/worldobject.h"
#include "utils/levelloader.h"
#include "utils/editor/worldeditor.h"
#include "utils/editor/editorloader.h"
#include <iostream>
#include <cstring>
using framework::Engine;
using framework::Scene;
using framework::Vector2D;
using scene::World;
using utils::WorldEditor;
using utils::LevelLoader;
using utils::EditorLoader;
using std::string;

// funcao principal do programa
// atualmente ela simplesmente inicia
// a engine com um World
int main(int argc, char *argv[]) {
    bool editorMode = false;
    string level = "data/levels/level_test.txt";

    for(int i = 1; i < argc; ++i)
        if(strcmp(argv[i], "--editor") == 0)
            editorMode = true;
        else if(argv[i][0] != '-')
            level = argv[i];

    Engine *engine = Engine::reference();
    engine->Initialize("Horus Eye", Vector2D(1024, 768), false);

    World *world;
    WorldEditor *worldEditor;

    LevelLoader *loader;
    EditorLoader *editor;
    if(editorMode) {
        worldEditor = new WorldEditor;
        editor = new EditorLoader(worldEditor);
    } else {
        world = new World;
        loader = new LevelLoader(world);
    }

    engine->PushScene(editorMode ? (Scene*) worldEditor : (Scene*) world);

    if(editorMode)
        editor->Load(level);
    else
        loader->Load(level);

    if(!editorMode)
        engine->audio_manager()->LoadMusic("data/musics/bgmusic.wav")->PlayForever();
    engine->Run();

    engine->Release();
    return 0;
}
