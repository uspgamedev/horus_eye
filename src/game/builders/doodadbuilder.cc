#include <cmath>
#include <functional>
#include <ugdk/system/engine.h>
#include <ugdk/resource/module.h>
#include <ugdk/graphic/drawable/sprite.h>
#include <ugdk/graphic/drawable/texturedrectangle.h>
#include <ugdk/graphic/node.h>
#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/geometry/rect.h>

#include "doodadbuilder.h"

#include "game/builders/collision.h"
#include "game/core/coordinates.h"
#include "game/components/damageable.h"
#include "game/components/body.h"
#include "game/components/graphic.h"
#include "game/map/specialwall.h"
#include "game/scenes/world.h"
#include "game/sprites/worldobject.h"
#include "game/utils/imagefactory.h"
#include "game/constants.h"
#include "game/components/orders.h"

namespace builder {
namespace DoodadBuilder {

using std::bind;
using namespace std::placeholders;
using ugdk::action::SpriteAnimationTable;
using ugdk::action::Entity;
using ugdk::graphic::Drawable;
using ugdk::graphic::TexturedRectangle;
using ugdk::graphic::Sprite;
using ugdk::graphic::Node;
using pyramidworks::collision::CollisionObject;
using pyramidworks::geometry::Rect;
using component::Body;
using sprite::WorldObject;

WorldObject* Door(const std::vector<std::string>& arguments) {
    WorldObject* wobj = new WorldObject;
    wobj->AddComponent(component::Graphic::Create(std::shared_ptr<ugdk::graphic::Drawable>(new Sprite("stairs"))));

    CollisionObject* col = new CollisionObject(wobj, "Wall", new Rect(constants::GetDouble("DOOR_BOUND_WIDTH"), constants::GetDouble("DOOR_BOUND_HEIGHT") ));
    col->AddCollisionLogic("Hero", [](const CollisionObject*) { WORLD()->FinishLevel(utils::LevelManager::FINISH_WIN); });
    wobj->AddComponent(new Body(col, NULL));

    return wobj;
}

static WorldObject* buildWall(ugdk::graphic::Texture* texture) {
    WorldObject* wobj = new WorldObject;
    if(texture) {
        Drawable* drawable = new map::SpecialWall(texture);
        drawable->set_hotspot(Vector2D(53, 156));
        wobj->AddComponent(component::Graphic::Create(std::shared_ptr<ugdk::graphic::Drawable>(drawable)));
    }
    wobj->set_identifier("Wall");

    CollisionObject* col = new CollisionObject(wobj, "Wall", new pyramidworks::geometry::Rect(1.0, 1.0));
    
    //CollisionObject* vis = new CollisionObject(WORLD()->visibility_manager(), wobj);
    //vis->InitializeCollisionClass("Opaque");
    //vis->set_shape(new pyramidworks::geometry::Rect(1.0, 1.0));
      
    //wobj->AddComponent(new Body(col, vis));

    wobj->AddComponent(new Body(col, nullptr));

    return wobj;
}

WorldObject* Wall(const std::vector<std::string>& arguments) {
    return buildWall(ugdk::resource::GetTextureFromFile("images/wall-simple.png"));
}

WorldObject* InvisibleWall(const std::vector<std::string>& arguments) {
    return buildWall(ugdk::resource::GetTextureFromFile("images/wall-shortened.png"));
}

WorldObject* BurntWall(const std::vector<std::string>& arguments) {
    return buildWall(ugdk::resource::GetTextureFromFile("images/wall-burnt.png"));
}

WorldObject* Entry(const std::vector<std::string>& arguments) {
    return buildWall(ugdk::resource::GetTextureFromFile("images/door.png"));
}

} // namespace DoodadBuilder
} // namespace builder
