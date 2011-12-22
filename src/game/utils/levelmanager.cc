#include <fstream>
#include <iostream>

#include <ugdk/base/engine.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/graphic/textmanager.h>
#include <ugdk/audio/audiomanager.h>
#include <ugdk/util/pathmanager.h>
#include <ugdk/graphic/image.h>
#include <ugdk/action/scene.h>

#include "game/utils/levelmanager.h"

#include "game/scenes/menubuilder.h"
#include "game/scenes/menu.h"
#include "game/scenes/world.h"
#include "game/scenes/imagescene.h"
#include "game/scenes/scrollingimagescene.h"
#include "game/scenes/loading.h"
#include "game/sprites/creatures/creature.h"
#include "game/sprites/creatures/hero.h"
#include "game/builders/herobuilder.h"
#include "game/sprites/explosion.h"
#include "game/scenes/imagescene.h"
#include "game/utils/imagefactory.h"
#include "game/utils/levelloader.h"
#include "game/utils/textloader.h"

#ifdef WIN32
#include <windows.h>
#endif

using namespace ugdk;
using namespace std;
using namespace scene;
using namespace sprite;

namespace utils {

LevelManager::LevelManager() {}

void LevelManager::Initialize() {
    restart_game_ = false;
    LoadLevelList("data/level_list.txt", level_list_);
    current_level_ = NULL;
    level_list_iterator_ = 0;
	hero_ = NULL;
	Creature::InitializeAnimations();
    Explosion::InitializeAnimations();
    MenuBuilder::InitializeAnimations();

	MenuBuilder builder;
    menu_ = builder.BuildMainMenu();
    Engine::reference()->PushScene(menu_);

	loading_ = NULL;
}

void LevelManager::LoadLevelList(std::string relative_file, std::vector<std::string>& level_list) {
    string file = PATH_MANAGER()->ResolvePath(relative_file);
    level_list.clear();
    ifstream list (file.c_str());
    if(list.is_open()) {
        int level_count;
        list >> level_count;
        for(int i = 0; i < level_count; ++i) {
            string filename;
            list >> filename;
            level_list.push_back(filename);
        }
        list.close();
    } else {
#ifdef WIN32
        MessageBox(HWND_DESKTOP,"Could not open the level list file.","Fatal Error: File not Found", MB_OK);
#else
        fprintf(stderr, "Fatal Error: Cannot open the level list file in %s.\n", file.c_str());
#endif
        exit(1);
    }
}

void finishAndDeleteCurrentScene() {
    Engine *engine = Engine::reference();
    Scene* current_scene = engine->CurrentScene();
    current_scene->Finish();
}

void LevelManager::ShowIntro() {
    Engine::reference()->PushScene(loading_ = new Loading);
	level_list_iterator_ = 0;
    Scene *scroll = new ScrollingImageScene(NULL, static_cast<Image*>(TEXT_LOADER()->GetImage("Intro")), 45);
    scroll->set_background_music(AUDIO_MANAGER()->LoadMusic("data/musics/action_game_theme.ogg"));
    Engine::reference()->PushScene(scroll);
}

void LevelManager::ShowCredits() {
    Scene *scroll = new ScrollingImageScene(NULL, static_cast<Image*>(TEXT_LOADER()->GetImage("CreditsFile")), 55);
    scroll->set_background_music(AUDIO_MANAGER()->LoadMusic("data/musics/action_game_theme.ogg"));
    Engine::reference()->PushScene(scroll);
}

void LevelManager::ShowEnding() {
	loading_->Finish();
	loading_ = NULL;
    Engine::reference()->PushScene(new ImageScene(NULL,
            VIDEO_MANAGER()->LoadImageFile("data/images/you_win.png")));
}

void LevelManager::ShowGameOver() {
    Engine::reference()->PushScene(new ImageScene(NULL,
            VIDEO_MANAGER()->LoadImageFile("data/images/game_over.png")));
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
        // no break on purpose
    case FINISH_QUIT:
		loading_->Finish();
		loading_ = NULL;
        // no break on purpose
    case NOT_FINISHED:
		if (hero_)
			delete hero_;
			hero_ = NULL;
        return;
    case FINISH_WIN:
    case FINISH_WARP:
        //LoadNextLevel(); -- The stacked Loading takes care of doing this.
		break;
    }
}

void LevelManager::LoadNextLevel() {
    if(level_list_iterator_ == level_list_.size()) {
        ShowEnding();
        if (hero_) delete hero_;
        hero_ = NULL;
        return;
    }
    utils::ImageFactory *factory = new utils::ImageFactory();
	if (level_list_iterator_ == 0) {
	    if (hero_ == NULL) {
            builder::HeroBuilder builder(factory);
            hero_ = builder.Kha();
	    }
	}
	hero_->mana_blocks().Fill();
    current_level_ = new World(hero_, factory);
    LevelLoader *loader = new LevelLoader(current_level_);
    loader->Load(level_list_.at(level_list_iterator_));
	delete loader;
    Engine::reference()->PushScene(current_level_);
}

void LevelManager::Finish() {
	if (hero_ != NULL) {
		delete hero_;
        hero_ = NULL;
    }
	if (loading_)
		delete loading_;
	Creature::ReleaseAnimations();
    Explosion::ReleaseAnimations();
    MenuBuilder::ReleaseAnimations();
}

LevelManager::~LevelManager() {}

}
