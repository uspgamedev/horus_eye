#ifndef HORUSEYE_FRAMEWORK_FOGMANAGER_H_
#define HORUSEYE_FRAMEWORK_FOGMANAGER_H_

#include <map>
#include "vector2D.h"
#include "sprite.h"

namespace framework {

class Image;
class Texture;
class FogManager {
  public:
    FogManager();
    ~FogManager();

    void InitializeTexture();
    void Release();

    Sprite* GetLightSource(const Vector2D& ellipse_coef);

  private:
    Sprite* CreateLightSource(const Vector2D& ellipse_coef);

    Image* fog_texture_;
    Vector2D stored_size_;
    std::map<std::pair<int,int>,Sprite*> fog_list_;
};

}

#endif /* HORUSEYE_FRAMEWORK_FOGMANAGER_H_ */
