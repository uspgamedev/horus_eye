//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/utils/hud.h
// Definicao da clase Hud.
//

#ifndef HUD_H_
#define HUD_H_

#include "../../framework/layer.h"
#include "../scenes/world.h"

using namespace scene;

namespace utils {
class Hud: public framework::Layer {
  public:
    Hud(World*);
    virtual ~Hud();

    // Atualiza a camada e seus Sprites.
    virtual void Update(float delta_t);

  private:
    framework::Sprite** life_icons_;
    framework::Sprite* enemy_counter_[5];
    framework::Animation* enemy_animation_[5];
    int enemy_counter_value_[5];
    framework::Animation *animation_;
};

}

#endif /* HUD_H_ */
