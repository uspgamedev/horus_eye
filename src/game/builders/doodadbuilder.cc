#include <functional>
#include <ugdk/base/engine.h>
#include <ugdk/base/resourcemanager.h>
#include <ugdk/graphic/drawable/sprite.h>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/light.h>
#include <ugdk/time/timeaccumulator.h>
#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/collision/collisionlogic.h>
#include <pyramidworks/collision/genericcollisionlogic.h>
#include <pyramidworks/geometry/circle.h>
#include <pyramidworks/geometry/rect.h>

#include "doodadbuilder.h"

#include "game/components/damageable.h"
#include "game/components/logic/follower.h"
#include "game/components/logic/wall.h"
#include "game/scenes/world.h"
#include "game/sprites/worldobject.h"
#include "game/utils/imagefactory.h"
#include "game/utils/constants.h"

namespace builder {

using std::tr1::bind;
using namespace std::tr1::placeholders;
using ugdk::action::AnimationSet;
using ugdk::base::ResourceManager;
using ugdk::graphic::Sprite;
using pyramidworks::collision::CollisionObject;
using pyramidworks::collision::GenericCollisionLogic;
using component::Creature;
using component::Follower;
using sprite::WorldObject;
using utils::Constants;

COLLISION_DIRECT(double, DamageCollisionExtra, obj) {
    sprite::WorldObject *wobj = static_cast<sprite::WorldObject*>(obj);
    wobj->damageable()->TakeDamage(data_);
}

COLLISION_DIRECT(scene::World*, WinCollision, obj) {
    if (data_->CountRemainingEnemies() == 0 && data_->num_button_not_pressed().Get() <= 0)
        data_->FinishLevel(utils::LevelManager::FINISH_WIN);
}

WorldObject* DoodadBuilder::Door(scene::World* world) {
    WorldObject* wobj = new WorldObject;
    wobj->node()->set_drawable(new Sprite(world->image_factory()->DoorImage()));

    CollisionObject* col = new CollisionObject(world->collision_manager(), wobj);
    col->InitializeCollisionClass("Wall");
    col->AddCollisionLogic("Hero", new WinCollision(world));
    col->set_shape(new pyramidworks::geometry::Rect(Constants::DOOR_BOUND_WIDTH, Constants::DOOR_BOUND_HEIGHT));
    wobj->set_collision_object(col);

    return wobj;
}

static WorldObject* buildWall(ugdk::graphic::Spritesheet* sheet) {
    WorldObject* wobj = new WorldObject;
    wobj->set_logic(new component::Wall(wobj, sheet));

    CollisionObject* col = new CollisionObject(WORLD()->collision_manager(), wobj);
    col->InitializeCollisionClass("Wall");
    col->set_shape(new pyramidworks::geometry::Rect(1.0, 1.0));
    wobj->set_collision_object(col);
    return wobj;
}

WorldObject* DoodadBuilder::Wall() {
    utils::ImageFactory factory;
    return buildWall(factory.WallImage());
}

WorldObject* DoodadBuilder::Entry() {
    utils::ImageFactory factory;
    return buildWall(factory.EntryImage());
}


class ButtonLogic : public component::Logic {
public:
	ButtonLogic(Sprite* sprite) : pressed_(false), sprite_(sprite) {
		sprite_->SetDefaultFrame(2); 
	}

	void Update(double dt) {}

	void Press() {
		if(!pressed_) {
			sprite_->SetDefaultFrame(0);
			WORLD()->num_button_not_pressed() -= 1;
			pressed_ = true;
		}
	}

private:
	bool pressed_;
	Sprite* sprite_;
};

static void CollisionButton(ButtonLogic* button_logic, void*) {
	button_logic->Press();
}

WorldObject* DoodadBuilder::Button() {
    utils::ImageFactory factory;
	WorldObject* wobj = new WorldObject;

	Sprite* sprite = new Sprite(factory.TileSwitchImage());
	ButtonLogic* logic = new ButtonLogic(sprite);

    wobj->node()->set_drawable(sprite);
	wobj->set_logic(logic);

	CollisionObject* col = new CollisionObject(WORLD()->collision_manager(), wobj);
    col->InitializeCollisionClass("Button");
    //col->AddCollisionLogic("Hero", new WinCollision(world));
	col->AddCollisionLogic("Hero", new GenericCollisionLogic(bind(CollisionButton, logic, _1)));
    col->set_shape(new pyramidworks::geometry::Rect(0.75, 0.75));
    wobj->set_collision_object(col);

	return wobj;
}

} // namespace builder