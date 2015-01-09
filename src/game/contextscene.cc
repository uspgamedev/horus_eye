#include "game/context.h"

#include "frontend/scenes/scrollingimagescene.h"
#include "frontend/settings.h"

#include "game/campaigns/campaign.h"

#include <ugdk/action/events.h>
#include <ugdk/text/label.h>
#include <ugdk/text/textbox.h>
#include <ugdk/text/module.h>
#include <ugdk/ui/drawable/texturedrectangle.h>
#include <ugdk/system/engine.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/rendertarget.h>
#include <ugdk/audio/module.h>
#include <ugdk/resource/module.h>
#include <ugdk/text/languageword.h>
#include <ugdk/system/compatibility.h>

#include <cstdio>
#include <vector>

namespace context {

using ugdk::math::Vector2D;
using ugdk::text::TextBox;

void InfoCampaignSceneFinished(const ugdk::action::SceneFinishedEvent& ev) {
    if (auto campaign = campaigns::Campaign::CurrentCampaign())
        campaign->InformSceneFinished();
}

void ShowScrollingText(const std::string& tag) {
    ugdk::text::LanguageWord* langword = ugdk::resource::GetLanguageWord(tag);
    auto textbox = ugdk::MakeUnique<TextBox>(langword->text(), ugdk::graphic::manager()->screen()->size().x, ugdk::text::manager()->GetFont(langword->font()));
    textbox->set_ident_style(TextBox::CENTER);
    auto scroll = ugdk::MakeUnique<frontend::scenes::ScrollingImageScene>(nullptr, std::move(textbox), 55);
    scroll->event_handler().AddListener(InfoCampaignSceneFinished);
    if (frontend::Settings::reference()->background_music())
        scroll->set_background_music(ugdk::audio::manager()->LoadMusic("musics/action_game_theme.ogg"));
    ugdk::system::PushScene(std::move(scroll));
}

void ShowImageAsScene(const std::string& tag, bool is_filename) {
    ugdk::graphic::GLTexture* texture;
    if (is_filename)
        texture = ugdk::resource::GetTextureFromFile(tag);
    else
        texture = ugdk::resource::GetTextureFromTag(tag);

    auto scroll = ugdk::MakeUnique<frontend::scenes::ImageScene>(nullptr, ugdk::MakeUnique<ugdk::ui::TexturedRectangle>(texture));
    scroll->event_handler().AddListener(InfoCampaignSceneFinished);
    ugdk::system::PushScene(std::move(scroll));
}

void ShowTextAsScene(const std::string& message) {
    auto scroll = ugdk::MakeUnique<frontend::scenes::ImageScene>(nullptr, ugdk::MakeUnique<ugdk::text::Label>(message, ugdk::text::manager()->GetFont("DejaVuMono")));
    ugdk::system::PushScene(std::move(scroll));
}


} // namespace context
