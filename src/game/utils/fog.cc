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

Fog::Fog() {
    this->set_light_type(LIGHT_SOURCE);
}

Fog::~Fog() {
    map<WorldObject*,Sprite*>::iterator it = light_sources_.begin();
    if(it != light_sources_.end()) {
        delete it->second;
        light_sources_.erase(it);
    }
}

Sprite* CreateLightSource(float radius) {
    return WORLD()->CreateFogTransparency(radius);
}

void Fog::AddLightSource(WorldObject* obj) {
    if(obj->light_radius() > Constants::LIGHT_RADIUS_THRESHOLD)
        if(this->light_sources_.count(obj) == 0)
            light_sources_[obj] = CreateLightSource(obj->light_radius());
}
void Fog::RemoveLightSource(WorldObject* obj) {
    map<WorldObject*,Sprite*>::iterator it = light_sources_.find(obj);
    if(it != light_sources_.end()) {
        light_sources_.erase(it);
    }
}
void Fog::UpdateLightSource(WorldObject* obj) {
    if(this->light_sources_.count(obj) > 0) {
        if(obj->light_radius() > Constants::LIGHT_RADIUS_THRESHOLD) {
            //delete light_sources_[obj];
            //light_sources_[obj] = CreateLightSource(obj->light_radius());
            // TODO, FIXME and ALL THAT. DUDE.
            Sprite *light = light_sources_[obj];
            Vector2D ellipse_coef = Vector2D(2, 1) * obj->light_radius() * 60.373835392;
            Vector2D stored_size = Vector2D(40.0f, 40.0f);
            Vector2D size = stored_size * 4.0f;
            size.x *= ellipse_coef.x / stored_size.x;
            size.y *= ellipse_coef.y / stored_size.y;
            light->set_size(size);
            light->set_hotspot(size * 0.5f);
        } else
            RemoveLightSource(obj);
    }
}

void Fog::Render() {
    if(!visible_) return;
    map<WorldObject*,Sprite*>::iterator it;
    for(it = light_sources_.begin(); it != light_sources_.end(); ++it) {
        if(it->second->image() == NULL) continue;
        Vector2D off = this->offset() - it->first->position();
        it->second->Render(NULL, off);
    }
}

}
