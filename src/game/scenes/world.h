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

namespace utils {
    class Hud;
}
namespace scene {

#define WORLD() ((World *)Engine::reference()->CurrentScene())

// Classe World
// O World e' uma cena onde o jogo se desencadeara'. O World contem
// elementos como: heroi, mumias, cenario e hud.
// AVISO (podem excluir): criei o esqueleto da classe para poder
// testar a compilacao do jogo. :P
class World : public framework::Scene {
  public:
    World();
    virtual ~World();

    void Update(float delta_t);

    void AddWorldObject(sprite::WorldObject*);
    void AddFloor(sprite::Floor*);
    void AddFloor(framework::Vector2D&);
    sprite::Wall* AddWall(framework::Vector2D&);
    void AddMummy(framework::Vector2D&);
    void AddHero(framework::Vector2D&);
    void AddDoor(framework::Vector2D&);
    void FinishLevel(bool);
    int CountRemainingEnemies();
    int max_enemies() { return max_enemies_; }
    void DecreaseEnemyCount() { remaining_enemies_--; }
    void End();

    // Funcao auxiliar que transforma VETORES de coordenadas de tela para de mundo
    static Vector2D FromScreenLinearCoordinates(Vector2D screen_coords);
    
    // Funcao auxiliar que transforma VETORES de coordenadas de mundo para de tela
    static Vector2D FromWorldLinearCoordinates(Vector2D world_coords);

    // Funcao que transforma PONTOS de coordenadas de mundo para de tela
    static Vector2D FromWorldCoordinates(Vector2D screen_coords);

    sprite::Hero * hero() { return hero_; }


    int level_width();
    int level_height();
    char** level_matrix();

    void set_level_width(int width);
    void set_level_height(int height);
    void set_level_matrix(char** matrix);

    void set_hero(sprite::Hero *hero) {
        hero_ = hero;
    }

  protected:
    sprite::Hero *hero_;
    std::list<sprite::WorldObject*> world_objects_, collisionless_objects;
    framework::Layer *world_layer_;
    utils::Hud *hud_;
    int level_width_, level_height_;
    char** level_matrix_;
    int	remaining_enemies_, max_enemies_;

    void RemoveInactiveObjects();
    void RemoveAll();

  private:
    bool finished_game_, good_end_, player_exit_;

};  // class World

}  // namespace scene

#endif  // HORUSEYE_GAME_SCENES_WORLD_H_
