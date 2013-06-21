#include "game/utils/levelmanager.h"

#include <fstream>
#include <iostream>

#include <ugdk/base/engine.h>
#include <ugdk/base/resourcemanager.h>
#include <ugdk/audio/audiomanager.h>
#include <ugdk/util/pathmanager.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/graphic/textmanager.h>
#include <ugdk/graphic/drawable/textbox.h>
#include <ugdk/graphic/drawable/texturedrectangle.h>
#include <ugdk/input/inputmanager.h>
#include <ugdk/action/scene.h>

#include "game/scenes/menu.h"
#include "game/scenes/world.h"
#include "game/scenes/imagescene.h"
#include "game/scenes/scrollingimagescene.h"
#include "game/scenes/loading.h"
#include "game/components/caster.h"
#include "game/builders/goodmenubuilder.h"
#include "game/sprites/worldobject.h"
#include "game/scenes/imagescene.h"
#include "game/utils/imagefactory.h"
#include "game/utils/levelloader.h"
#include "game/map/tile.h"
#include "game/utils/settings.h"

#ifdef WIN32
#include <windows.h>
#endif

using namespace ugdk;
using namespace ugdk::action;
using namespace scene;
using namespace sprite;

using std::string;
using std::ifstream;
using ugdk::Engine;
using ugdk::base::ResourceManager;
using ugdk::graphic::Drawable;
using ugdk::graphic::TexturedRectangle;
using ugdk::graphic::TextBox;

namespace utils {

LevelManager::LevelManager() {}

void LevelManager::Initialize() {
    restart_game_ = false;
    LoadLevelList("level_list.txt", level_list_);
    current_level_ = NULL;
    level_list_iterator_ = 0;
    builder::MenuBuilder builder;
    menu_ = builder.MainMenu();
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

void LevelManager::ShowIntro() {
    Engine::reference()->PushScene(loading_ = new Loading);
    level_list_iterator_ = 0;
    ugdk::LanguageWord* langword = ResourceManager::GetLanguageWord("Intro");
    TextBox* textbox = new TextBox(langword->text(), VIDEO_MANAGER()->video_size().x, TEXT_MANAGER()->GetFont(langword->font()));
    textbox->set_ident_style(TextBox::CENTER);
    Scene *scroll = new ScrollingImageScene(NULL, textbox, 45);
    if(Settings::reference()->background_music())
        scroll->set_background_music(AUDIO_MANAGER()->LoadMusic("musics/action_game_theme.ogg"));
    Engine::reference()->PushScene(scroll);
}

void LevelManager::ShowCredits() {
    ugdk::LanguageWord* langword = ResourceManager::GetLanguageWord("CreditsFile");
    TextBox* textbox = new TextBox(langword->text(), VIDEO_MANAGER()->video_size().x, TEXT_MANAGER()->GetFont(langword->font()));
    textbox->set_ident_style(TextBox::CENTER);
    Scene *scroll = new ScrollingImageScene(NULL, textbox, 55);
    if(Settings::reference()->background_music())
        scroll->set_background_music(AUDIO_MANAGER()->LoadMusic("musics/action_game_theme.ogg"));
    Engine::reference()->PushScene(scroll);
}

void LevelManager::ShowEnding() {
    loading_->Finish();
    loading_ = NULL;
    Drawable* message = new TexturedRectangle(ResourceManager::GetTextureFromFile("images/you_win.png"));
    Engine::reference()->PushScene(new ImageScene(NULL, message));
}

void LevelManager::ShowGameOver() {
    Drawable* message = new TexturedRectangle(ResourceManager::GetTextureFromFile("images/game_over.png"));
    Engine::reference()->PushScene(new ImageScene(NULL, message));
}

void LevelManager::FinishLevel(LevelState state) {
    if(Engine::reference()->CurrentScene() != current_level_) {
        // Oh crap, there's some stacked Scene that shouldn't be there!
        // Lets just do nothing...
        return;
    }
    if(state == FINISH_WIN)
        ++level_list_iterator_;

    current_level_->Finish();
    current_level_ = NULL;

    switch(state) {
    case FINISH_DIE:
        ShowGameOver();
        // no break on purpose
    case FINISH_QUIT:
        if(loading_) loading_->Finish();
        loading_ = NULL;
        // no break on purpose
    case NOT_FINISHED:
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
        return;
    }
    loadSpecificLevel(level_list_.at(level_list_iterator_));
}

void LevelManager::Finish() {
    if (loading_)
        delete loading_;
}

LevelManager::~LevelManager() {}

void LevelManager::loadSpecificLevel(const std::string& level_name) {
    current_level_ = new World();
    {
        LevelLoader loader(current_level_);
        loader.Load(level_name);
    }
    {
        current_level_->AddTask([](double) -> bool {
            ugdk::input::InputManager *input = ugdk::Engine::reference()->input_manager();
            if(input->KeyPressed(ugdk::input::K_ESCAPE)) {
                builder::MenuBuilder builder;
                ugdk::Engine::reference()->PushScene(builder.PauseMenu());
            }
            return true;
        });
    }

    Engine::reference()->PushScene(current_level_);
    current_level_->Start();
}

}
