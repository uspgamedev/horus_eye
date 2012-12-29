#include <cmath>
#include <ugdk/portable/tr1.h>
#include FROM_TR1(functional)
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

#include "game/core/coordinates.h"
#include "game/components/damageable.h"
#include "game/components/base.h"
#include "game/components/shape.h"
#include "game/components/logic/follower.h"
#include "game/components/basegraphic.h"
#include "game/scenes/world.h"
#include "game/sprites/worldobject.h"
#include "game/utils/imagefactory.h"
#include "game/constants.h"
#include "game/components/orders.h"

namespace builder {
namespace DoodadBuilder {

using std::tr1::bind;
using namespace std::tr1::placeholders;
using ugdk::action::AnimationSet;
using ugdk::base::ResourceManager;
using ugdk::graphic::Sprite;
using ugdk::graphic::Node;
using ugdk::time::TimeAccumulator;
using pyramidworks::collision::CollisionObject;
using pyramidworks::collision::GenericCollisionLogic;
using component::Follower;
using component::Shape;
using sprite::WorldObject;

COLLISION_DIRECT(double, DamageCollisionExtra, obj) {
    sprite::WorldObject *wobj = static_cast<sprite::WorldObject*>(obj);
    wobj->damageable()->TakeDamage(data_);
}

void WinCollision(void*) {
    WORLD()->FinishLevel(utils::LevelManager::FINISH_WIN);
}

WorldObject* Door(const std::vector<std::string>& arguments) {
    WorldObject* wobj = new WorldObject;
    wobj->AddComponent(new component::BaseGraphic(new Sprite("stairs")));

    CollisionObject* col = new CollisionObject(WORLD()->collision_manager(), wobj);
    col->InitializeCollisionClass("Wall");
    col->AddCollisionLogic("Hero", new GenericCollisionLogic(WinCollision));
    col->set_shape(new pyramidworks::geometry::Rect(constants::GetDouble("DOOR_BOUND_WIDTH"), constants::GetDouble("DOOR_BOUND_HEIGHT") ));
    wobj->AddComponent(new Shape(col, NULL));

    return wobj;
}

static WorldObject* buildWall(ugdk::graphic::Spritesheet* sheet) {
    WorldObject* wobj = new WorldObject;
    wobj->AddComponent(new component::BaseGraphic(new Sprite(sheet)));
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
    return buildWall(factory.WallImage());
}

WorldObject* Entry(const std::vector<std::string>& arguments) {
    utils::ImageFactory factory;
    return buildWall(factory.EntryImage());
}

static void PIZZAMATAHERO() {
    WORLD()->hero()->damageable()->TakeDamage(12341231231.0);
}

static void WorldPressButton() {
    //WORLD()->num_button_not_pressed() -= 1;
}


class ButtonLogic : public component::Base {
public:
    ButtonLogic(Sprite* sprite, std::tr1::function<void (void)> callback)
        :	pressed_(false),
            sprite_(sprite),
            callback_(callback) {
        sprite_->SetDefaultFrame(2); 
    }

    void Update(double dt) {}

    void Press() {
        if(!pressed_) {
            sprite_->SetDefaultFrame(0);
            callback_();
            pressed_ = true;
        }
    }

private:
    bool pressed_;
    Sprite* sprite_;
    std::tr1::function<void (void)> callback_;
};

static void CollisionButton(ButtonLogic* button_logic, void*) {
    button_logic->Press();
}

WorldObject* Button(const std::vector<std::string>& arguments) {
    utils::ImageFactory factory;
    Sprite* sprite = new Sprite(factory.TileSwitchImage());
    WorldObject* wobj = new WorldObject;
    wobj->AddComponent(new component::BaseGraphic(sprite));

    ButtonLogic* logic;
    if(arguments.size() > 0 && arguments[0].compare("pizza") == 0) {
        logic = new ButtonLogic(sprite, PIZZAMATAHERO);
        //WORLD()->num_button_not_pressed() -= 1;
    } else
        logic = new ButtonLogic(sprite, WorldPressButton);

    wobj->AddComponent(logic, "button", component::orders::LOGIC);
    wobj->component<component::BaseGraphic>()->set_layer(scene::BACKGROUND_LAYER);

    CollisionObject* col = new CollisionObject(WORLD()->collision_manager(), wobj);
    col->InitializeCollisionClass("Button");
    col->AddCollisionLogic("Hero", new GenericCollisionLogic(bind(CollisionButton, logic, _1)));
    col->AddCollisionLogic("Block", new GenericCollisionLogic(bind(CollisionButton, logic, _1)));
    col->set_shape(new pyramidworks::geometry::Rect(0.75, 0.75));
    wobj->AddComponent(new Shape(col, NULL));

    return wobj;
}

class BlockLogic : public component::Base {
public:
    BlockLogic(WorldObject* owner) : owner_(owner), 
        time_left_(new TimeAccumulator(0)) {}

