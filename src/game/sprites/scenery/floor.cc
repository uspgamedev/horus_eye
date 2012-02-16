#include "floor.h"
#include <ugdk/graphic/drawable/sprite.h>
#include <ugdk/graphic/spritesheet/flexiblespritesheet.h>
#include <ugdk/graphic/node.h>

#include "game/utils/constants.h"
#include "game/utils/tile.h"
#include "game/scenes/world.h"
#include <cfloat>

using namespace ugdk;
using namespace utils;
using namespace scene;

namespace sprite {

#define CLEAR   0
#define DARK    1

Floor::Floor(ugdk::graphic::FlexibleSpritesheet* image) : tile_(NULL), sprite_(new ugdk::graphic::Sprite(image)) {
    image->set_hotspot(Vector2D(Constants::FLOOR_HOTSPOT_X, Constants::FLOOR_HOTSPOT_Y));

    node_->set_drawable(sprite_);
    node_->modifier()->ToggleFlag(ugdk::graphic::Modifier::TRUNCATES_WHEN_APPLIED);
}

void Floor::Update(double delta_t) {
    if (!tile_) tile_ = Tile::GetFromWorldPosition(WORLD()->level_matrix(), this->world_position());

    if (tile_->visible())   node_->modifier()->set_color(ugdk::Color(1.0, 1.0, 1.0));
    else                    node_->modifier()->set_color(ugdk::Color(0.5, 0.5, 0.5));
    super::Update(delta_t);
}

void Floor::set_world_position(const ugdk::Vector2D& pos) {
    super::set_world_position(pos);
    node_->set_zindex(-FLT_MAX); // floor must be below everything
}

}



