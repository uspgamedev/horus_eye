#include "game/utils/levelmanager.h"

#include <fstream>
#include <iostream>

#include <ugdk/system/engine.h>
#include <ugdk/resource/module.h>
#include <ugdk/audio/module.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/textmanager.h>
#include <ugdk/graphic/drawable/label.h>
#include <ugdk/graphic/drawable/textbox.h>
#include <ugdk/graphic/drawable/texturedrectangle.h>
#include <ugdk/input/module.h>
#include <ugdk/action/scene.h>

#include "game/scenes/world.h"
#include "game/scenes/imagescene.h"
#include "game/scenes/scrollingimagescene.h"
#include "game/scenes/loading.h"
#include "game/scenes/imagescene.h"
#include "game/components/caster.h"
#include "game/builders/goodmenubuilder.h"
#include "game/builders/herobuilder.h"
#include "game/sprites/worldobject.h"
#include "game/utils/imagefactory.h"
#include "game/utils/levelloader.h"
#include "game/utils/settings.h"
#include "game/map/tile.h"

#ifdef WIN32
#include <windows.h>
#endif

using namespace ugdk;
using namespace ugdk::action;
using namespace scene;
using namespace sprite;

using std::string;
using std::ifstream;
using ugdk::graphic::Drawable;
using ugdk::graphic::TexturedRectangle;
using ugdk::graphic::TextBox;

namespace externals {
// Convert string to wstring
std::wstring str_to_wstr(const std::string& arg);
}

namespace utils {

LevelManager::LevelManager() {}

void LevelManager::Initialize() {
    restart_game_ = false;
    current_level_ = nullptr;
    level_list_iterator_ = 0;

    menu_ = builder::MainMenu();
    ugdk::system::PushScene(menu_);

    loading_ = nullptr;
}

void LevelManager::ShowIntro() {

    if(loading_) {
        loading_->Finish();
        loading_ = nullptr;
    }

    ugdk::system::PushScene(loading_ = new Loading);
    level_list_iterator_ = 0;
    ugdk::LanguageWord* langword = ugdk::resource::GetLanguageWord("Intro");
    TextBox* textbox = new TextBox(langword->text(), ugdk::graphic::manager()->video_size().x, TEXT_MANAGER()->GetFont(langword->font()));
    textbox->set_ident_style(TextBox::CENTER);
    Scene *scroll = new ScrollingImageScene(NULL, textbox, 45);
    if(Settings::reference()->background_music())
        scroll->set_background_music(ugdk::audio::manager()->LoadMusic("musics/action_game_theme.ogg"));
    ugdk::system::PushScene(scroll);
}

void LevelManager::ShowCredits() {
    ugdk::LanguageWord* langword = ugdk::resource::GetLanguageWord("CreditsFile");
    TextBox* textbox = new TextBox(langword->text(), ugdk::graphic::manager()->video_size().x, TEXT_MANAGER()->GetFont(langword->font()));
    textbox->set_ident_style(TextBox::CENTER);
    Scene *scroll = new ScrollingImageScene(NULL, textbox, 55);
    if(Settings::reference()->background_music())
        scroll->set_background_music(ugdk::audio::manager()->LoadMusic("musics/action_game_theme.ogg"));
    ugdk::system::PushScene(scroll);
}

void LevelManager::ShowEnding() {
    loading_->Finish();
    loading_ = nullptr;
    Drawable* message = new TexturedRectangle(ugdk::resource::GetTextureFromFile("images/you_win.png"));
    ugdk::system::PushScene(new ImageScene(NULL, message));
}

void LevelManager::ShowGameOver() {
    Drawable* message = new TexturedRectangle(ugdk::resource::GetTextureFromFile("images/game_over.png"));
    ugdk::system::PushScene(new ImageScene(NULL, message));
}

void LevelManager::FinishLevel(LevelState state) {
    //if(ugdk::system::CurrentScene() != current_level_) {
        // Oh crap, there's some stacked Scene that shouldn't be there!
        // Lets just do nothing...
        //return;
    //}
    if(state == FINISH_WIN)
        ++level_list_iterator_;

    current_level_->Finish();
    current_level_ = nullptr;

    switch(state) {
    case FINISH_DIE:
        ShowGameOver();
        // no break on purpose
    case FINISH_QUIT:
        if(loading_) loading_->Finish();
        loading_ = nullptr;
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
    utils::LoadLevel(current_campaign_, level_name, &current_level_);
    if(!current_level_) {
        ugdk::system::PushScene(new ImageScene(NULL, new ugdk::graphic::Label(
            L"Error loading level '" + externals::str_to_wstr(level_name) 
            + L"' from campaign '" + externals::str_to_wstr(current_campaign_) + L"'.",
            TEXT_MANAGER()->current_font()
        )));
        if(ugdk::input::manager()->KeyDown(ugdk::input::K_ESCAPE))
            loading_->Finish();
        return;
    }
    current_level_->AddTask([](double) {
        if(ugdk::input::manager()->KeyPressed(ugdk::input::K_ESCAPE))
            ugdk::system::PushScene(builder::PauseMenu());
    });
    current_level_->SetHero(builder::HeroBuilder::Kha());
    ugdk::system::PushScene(current_level_);
    current_level_->Start();
}

}
