#include "../../framework/engine.h"
#include "../../framework/videomanager.h"
#include "../../framework/textmanager.h"
#include "../../framework/image.h"
#include "../../framework/scene.h"
#include "../scenes/menubuilder.h"
#include "../scenes/menu.h"
#include "../scenes/world.h"
#include "../scenes/imagescene.h"
#include "../scenes/scrollingimagescene.h"
#include "../scenes/loading.h"
#include "../sprites/weapons/herofireballweapon.h"
#include "../sprites/weapons/herobaseweapon.h"
#include "../sprites/weapons/heroexplosionweapon.h"
#include "levelmanager.h"
#include "imagefactory.h"
#include "levelloader.h"
#include <fstream>
#include <iostream>

using namespace framework;
using namespace std;
using namespace scene;
using namespace sprite;

namespace utils {

LevelManager::LevelManager() {}

void LevelManager::Initialize() {

    MenuBuilder builder;
	ifstream list ("data/level_list.txt");
    if(list.is_open()) {
		int level_count;
		list >> level_count;
        for(int i = 0; i < level_count; ++i) {
			string filename;
			list >> filename;
            level_list_.push_back(filename);
        }
		list.close();
    } else {
		cout << "CANNOT OPEN data/level_list.txt" << endl;
		exit(0);
	}
    current_level_ = NULL;
    level_list_iterator_ = 0;
	hero_ = NULL;
    menu_ = builder.BuildMainMenu();
    Engine::reference()->PushScene(menu_);
}

void finishAndDeleteCurrentScene() {
    Engine *engine = Engine::reference();
    Scene* current_scene = engine->CurrentScene();
    current_scene->Finish();
}

void LevelManager::ShowIntro() {
    TEXT_MANAGER()->setFont("data/font/Filmcryptic.ttf", 50, NULL);
    Image *intro = TEXT_MANAGER()->LoadFile("data/text/intro_en.txt", 'c');
    Engine::reference()->PushScene(new Loading);
    Engine::reference()->PushScene(new ScrollingImageScene(NULL, intro, 45));
}

void LevelManager::ShowCredits() {
    TEXT_MANAGER()->setFont("data/font/Filmcryptic.ttf", 50, NULL);
    Image *intro = TEXT_MANAGER()->LoadFile("data/text/credits_en.txt", 'c');
    Engine::reference()->PushScene(new ScrollingImageScene(NULL, intro, 30));
}

void LevelManager::ShowEnding() {
    Engine::reference()->PushScene(new ImageScene(NULL,
            VIDEO_MANAGER()->LoadImage("data/images/you_win.png")));
}

void LevelManager::ShowGameOver() {
    Engine::reference()->PushScene(new ImageScene(NULL,
            VIDEO_MANAGER()->LoadImage("data/images/game_over.png")));
}

void LevelManager::StartGame() {
    if(current_level_ != NULL)
        return;
    level_list_iterator_ = 0;
    LoadNextLevel();
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
		hero_->AddWeapon(1, new HeroExplosionWeapon(hero_));
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