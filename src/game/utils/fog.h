#ifndef HORUSEYE_GAME_UTILS_FOG_H_
#define HORUSEYE_GAME_UTILS_FOG_H_

#include "../../framework/layer.h"
#include <map>

namespace framework {
class Sprite;
class Image;
}
namespace sprite {
class WorldObject;
}
namespace utils {

using std::map;

class Fog: public framework::Layer {
  public:
    Fog();
    ~Fog();

    void AddLightSource(sprite::WorldObject*);
    void RemoveLightSource(sprite::WorldObject*);
    void UpdateLightSource(sprite::WorldObject*);

    bool IsIluminated(sprite::WorldObject*) { return true; }
    void UpdateVisibility() { }

    virtual void Render();

  private:
    map<sprite::WorldObject*, framework::Sprite*> light_sources_;
};

}

#endif /* HORUSEYE_GAME_UTILS_FOG_H_ */
