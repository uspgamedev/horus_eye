#ifndef HORUSEYE_FRAMEWORK_FOGMANAGER_H_
#define HORUSEYE_FRAMEWORK_FOGMANAGER_H_

#include <map>
#include "vector2D.h"

namespace framework {

class Image;
class Texture;
class FogManager {
  public:
    FogManager();
    ~FogManager();

    Image* GetLightSource(const Vector2D& ellipse_coef);

  private:
    Image* CreateLightSource(const Vector2D& ellipse_coef);

    Texture* fog_texture_;
    Vector2D stored_size_;
    std::map<std::pair<int,int>,Image*> fog_list_;
};

}

#endif /* HORUSEYE_FRAMEWORK_FOGMANAGER_H_ */
