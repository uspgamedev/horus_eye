#include <cmath>
#include <ugdk/portable/tr1.h>
#include FROM_TR1(functional)
#include <ugdk/base/engine.h>
#include <ugdk/base/resourcemanager.h>
#include <ugdk/graphic/drawable/sprite.h>
#include <ugdk/graphic/drawable/texturedrectangle.h>
#include <ugdk/graphic/node.h>
#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/collision/collisionlogic.h>
#include <pyramidworks/collision/genericcollisionlogic.h>
#include <pyramidworks/geometry/rect.h>

#include "doodadbuilder.h"

#include "game/core/coordinates.h"
#include "game/components/damageable.h"
#include "game/components/shape.h"
#include "game/components/basegraphic.h"
#include "game/map/specialwall.h"
#include "game/scenes/world.h"
#include "game/sprites/worldobject.h"
#include "game/utils/imagefactory.h"
#include "game/constants.h"
#include "game/components/orders.h"

namespace builder {
namespace DoodadBuilder {

using std::tr1::bind;
using namespace std::tr1::placeholders;
using ugdk::action::SpriteAnimationTable;
using ugdk::base::ResourceManager;
using ugdk::graphic::Drawable;
using ugdk::graphic::TexturedRectangle;
using ugdk::graphic::Sprite;
using ugdk::graphic::Node;
using pyramidworks::collision::CollisionObject;
using pyramidworks::collision::GenericCollisionLogic;
using pyramidworks::geometry::Rect;
using component::Shape;
using sprite::WorldObject;

COLLISION_DIRECT(double, DamageCollisionExtra, obj) {
    sprite::WorldObject *wobj = static_cast<sprite::WorldObject*>(obj);
    wobj->damageable()->TakeDamage(data_);
}

WorldObject* Door(const std::vector<std::string>& arguments) {
    WorldObject* wobj = new WorldObject;
    wobj->AddComponent(new component::BaseGraphic(new Sprite("stairs")));

    CollisionObject* col = new CollisionObject(WORLD()->collision_manager(), wobj);
    col->InitializeCollisionClass("Wall");
    col->AddCollisionLogic("Hero", new GenericCollisionLogic([](void*) {
        WORLD()->FinishLevel(utils::LevelManager::FINISH_WIN);
    }));
    col->set_shape(new Rect(constants::GetDouble("DOOR_BOUND_WIDTH"), constants::GetDouble("DOOR_BOUND_HEIGHT") ));
    wobj->AddComponent(new Shape(col, NULL));

    return wobj;
}

static WorldObject* buildWall(ugdk::graphic::Texture* texture) {
    WorldObject* wobj = new WorldObject;
    Drawable* drawable = new map::SpecialWall(texture);
    drawable->set_hotspot(Vector2D(53, 156));
    wobj->AddComponent(new component::BaseGraphic(drawable));
    wobj->set_identifier("Wall");

    CollisionObject* col = new CollisionObject(WORLD()->collision_manager(), wobj);
    col->InitializeCollisionClass("Wall");
    col->set_shape(new pyramidworks::geometry::Rect(1.0, 1.0));
    
    CollisionObject* vis = new CollisionObject(WORLD()->visibility_manager(), wobj);
    vis->InitializeCollisionClass("Opaque");
    vis->set_shape(new pyramidworks::geometry::Rect(1.0, 1.0));

    wobj->AddComponent(new Shape(col, vis));
    return wobj;
}

WorldObject* Wall(const std::vector<std::string>& arguments) {
    utils::ImageFactory factory;
    return buildWall(ResourceManager::GetTextureFromFile("images/wall-simple.png"));
}

WorldObject* Entry(const std::vector<std::string>& arguments) {
    utils::ImageFactory factory;
    return buildWall(ResourceManager::GetTextureFromFile("images/door.png"));
}

Node* Floor(const ugdk::math::Vector2D& position) {
    utils::ImageFactory imagefactory;
    Node* floor = new Node(imagefactory.FloorImage());
    floor->geometry().set_offset(core::FromWorldCoordinates(position));
    return floor;
}

} // namespace DoodadBuilder
} // namespace builder
