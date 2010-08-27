/*
 * fog.cc
 *
 *  Created on: 13/08/2010
 *      Author: Henrique
 */

#include "fog.h"
#include "../../framework/vector2D.h"
#include "../../framework/videomanager.h"
#include "../scenes/world.h"
#include "../sprites/worldobject.h"
#include "../utils/constants.h"

namespace utils {

using namespace framework;
using namespace sprite;
using namespace scene;

Fog::Fog() {
    blank_background_ = new Image;
    blank_background_->Create(VIDEO_MANAGER()->video_size());
    blank_background_->Optimize();
}

Fog::~Fog() {
    map<WorldObject*,Sprite*>::iterator it = light_sources_.begin();
    if(it != light_sources_.end()) {
        delete it->second;
        light_sources_.erase(it);
    }
    if(blank_background_ != NULL)
        delete blank_background_;

}

Sprite* CreateLightSource(float radius) {
    Sprite *sprite = new Sprite;
    sprite->Initialize(WORLD()->CreateFogTransparency(radius));
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
    Vector2D offset = this->offset();
    map<WorldObject*,Sprite*>::iterator it;
    for(it = light_sources_.begin(); it != light_sources_.end(); ++it) {
        Vector2D off = it->first->position() - it->second->hotspot() - offset;
        blank_background_->MergeTransparency(it->second->image(), off);
    }
    offset = Vector2D();
    blank_background_->DrawTo(Engine::reference()->video_manager()->backbuffer(), offset, 0, 0 );
    blank_background_->Clear(0xff000000);
}

}
