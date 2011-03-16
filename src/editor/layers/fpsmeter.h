#ifndef HORUSEYE_EDITOR_LAYERS_FPSMETER_H_
#define HORUSEYE_EDITOR_LAYERS_FPSMETER_H_

#include "../../framework/layer.h"
#include "../../framework/sprite.h"

namespace editor {

class FPSMeter : public framework::Layer {
  public:
    FPSMeter();
    virtual ~FPSMeter() {}
    virtual void Update(float delta_t);

  private:
    framework::Sprite *fps_meter_[3];
    int fps_meter_value_[3];
};

}

#endif /* HORUSEYE_EDITOR_LAYERS_FPSMETER_H_ */
