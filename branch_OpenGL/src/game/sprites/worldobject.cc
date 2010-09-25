//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/sprites/worldobject.cc
// Implementacao da classe WorldObject.
//

#include <cmath>
#include "worldobject.h"
#include "../scenes/world.h"
#include "../utils/circleobject.h"
#include "../utils/visionstrategy.h"

namespace sprite {

using namespace framework;
using namespace scene;
using namespace utils;

WorldObject::WorldObject()
    : bound_(NULL),
      status_(STATUS_ACTIVE),
      collision_type_(NO_COLLISION),
      light_radius_(0.0f)
{}

WorldObject::~WorldObject() {
    delete bound_;
}

void WorldObject::Update(float dt) {
    Sprite::Update(dt);
    set_zindex(World::FromWorldLinearCoordinates(world_position()).y); // Seta zindex
}

void WorldObject::set_light_radius(float radius) {
    light_radius_ = radius;
    WORLD()->UpdateLightSource(this);
}

bool WorldObject::IsColliding(WorldObject* obj) const {
    return bound_->Intersects(obj->bound());
}

void WorldObject::HandleCollision(WorldObject* obj) {
//     double dispatch
//     http://en.wikipedia.org/wiki/Double_dispatch
    obj->CollidesWith(this);
}

void WorldObject::set_world_position(const framework::Vector2D& pos) {
   bound_->set_position(pos);
   set_position(World::FromWorldCoordinates(pos));
}

void WorldObject::Render(Image *back_buffer, Vector2D &offset, float color_coef) {
    if(!this->visible()) return;

    VisionStrategy vision;
    std::list<WorldObject*> list = WORLD()->light_sources();

    float light = 0.0f; // If there's no light, this object is "invisible" (complete black)
    std::list<WorldObject*>::iterator it;
    for(it = list.begin(); it != list.end(); ++it) {
        Vector2D dist = (*it)->world_position() - this->world_position();
        // First you check if the light source is reasonably close,
        // aproximating the distance using norm 1
        if(fabs(dist.x) + fabs(dist.y) > 10.0f)
            continue;

        // Next, you check if the light source is visible
        // floor ignores this check for performance reasons
        if(!(collision_type_ & NO_COLLISION))
            if(!vision.IsLightVisible((*it)->world_position(), this->world_position()))
                continue;

        // Now we calculate how much this light source illuminates this object
        dist = this->position() - (*it)->position();
        Vector2D ellipse_coef = World::FogEllipseCoordinates((*it)->light_radius());
        dist.x /= ellipse_coef.x;
        dist.y /= ellipse_coef.y;
        float distance = Vector2D::InnerProduct(dist, dist);
        light += exp(-distance * 5.5412635451584261462455391880218);

        // If the object is completely illuminated, we don't have to check more lights
        if(light >= 1.0f) {
            light = 1.0f;
            break;
        }
    }
    Sprite::Render(back_buffer, offset, color_coef * light);
}

}  // namespace sprite
