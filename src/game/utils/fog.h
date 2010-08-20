/*
 * fog.h
 *
 *  Created on: 13/08/2010
 *      Author: Henrique
 */

#ifndef HORUSEYE_GAME_UTILS_FOG_H_
#define HORUSEYE_GAME_UTILS_FOG_H_

#include "../../framework/layer.h"
#include <map>

namespace sprite {
class WorldObject;
}

namespace utils {

using std::map;

class Fog: public framework::Layer {
  public:
    Fog(bool single_light);
    virtual ~Fog();

    void SetLightSource(sprite::WorldObject*);

    void AddLightSource(sprite::WorldObject*);
    void RemoveLightSource(sprite::WorldObject*);

    virtual void Render();

  private:
    bool single_light_;
    framework::Image* blank_background_;
    map<sprite::WorldObject*, framework::Sprite*> light_sources_;
};

}

#endif /* HORUSEYE_GAME_UTILS_FOG_H_ */
