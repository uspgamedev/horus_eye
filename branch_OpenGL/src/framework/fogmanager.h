#ifndef HORUSEYE_FRAMEWORK_FOGMANAGER_H_
#define HORUSEYE_FRAMEWORK_FOGMANAGER_H_

#include <map>
#include "vector2D.h"

namespace framework {

class Image;
class FogManager {
  public:
    FogManager() {}
    ~FogManager();

    Image* GetLightSource(const Vector2D& ellipse_coef);

  private:
    std::map<std::pair<int,int>,Image*> fog_list_;
};

}

#endif /* HORUSEYE_FRAMEWORK_FOGMANAGER_H_ */