    void Update(double dt) {
        if(time_left_->Expired()) {
            // Not moving anywhere
            last_stable_position_ = owner_->world_position();
        } else {
            MoveBlock(dt);
        }
    }

    void PushToward(const Vector2D& pushdir) {
        if(!time_left_->Expired()) return; // One cannot affect a block while it moves.
        direction_ = pushdir;

        if(pushdir.x > fabs(pushdir.y)) 
            direction_ = Vector2D(-1, 0);
        else if (pushdir.x < -fabs(pushdir.y))
            direction_ = Vector2D(+1, 0);
        else if (pushdir.y > fabs(pushdir.x))
            direction_ = Vector2D(0, -1);
        else
            direction_ = Vector2D(0, +1);

        static double BLOCK_MOVE_SPEED = 2.0;
        direction_ = direction_ * BLOCK_MOVE_SPEED;

        time_left_->Restart(500);
    }

    void RevertPosition() {
        owner_->set_world_position(last_stable_position_);
        time_left_->Restart(0);
    }

    WorldObject* owner() { return owner_; }

private:
    void MoveBlock(double dt) {
        Vector2D newpos = owner_->world_position();
        newpos += direction_ * dt;
        owner_->set_world_position(newpos);
    }

    WorldObject* owner_;
    Vector2D last_stable_position_, direction_;
    TimeAccumulator* time_left_;
};

static void PushOnCollision(BlockLogic* logic, void* obj) {
    Vector2D pushdir = (((WorldObject *)obj)->world_position() - logic->owner()->world_position()).Normalize();
    logic->PushToward(pushdir);
}

static void InvalidMovementCollision(BlockLogic* data, void* obj) {
    data->RevertPosition();
}

WorldObject* Block(const std::vector<std::string>& arguments) {
    utils::ImageFactory factory;
    Sprite* sprite = new Sprite(factory.WallImage());
    WorldObject* wobj = new WorldObject;
    wobj->AddComponent(new component::BaseGraphic(sprite));
    
    BlockLogic* logic = new BlockLogic(wobj);
    wobj->AddComponent(logic, "block", component::orders::LOGIC);

    //wobj->node()->modifier()->set_scale(Vector2D(1.0,0.7));
    // TODO: different GDD

    CollisionObject* col = new CollisionObject(WORLD()->collision_manager(), wobj);
    col->InitializeCollisionClass("Block");
    col->AddCollisionLogic("Projectile", new GenericCollisionLogic(bind(PushOnCollision, logic, _1)));
    col->AddCollisionLogic("Wall", new GenericCollisionLogic(bind(InvalidMovementCollision, logic, _1)));
    col->set_shape(new pyramidworks::geometry::Rect(0.75, 0.75));
    wobj->AddComponent(new Shape(col, NULL));

    return wobj;
}

Node* Floor(const ugdk::Vector2D& position) {
    utils::ImageFactory imagefactory;
    Node* floor = new Node(imagefactory.FloorImage());
    floor->modifier()->set_offset(core::FromWorldCoordinates(position));
    return floor;
}

} // namespace DoodadBuilder
} // namespace builder
