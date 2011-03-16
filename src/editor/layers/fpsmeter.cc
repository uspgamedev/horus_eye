#include "fpsmeter.h"
#include "../../framework/image.h"
#include "../../framework/engine.h"
#include "../../framework/videomanager.h"
#include "../../framework/animation.h"


#define FPS_BAR_OFFSET_X  10
#define FPS_BAR_OFFSET_Y  30

#define NUMBER_WIDTH 18
#define NUMBER_HEIGHT 16

using namespace framework;

namespace editor {

FPSMeter::FPSMeter() {
    Image* number = VIDEO_MANAGER()->LoadImage("data/images/numbers2.png");
    number->set_frame_size(Vector2D(NUMBER_WIDTH, NUMBER_HEIGHT));

    for(int i = 0; i < 3; ++i) {
        (fps_meter_[i] = new Sprite)->Initialize(number);
        fps_meter_[i]->set_position(Vector2D(
                        FPS_BAR_OFFSET_X + NUMBER_WIDTH*i + 0.0f, FPS_BAR_OFFSET_Y + 0.0f));
        AddSprite(fps_meter_[i]);
        fps_meter_value_[i] = 0;
    }
}

void FPSMeter::Update(float delta_t) {
    Layer::Update(delta_t);
    int digit[3];
    int fps = Engine::reference()->current_fps();
    if(fps > 999) fps = 999;
    for(int i = 2; i >= 0; --i) {
        digit[i] = fps % 10;
        fps /= 10;
    }
    for(int i = 0; i < 3; ++i) {
        if(digit[i] != fps_meter_value_[i]) {
            fps_meter_value_[i] = digit[i];
            fps_meter_[i]->animation()->set_framelist(fps_meter_value_[i], -1);
        }
    }
}

}
