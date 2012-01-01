#include "floor.h"
#include <ugdk/graphic/videomanager.h>
#include <ugdk/base/engine.h>
#include <ugdk/action/animationset.h>
#include <ugdk/action/sprite.h>

//#include "game/utils/circleobject.h"
#include "game/utils/constants.h"
#include "game/utils/tile.h"
#include "game/scenes/world.h"
#include <float.h>

using namespace ugdk;
using namespace utils;
using namespace scene;

namespace sprite {

#define CLEAR   0
#define DARK    1

Floor::Floor(ugdk::FlexibleSpritesheet* image) : tile_(NULL), sprite_(new ugdk::Sprite(image)) {
    image->set_hotspot(Vector2D(Constants::FLOOR_HOTSPOT_X, Constants::FLOOR_HOTSPOT_Y));

    node_->set_drawable(sprite_);
}

void Floor::Update(float delta_t) {
    if (!tile_) tile_ = Tile::GetFromWorldPosition(WORLD()->level_matrix(), this->world_position());

    if (tile_->visible())   node_->modifier()->set_color(ugdk::Color(1.0f, 1.0f, 1.0f));
    else                    node_->modifier()->set_color(ugdk::Color(0.5f, 0.5f, 0.5f));
    super::Update(delta_t);
}

void Floor::set_world_position(const ugdk::Vector2D& pos) {
    super::set_world_position(pos);
    node_->set_zindex(-FLT_MAX); // chao deve ficar a baixo de tudo/
}

}



