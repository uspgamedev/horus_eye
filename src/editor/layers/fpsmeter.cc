#include "fpsmeter.h"
#include "ugdk/image.h"
#include "ugdk/engine.h"
#include "ugdk/videomanager.h"
#include "ugdk/animation.h"


#define FPS_BAR_OFFSET_X  10
#define FPS_BAR_OFFSET_Y  30

#define NUMBER_WIDTH 18
#define NUMBER_HEIGHT 16

using namespace ugdk;

namespace editor {

FPSMeter::FPSMeter() {
    Image* number = VIDEO_MANAGER()->LoadImage("data/images/numbers2.png");
    number->set_frame_size(Vector2D(NUMBER_WIDTH, NUMBER_HEIGHT));
    // TODO: change to opengl lists!
    digit_set_ = new AnimationSet();
    for (int i = 0; i < 10; i++) {
        string digit_name(1, '0'+i);
        digit_set_->Add(digit_name, i, -1);
        digits_[i] = digit_set_->MakeIndex(digit_name);
    }
    for(int i = 0; i < 3; ++i) {
        (fps_meter_[i] = new Sprite)->Initialize(number, digit_set_);
        fps_meter_[i]->set_position(Vector2D(
                        FPS_BAR_OFFSET_X + NUMBER_WIDTH*i + 0.0f, FPS_BAR_OFFSET_Y + 0.0f));
        AddSprite(fps_meter_[i]);
        fps_meter_value_[i] = 0;
    }
}

FPSMeter::~FPSMeter() {
    digit_set_->Release();
    delete digit_set_;
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
            fps_meter_[i]->SelectAnimation(digits_[fps_meter_value_[i]]);
        }
    }
}

}
