#include "commonmenu.h"

#include "game/utils/menuimagefactory.h"

#include <ugdk/action/animationplayer.h>
#include <ugdk/action/scene.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/rendertarget.h>
#include <ugdk/graphic/spriteanimationframe.h>
#include <ugdk/input/keycode.h>
#include <ugdk/structure/box.h>
#include <ugdk/system/compatibility.h>
#include <ugdk/ui/button.h>
#include <ugdk/ui/drawable.h>
#include <ugdk/ui/menu.h>

namespace frontend {
namespace nativebuilders {

using ugdk::ui::Menu;
using ugdk::ui::Button;
using ugdk::math::Vector2D;

void MenuFocus(const ugdk::action::SceneFocusEvent& ev) {
    ev.scene->set_active(true);
    ev.scene->set_visible(true);
}

void MenuDeFocus(const ugdk::action::SceneDefocusEvent& ev) {
    ev.scene->set_active(false);
    ev.scene->set_visible(false);
}

void FinishOwner(const ugdk::ui::Button* button) {
    button->owner()->Finish();
}

AnimationPlayerHolder::AnimationPlayerHolder() : Task([this](double dt) {
    for (auto& p : players_)
        p->Update(dt);
}) {}
AnimationPlayerHolder::~AnimationPlayerHolder() {}

void AnimationPlayerHolder::Add(const std::shared_ptr<ugdk::graphic::SpriteAnimationPlayer>& player) {
    players_.push_back(player);
}

std::unique_ptr<Menu> BaseBuildMenu(ugdk::ui::HookPoint hook) {
    ugdk::math::Vector2D origin(0.0, 0.0), target = ugdk::graphic::manager()->screen()->size();
    utils::MenuImageFactory mif;

    auto holder = new AnimationPlayerHolder;
    auto menu = ugdk::MakeUnique<Menu>(ugdk::structure::Box<2>(origin, target), Vector2D(0.0, 0.0), hook);
    menu->event_handler().AddListener(MenuFocus);
    menu->event_handler().AddListener(MenuDeFocus);

    for (int i = 0; i < 2; ++i) {
        auto sprite = mif.HorusEye();
        menu->SetOptionDrawable(std::unique_ptr<ugdk::ui::Drawable>(sprite.first), i);
        holder->Add(sprite.second);
    }
    menu->AddTask(*holder);

    menu->AddCallback(ugdk::input::Keycode::ESCAPE, std::mem_fn(&ugdk::ui::Menu::Finish));
    menu->AddCallback(ugdk::input::Keycode::RETURN, std::mem_fn(&ugdk::ui::Menu::InteractWithFocused));

    menu->StopsPreviousMusic(false);
    return std::move(menu);
}

} // namespace nativebuilders
} // namespace frontend
