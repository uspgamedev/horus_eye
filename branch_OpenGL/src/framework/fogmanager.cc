#include "fogmanager.h"
#include "image.h"

namespace framework {

using namespace std;

FogManager::~FogManager() {
    map<pair<int,int>,Image*>::iterator it = fog_list_.begin();
    if(it != fog_list_.end()) {
        delete it->second;
        fog_list_.erase(it);
    }
}

Image* FogManager::GetLightSource(const Vector2D& ellipse_coef) {
    pair<int,int> coef(static_cast<int>(ellipse_coef.x),
                       static_cast<int>(ellipse_coef.y));
    if(fog_list_.count(coef) > 0)
        return fog_list_[coef];
    Image* fog = new Image;
    if(fog->CreateFogTransparency(ellipse_coef))
        return fog_list_[coef] = fog;
    delete fog;
    return NULL;
}
}
