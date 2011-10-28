#include "floor.h"
#include <ugdk/graphic/videomanager.h>
#include <ugdk/base/engine.h>
#include <ugdk/action/animationset.h>
#include "game/utils/circleobject.h"
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

Floor::Floor(ugdk::Image* image) {
    Initialize(image);
    set_hotspot(Vector2D(Constants::FLOOR_HOTSPOT_X, Constants::FLOOR_HOTSPOT_Y));
    this->collision_type_ = NO_COLLISION;
    bound_ = new CircleObject(0.0f);
    tile_ = NULL;
    SetDefaultFrame(CLEAR);
}

void Floor::Update(float delta_t) {
    if (!tile_)
        tile_ = Tile::GetFromWorldPosition(WORLD()->level_matrix(), this->world_position());
    if (tile_->visible())   SetDefaultFrame(CLEAR);
    else                    SetDefaultFrame(DARK);
    WorldObject::Update(delta_t);
    set_zindex(-FLT_MAX); // chao deve ficar a baixo de tudo/
}

}



