#ifndef HORUSEYE_EDITOR_LAYERS_FPSMETER_H_
#define HORUSEYE_EDITOR_LAYERS_FPSMETER_H_

#include "ugdk/layer.h"
#include "ugdk/sprite.h"
#include "ugdk/animationset.h"

namespace editor {

class FPSMeter : public ugdk::Layer {
  public:
    FPSMeter();
    virtual ~FPSMeter();
    virtual void Update(float delta_t);

  private:
    ugdk::Sprite       *fps_meter_[3];
    int                     fps_meter_value_[3];
    ugdk::AnimationSet *digit_set_;
    ugdk::uint32       digits_[10];
};

}

#endif /* HORUSEYE_EDITOR_LAYERS_FPSMETER_H_ */
