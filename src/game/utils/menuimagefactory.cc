#include "menuimagefactory.h"

#include <ugdk/graphic/module.h>
#include <ugdk/resource/module.h>
#include <ugdk/graphic/sprite.h>
#include <pyramidworks/ui/drawable.h>

using ugdk::graphic::Sprite;

namespace utils {

MenuImageFactory::MenuImageFactory() {}

std::pair<
    ugdk::graphic::Drawable*,
    std::shared_ptr<ugdk::action::SpriteAnimationPlayer>
> MenuImageFactory::HorusEye() {
    auto r = 
        ugdk::graphic::CreateSpritePrimitive(ugdk::resource::GetSpritesheetFromTag("eye"),
                                             ugdk::resource::GetSpriteAnimationTableFromFile("animations/menu.gdd"));

    std::get<0>(r)->set_shader_program(ugdk::graphic::manager()->shaders().current_shader());
    auto d = new pyramidworks::ui::Drawable(std::get<0>(r));
    std::get<1>(r)->Select("SELECTION_EYE");

    d->set_hotspot(ugdk::graphic::Drawable::CENTER);

    return std::make_pair(d, std::get<1>(r));
}

}

