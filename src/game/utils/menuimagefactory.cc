#include "menuimagefactory.h"

#include <ugdk/base/engine.h>
#include <ugdk/base/resourcemanager.h>
#include <ugdk/graphic/drawable/sprite.h>

using namespace ugdk;
using ugdk::graphic::Sprite;

namespace utils {

MenuImageFactory::MenuImageFactory() {}

graphic::Sprite* MenuImageFactory::HorusEye() {
    ugdk::action::AnimationSet* ANIMATIONS = ugdk::base::ResourceManager::GetAnimationSetFromFile("animations/menu.gdd");
    ugdk::graphic::Spritesheet *menu_eye_sheet = RESOURCE_MANAGER()->spritesheet_container().Find("images/eye.png");
    Sprite* sprite = new Sprite(menu_eye_sheet, ANIMATIONS);
    sprite->set_hotspot(ugdk::graphic::Drawable::CENTER);
    sprite->SelectAnimation("SELECTION_EYE");
    return sprite;
}
}

