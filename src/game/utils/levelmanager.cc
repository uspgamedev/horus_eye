/*
 * levelmanager.cc
 *
 *  Created on: Aug 4, 2010
 *      Author: Henrique
 */

#include "../../framework/engine.h"
#include "../../framework/videomanager.h"
#include "../../framework/image.h"
#include "../../framework/scene.h"
#include "levelmanager.h"
#include "../scenes/menu.h"
#include "../scenes/world.h"
#include "../scenes/imagescene.h"
#include "levelloader.h"
#include <cstdio>

using namespace framework;
using namespace std;
using namespace scene;

namespace utils {

LevelManager::LevelManager() {}

void LevelManager::Initialize() {
    FILE* list = fopen("data/level_list.txt", "r");
    if(list != NULL) {
        int level_count = 0;
        fscanf(list,"%d",&level_count);
        for(int i = 0; i < level_count; ++i) {
            char filename[255];
            fscanf(list,"%s",filename);
            level_list_.push_back(filename);
        }
        fclose(list);
    }
    current_level_ = NULL;
    level_list_iterator_ = 0;
    menu_ = new Menu;
    Engine::reference()->PushScene(menu_);
}

void finishAndDeleteCurrentScene() {
    Engine *engine = Engine::reference();
    Scene* current_scene = engine->CurrentScene();
    current_scene->Finish();
    //engine->PopScene();
    //delete current_scene;
}

void LevelManager::ShowIntro() {
    Image *intro = VIDEO_MANAGER()->LoadImage("data/images/intro_text_en.png");
    Engine::reference()->PushScene(new ImageScene(NULL, intro, 30, ImageScene::INTRO));
}

void LevelManager::ShowEnding() {
    Image *intro = VIDEO_MANAGER()->LoadImage("data/images/intro_text_en.png");
    ImageScene *ending = new ImageScene(NULL, intro, 30, ImageScene::ENDING);
    Engine::reference()->PushScene(ending);
}

void LevelManager::ShowGameOver() {
    Image *intro = VIDEO_MANAGER()->LoadImage("data/images/intro_text_en.png");
    ImageScene *ending = new ImageScene(NULL, intro, 30, ImageScene::GAMEOVER);
    Engine::reference()->PushScene(ending);
}

void LevelManager::StartGame(ImageScene::SceneType type) {
    if(current_level_ != NULL)
        return;
    finishAndDeleteCurrentScene();
    if(type == ImageScene::INTRO) {
        level_list_iterator_ = 0;
        LoadNextLevel();
    }
}

void LevelManager::FinishLevel(LevelState state) {
    if(Engine::reference()->CurrentScene() != current_level_) {
        // Oh crap, tem alguma Scene empilhada que não deveria estar la!
        // Vamos simplesmente fazer nada...
        return;
    }
    if(state == FINISH_WIN || state == FINISH_WARP)
        ++level_list_iterator_;

    finishAndDeleteCurrentScene();
    current_level_ = NULL;

    switch(state) {
    case FINISH_DIE:
        ShowGameOver();
    case FINISH_QUIT:
    case NOT_FINISHED:
        return;
    case FINISH_WIN:
    case FINISH_WARP:
        LoadNextLevel();
    }
}

void LevelManager::LoadNextLevel() {
    if(level_list_iterator_ == level_list_.size()) {
        ShowEnding();
        return;
    }
    current_level_ = new World;
    LevelLoader *loader = new LevelLoader(current_level_);
    loader->Load(level_list_.at(level_list_iterator_));
    Engine::reference()->PushScene(current_level_);
    delete loader;
}

LevelManager::~LevelManager() {}

}
