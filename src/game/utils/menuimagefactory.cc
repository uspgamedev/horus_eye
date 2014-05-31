#include "menuimagefactory.h"

#include <ugdk/graphic/module.h>
#include <ugdk/graphic/primitive.h>
#include <ugdk/graphic/primitivesetup.h>
#include <ugdk/graphic/sprite.h>
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

    auto primitive = std::make_shared<ugdk::graphic::Primitive>(nullptr, nullptr);
    primitive->set_shader_program(ugdk::graphic::manager()->shaders().current_shader());
    ugdk::graphic::PrimitiveSetup::Sprite::Prepare(*primitive, ugdk::resource::GetTextureAtlasFromFile("resources/images/eye-sheet"));

    auto player = ugdk::graphic::PrimitiveSetup::Sprite::CreateSpriteAnimationPlayer(
        *primitive, ugdk::resource::GetSpriteAnimationTableFromFile("resources/animations/menu.json"));
    player->Select("SELECTION_EYE");

    auto d = new pyramidworks::ui::Drawable(primitive);
    d->set_hotspot(ugdk::graphic::Drawable::CENTER);

    return std::make_pair(d, player);
}

}

