#include "../../framework/engine.h"
#include "../../framework/videomanager.h"
#include "../../framework/image.h"
#include "../../framework/scene.h"
#include "levelmanager.h"
#include "../scenes/menu.h"
#include "../scenes/world.h"
#include "../scenes/imagescene.h"
#include "imagefactory.h"
#include "levelloader.h"
#include "../sprites/weapons/herofireballweapon.h"
#include "../sprites/weapons/herobaseweapon.h"
#include <cstdio>

using namespace framework;
using namespace std;
using namespace scene;
using namespace sprite;

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
	hero_ = NULL;
    menu_ = new Menu;
    Engine::reference()->PushScene(menu_);
}

void finishAndDeleteCurrentScene() {
    Engine *engine = Engine::reference();
    Scene* current_scene = engine->CurrentScene();
    current_scene->Finish();
    engine->PopScene();
    delete current_scene;
}

void LevelManager::ShowIntro() {
    Engine::reference()->text_manager()->setFont("data/font/Filmcryptic.ttf", 28, NULL);
    TEXT_MANAGER()->setFont("data/font/Filmcryptic.ttf", 50, NULL);
//    Image *intro = TEXT_MANAGER()->LoadFile("data/text/intro_en.txt", 'c');
//    Image *intro = TEXT_MANAGER()->LoadText("Texto\nTexto Grande\nTexto\nTxt\nTexto\n", 'c');
//    Image *intro = TEXT_MANAGER()->LoadLine("Texto");
    Image *intro = VIDEO_MANAGER()->LoadImage("data/images/intro_text_en.png");
    Engine::reference()->PushScene(new ImageScene(NULL, intro, 30, ImageScene::INTRO));
}

void LevelManager::ShowEnding() {
    ImageFactory image_factory;
    Image *img = image_factory.WinImage();
    ImageScene *ending = new ImageScene(NULL, img, 30, ImageScene::ENDING);
    Engine::reference()->PushScene(ending);
}

void LevelManager::ShowGameOver() {
    ImageFactory image_factory;
    Image *img = image_factory.LoseImage();
    ImageScene *ending = new ImageScene(NULL, img, 30, ImageScene::GAMEOVER);
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
        // Oh crap, there's some stacked Scene that shouldn't be there!
        // Lets just do nothing...
        return;
    }
    if(state == FINISH_WIN)
        ++level_list_iterator_;

    finishAndDeleteCurrentScene();
    current_level_ = NULL;

    switch(state) {
    case FINISH_DIE:
		hero_ = NULL;
        ShowGameOver();
    case FINISH_QUIT:
    case NOT_FINISHED:
		if (hero_)
			delete hero_;
			hero_ = NULL;
        return;
    case FINISH_WIN:
    case FINISH_WARP:
        LoadNextLevel();
    }
}

void LevelManager::LoadNextLevel() {
    if(level_list_iterator_ == level_list_.size()) {
        ShowEnding();
        if (hero_) delete hero_;
        hero_ = NULL;
        return;
    }

	if (hero_ == NULL) {
		hero_ = new sprite::Hero;
	}
	if (level_list_iterator_ == 0) {
		hero_->set_life(hero_->max_life());
		hero_->set_mana(hero_->max_mana());
		hero_->AddWeapon(0, new HeroFireballWeapon(hero_));
		// Add here the other initial weapons of the hero.
	}
    current_level_ = new World(hero_);
    LevelLoader *loader = new LevelLoader(current_level_);
    loader->Load(level_list_.at(level_list_iterator_));
    Engine::reference()->PushScene(current_level_);
    delete loader;
}

void LevelManager::Finish() {
	if (hero_)
		delete hero_;
}

LevelManager::~LevelManager() {}

}
