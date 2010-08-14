//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/utils/hud.h
// Definicao da clase Hud.
//

#ifndef HORUSEYE_GAME_UTILS_HUD_H_
#define HORUSEYE_GAME_UTILS_HUD_H_

#include "../../framework/layer.h"
#include "../scenes/world.h"

using framework::Sprite;
using framework::Animation;

namespace utils {
class Hud: public framework::Layer {
  public:
    Hud(scene::World*);
    virtual ~Hud();

    // Atualiza a camada e seus Sprites.
    virtual void Update(float delta_t);

  private:
    //Sprite** life_icons_;
    Sprite* life_bar_[2]; // 0-> Barra atual;  1->Barra do fundo;
    Image* life_bar_images_[2];
    Sprite* mana_bar_[2]; // 0-> Barra atual;  1->Barra do fundo;
    Image* mana_bar_images_[2];
    //int icon_count_;
    Sprite* enemy_counter_[5];
    Animation* enemy_animation_[5];
    int enemy_counter_value_[5];
    //Animation *animation_;

};

}

#endif /* HORUSEYE_GAME_UTILS_HUD_H_ */
