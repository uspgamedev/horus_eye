#include "game/utils/levelmanager.h"

#include <fstream>
#include <iostream>

#include <ugdk/system/engine.h>
#include <ugdk/resource/module.h>
#include <ugdk/audio/module.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/text/textmanager.h>
#include <ugdk/graphic/text/label.h>
#include <ugdk/graphic/text/textbox.h>
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
#include "game/initializer.h"

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

namespace utils {

namespace {
LevelManager* reference_ = nullptr;
}

LevelManager* LevelManager::reference() {
    if(!reference_)
        reference_ = new LevelManager;
    return reference_;
}

LevelManager::LevelManager()
: current_level_(nullptr)
{}

LevelManager::~LevelManager() {
    reference_ = nullptr;
}

void LevelManager::InitializeCampaign(const std::string& name, const std::vector<std::string>& list) {
    current_level_ = nullptr;
    loading_ = nullptr;
    level_list_iterator_ = 0;

    current_campaign_ = name;
    level_list_ = list;
    ShowIntro();
}

void LevelManager::ShowIntro() {
    ugdk::system::PushScene(loading_ = new Loading);

    level_list_iterator_ = 0;
    ugdk::LanguageWord* langword = ugdk::resource::GetLanguageWord("Intro");
    TextBox* textbox = new TextBox(langword->text(), ugdk::graphic::manager()->canvas()->size().x, TEXT_MANAGER()->GetFont(langword->font()));
    textbox->set_ident_style(TextBox::CENTER);
    Scene *scroll = new ScrollingImageScene(NULL, textbox, 45);
    if(Settings::reference()->background_music())
        scroll->set_background_music(ugdk::audio::manager()->LoadMusic("musics/action_game_theme.ogg"));
    ugdk::system::PushScene(scroll);
}

void LevelManager::ShowCredits() {
    ugdk::LanguageWord* langword = ugdk::resource::GetLanguageWord("CreditsFile");
    TextBox* textbox = new TextBox(langword->text(), ugdk::graphic::manager()->canvas()->size().x, TEXT_MANAGER()->GetFont(langword->font()));
    textbox->set_ident_style(TextBox::CENTER);
    Scene *scroll = new ScrollingImageScene(NULL, textbox, 55);
    if(Settings::reference()->background_music())
        scroll->set_background_music(ugdk::audio::manager()->LoadMusic("musics/action_game_theme.ogg"));
    ugdk::system::PushScene(scroll);
}

void LevelManager::ShowEnding() {
    Finish();
    Drawable* message = new TexturedRectangle(ugdk::resource::GetTextureFromFile("images/you_win.png"));
    ugdk::system::PushScene(new ImageScene(NULL, message));
}

void LevelManager::ShowGameOver() {
    Finish();
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
        Finish();
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
    if (loading_) {
        loading_->Finish();
    }
    loading_ = nullptr;
}

void LevelManager::loadSpecificLevel(const std::string& level_name) {
    utils::LoadLevel(current_campaign_, level_name, &current_level_);
    if(!current_level_) {
        ugdk::system::PushScene(new ImageScene(NULL, new ugdk::graphic::Label(
            "Error loading level '" + (level_name) + "' from campaign '" + (current_campaign_) + "'.",
            TEXT_MANAGER()->current_font()
        )));
        if(ugdk::input::manager()->keyboard().IsDown(ugdk::input::Keycode::ESCAPE))
            loading_->Finish();
        return;
    }
    current_level_->AddTask([](double) {
        if(ugdk::input::manager()->keyboard().IsPressed(ugdk::input::Keycode::ESCAPE))
            ugdk::system::PushScene(builder::PauseMenu());
    });
    current_level_->SetHero(builder::HeroBuilder::Kha());
    ugdk::system::PushScene(current_level_);
    current_level_->Start();
}

}
