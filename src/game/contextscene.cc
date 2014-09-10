#include "game/context.h"

#include "game/scenes/scrollingimagescene.h"
#include "game/utils/settings.h"

#include <ugdk/text/label.h>
#include <ugdk/text/textbox.h>
#include <ugdk/text/module.h>
#include <ugdk/ui/drawable/texturedrectangle.h>
#include <ugdk/system/engine.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/rendertarget.h>
#include <ugdk/audio/module.h>
#include <ugdk/resource/module.h>
#include <ugdk/util/languageword.h>
#include <ugdk/system/compatibility.h>

#include <cstdio>
#include <vector>

namespace context {

using ugdk::math::Vector2D;
using ugdk::text::TextBox;

void ShowScrollingText(const std::string& tag) {
    ugdk::LanguageWord* langword = ugdk::resource::GetLanguageWord(tag);
    auto textbox = ugdk::MakeUnique<TextBox>(langword->text(), ugdk::graphic::manager()->screen()->size().x, ugdk::text::manager()->GetFont(langword->font()));
    textbox->set_ident_style(TextBox::CENTER);
    ugdk::action::Scene *scroll = new scene::ScrollingImageScene(nullptr, std::move(textbox), 55);
    if (utils::Settings::reference()->background_music())
        scroll->set_background_music(ugdk::audio::manager()->LoadMusic("musics/action_game_theme.ogg"));
    ugdk::system::PushScene(scroll);
}

void ShowImageAsScene(const std::string& tag, bool is_filename) {
    ugdk::internal::GLTexture* texture;
    if (is_filename)
        texture = ugdk::resource::GetTextureFromFile(tag);
    else
        texture = ugdk::resource::GetTextureFromTag(tag);

    ugdk::action::Scene *scroll = new scene::ImageScene(nullptr, ugdk::MakeUnique<ugdk::ui::TexturedRectangle>(texture));
    ugdk::system::PushScene(scroll);
}

void ShowTextAsScene(const std::string& message) {
    ugdk::action::Scene *scroll = new scene::ImageScene(nullptr, ugdk::MakeUnique<ugdk::text::Label>(message, ugdk::text::manager()->GetFont("DejaVuMono")));
    ugdk::system::PushScene(scroll);
}


} // namespace context
