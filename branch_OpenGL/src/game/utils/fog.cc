/*
 * fog.cc
 *
 *  Created on: 13/08/2010
 *      Author: Henrique
 */

#include "fog.h"
#include "../../framework/vector2D.h"
#include "../../framework/engine.h"
#include "../../framework/videomanager.h"
#include "../../framework/image.h"
#include "../scenes/world.h"
#include "../sprites/worldobject.h"
#include "../utils/constants.h"

namespace utils {

using namespace framework;
using namespace sprite;
using namespace scene;

Fog::Fog() {}

Fog::~Fog() {
    map<WorldObject*,Sprite*>::iterator it = light_sources_.begin();
    if(it != light_sources_.end()) {
        delete it->second;
        light_sources_.erase(it);
    }
}

Sprite* CreateLightSource(float radius) {
    Sprite *sprite = new Sprite;
    sprite->Initialize(WORLD()->CreateFogTransparency(radius));
	if(sprite->image() != NULL)
		sprite->set_hotspot(sprite->image()->frame_size() * 0.5f);
    return sprite;
}

void Fog::AddLightSource(WorldObject* obj) {
    if(obj->light_radius() > Constants::LIGHT_RADIUS_THRESHOLD)
        if(this->light_sources_.count(obj) == 0)
            light_sources_[obj] = CreateLightSource(obj->light_radius());
}
void Fog::RemoveLightSource(WorldObject* obj) {
    map<WorldObject*,Sprite*>::iterator it = light_sources_.find(obj);
    if(it != light_sources_.end()) {
        delete it->second;
        light_sources_.erase(it);
    }
}
void Fog::UpdateLightSource(WorldObject* obj) {
    if(this->light_sources_.count(obj) > 0) {
        if(obj->light_radius() > Constants::LIGHT_RADIUS_THRESHOLD) {
            delete light_sources_[obj];
            light_sources_[obj] = CreateLightSource(obj->light_radius());
        } else
            RemoveLightSource(obj);
    }
}

void Fog::Render() {
    if(!visible_) return;
    Vector2D offset = this->offset();
    map<WorldObject*,Sprite*>::iterator it;
    for(it = light_sources_.begin(); it != light_sources_.end(); ++it) {
        if(it->first != WORLD()->hero() || it->second->image() == NULL) continue;
        Vector2D off = it->first->position() - it->second->hotspot() - offset;
        it->second->image()->DrawTo(it->second->image(), off, 0, 0);
    }
}

}
