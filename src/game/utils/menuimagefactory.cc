#include "menuimagefactory.h"

#include <ugdk/base/engine.h>
#include <ugdk/base/resourcemanager.h>
#include <ugdk/action/animationplayer.h>
#include <ugdk/graphic/drawable/sprite.h>

using ugdk::graphic::Sprite;
using ugdk::base::ResourceManager;

namespace utils {

MenuImageFactory::MenuImageFactory() {}

ugdk::graphic::Sprite* MenuImageFactory::HorusEye() {
    ugdk::action::SpriteAnimationTable* ANIMATIONS = ResourceManager::GetSpriteAnimationTableFromFile("animations/menu.gdd");
    ugdk::graphic::Spritesheet *menu_eye_sheet = ResourceManager::GetSpritesheetFromTag("eye");
    ugdk::action::SpriteTableAnimationPlayer* player = new ugdk::action::SpriteTableAnimationPlayer(ANIMATIONS);
    player->Select("SELECTION_EYE");
    Sprite* sprite = new Sprite(menu_eye_sheet, player);
    sprite->set_hotspot(ugdk::graphic::Drawable::CENTER);
    return sprite;
}
}

