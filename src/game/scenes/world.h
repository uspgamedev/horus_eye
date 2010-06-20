//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/scenes/world.h
// Definicao da classe World.
//

#ifndef HORUSEYE_GAME_SCENES_WORLD_H_
#define HORUSEYE_GAME_SCENES_WORLD_H_

#include <list>
#include "../../framework/scene.h"
#include "../../framework/vector2D.h"
#include "../sprites/hero.h"
#include "../sprites/worldobject.h"
#include "../sprites/floor.h"
#include "../sprites/wall.h"


namespace scene {

// Classe World
// O World e' uma cena onde o jogo se desencadeara'. O World contem
// elementos como: heroi, mumias, cenario e hud.
// AVISO (podem excluir): criei o esqueleto da classe para poder
// testar a compilacao do jogo. :P
class World : public framework::Scene {
  public:
    World() : Scene(), world_layer_(new framework::Layer()) {
        AddLayer(world_layer_);
        hero_ = NULL;
    }
    virtual ~World();

    void Update(float delta_t);

    void AddWorldObject(sprite::WorldObject*);
    void AddFloor(sprite::Floor*);
    void FinishLevel(bool);
    int CountRemainingEnemies();
    sprite::Hero * hero();
    void set_hero(sprite::Hero *hero) {
        hero_ = hero;
    }
    framework::Vector2D camera_position() { return camera_position_; }
    void set_camera_position(framework::Vector2D &vector) {
        camera_position_ = vector;
    }

  protected:
    sprite::Hero *                  hero_;
    std::list<sprite::WorldObject*> world_objects_,
                                    collidable_;
    framework::Vector2D             camera_position_;
    framework::Layer                *world_layer_;

    void RemoveInactiveObjects();

};  // class World

}  // namespace scene

#endif  // HORUSEYE_GAME_SCENES_WORLD_H_
