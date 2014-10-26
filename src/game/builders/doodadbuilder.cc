#include "doodadbuilder.h"

#include "game/builders/collision.h"
#include "game/core/coordinates.h"
#include "game/components/damageable.h"
#include "game/components/body.h"
#include "game/components/graphic.h"
#include "game/components/orders.h"
#include "game/map/specialwall.h"
#include "game/map/room.h"
#include "game/scenes/world.h"
#include "game/sprites/worldobject.h"
#include "game/constants.h"   

#include <ugdk/system/compatibility.h>
#include <ugdk/system/engine.h>
#include <ugdk/resource/module.h>
#include <ugdk/graphic/sprite.h>
#include <pyramidworks/collision/collisiondata.h>
#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/geometry/rect.h>

#include <cmath>
#include <functional>

namespace builder {
namespace DoodadBuilder {

using std::bind;
using namespace std::placeholders;
using pyramidworks::collision::CollisionObject;
using pyramidworks::geometry::Rect;
using component::Body;
using sprite::WorldObject;

sprite::WObjPtr Door(const std::vector<std::string>& arguments) {
    sprite::WObjPtr wobj = WorldObject::Create();
    wobj->set_identifier("Door");

    auto graphic = component::Graphic::CreateWithSingleFrame("scenery", "stairs");
    graphic->set_render_offset(-Vector2D(76.5, 63.5));
    wobj->AddComponent(graphic);

    CollisionObject* col = new CollisionObject(wobj.get(), "Wall", ugdk::MakeUnique<Rect>(constants::GetDouble("DOOR_BOUND_WIDTH"), constants::GetDouble("DOOR_BOUND_HEIGHT") ));
    col->AddCollisionLogic("Hero", [](const CollisionObject* obj) { 
        dynamic_cast<WorldObject*>(obj->data())->current_room()->level()->Finish();
    });
    wobj->AddComponent(new Body(col, NULL));

    return wobj;
}

namespace {


sprite::WObjPtr buildWall(const std::string& frame) {
    sprite::WObjPtr wobj = WorldObject::Create();
    wobj->AddComponent(component::Graphic::Create([frame](ugdk::graphic::Primitive& p) {
        map::PreparePrimitiveSpecialWall(p, ugdk::resource::GetTextureAtlasFromTag("wall"), frame);
    }));
    wobj->graphic()->set_render_offset(-Vector2D(53, 156));
    wobj->set_identifier("Wall");

    CollisionObject* col = new CollisionObject(wobj.get(), "Wall", ugdk::MakeUnique<Rect>(1.0, 1.0));
    
    //CollisionObject* vis = new CollisionObject(WORLD()->visibility_manager(), wobj);
    //vis->InitializeCollisionClass("Opaque");
    //vis->set_shape(new pyramidworks::geometry::Rect(1.0, 1.0));
      
    //wobj->AddComponent(new Body(col, vis));

    wobj->AddComponent(new Body(col, nullptr));

    return wobj;
}

}

sprite::WObjPtr Wall(const std::vector<std::string>& arguments) {
    return buildWall("wall-simple");
}

sprite::WObjPtr InvisibleWall(const std::vector<std::string>& arguments) {
    return buildWall("wall-shortened");
}

sprite::WObjPtr BurntWall(const std::vector<std::string>& arguments) {
    return buildWall("wall-burnt");
}

sprite::WObjPtr Entry(const std::vector<std::string>& arguments) {
    return buildWall("wall-simple");
}

} // namespace DoodadBuilder
} // namespace builder
