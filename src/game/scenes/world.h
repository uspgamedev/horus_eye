//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/scenes/world.h
// Definicao da classe World.
//

#ifndef HORUSEYE_GAME_SCENES_WORLD_H_
#define HORUSEYE_GAME_SCENES_WORLD_H_

#include <list.h>
#include "../../framework/scene.h"
#include "../../framework/vector2D.h"
#include "../sprites/worldobject.h"
#include "../sprites/hero.h"

namespace scene {

// Classe World
// O World e' uma cena onde o jogo se desencadeara'. O World contem
// elementos como: heroi, mumias, cenario e hud.
// AVISO (podem excluir): criei o esqueleto da classe para poder
// testar a compilacao do jogo. :P
class World : public framework::Scene {
  public:
    World() : Scene() { }
    virtual ~World() { }

    void Update(float delta_t) { } // TODO no .cc

    void AddWorldObject(sprite::WorldObject*);
    void AddFloor(sprite::Floor*);
    void FinishLevel(bool);
    int CountRemainingEnemies();
    Hero * hero();

  protected:
    sprite::Hero *                  hero_;
    std::list<sprite::WorldObject*> world_objects_,
                                    collidable_;
    framework::Vector2D             camera_position_;
    framework::Laye                 world_layer_;

    void RemoveInactiveObjects();

};  // class World

}  // namespace scene

#endif  // HORUSEYE_GAME_SCENES_WORLD_H_
