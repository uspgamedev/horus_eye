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
#include "../utils/levelmanager.h"
#include "../sprites/itembuilder.h"
#include "../sprites/mummybuilder.h"
#include"../utils/imagefactory.h"
namespace utils {
class Hud;
class Fog;
}
namespace sprite {
class Hero;
class WorldObject;
class Floor;
class Wall;
class Mummy;
}

using framework::Vector2D;
using framework::Image;

namespace scene {

#define WORLD() (utils::LevelManager::reference()->get_current_level() )

// Classe World
// O World e' uma cena onde o jogo se desencadeara'. O World contem
// elementos como: heroi, mumias, cenario e hud.
// AVISO (podem excluir): criei o esqueleto da classe para poder
// testar a compilacao do jogo. :P
class World : public framework::Scene {
  public:
    World(sprite::Hero *hero);
    virtual ~World();

    void Update(float delta_t);

    sprite::Wall* AddWall(framework::Vector2D);
    void AddWorldObject(sprite::WorldObject*, framework::Vector2D pos);
    void AddFloor(sprite::Floor*);
    void AddFloor(framework::Vector2D pos);
    void AddMummy(framework::Vector2D pos);
	void AddBigMummy(framework::Vector2D pos);
	void AddRangedMummy(framework::Vector2D pos);
	void AddPharaoh(framework::Vector2D pos);
    void AddHero(framework::Vector2D pos);
    void AddDoor(framework::Vector2D pos);
    void AddPotion(framework::Vector2D pos);
    void AddLifePotion(framework::Vector2D pos);
    void AddManaPotion(framework::Vector2D pos);
    void AddSightPotion(framework::Vector2D pos);

    int CountRemainingEnemies();
    int max_enemies() { return max_enemies_; }
    void DecreaseEnemyCount() { remaining_enemies_--; }
    void FinishLevel(utils::LevelManager::LevelState state) {
        level_state_ = state;
    }
    void End();

    // Funcao auxiliar que transforma VETORES de coordenadas de tela para de mundo
    static Vector2D FromScreenLinearCoordinates(Vector2D screen_coords);
    
    // Funcao auxiliar que transforma VETORES de coordenadas de mundo para de tela
    static Vector2D FromWorldLinearCoordinates(Vector2D world_coords);

    // Funcao que transforma PONTOS de coordenadas de mundo para de tela
    static Vector2D FromWorldCoordinates(Vector2D screen_coords);

    // Funcao que transforma PONTOS de coordenadas de tela para de mundo
    static Vector2D FromScreenCoordinates(Vector2D screen_coords);

    static Image* CreateFogTransparency(float radius);

    vector<sprite::Mummy *> Mummies();
    
	//getters
	sprite::Hero * hero() { return hero_; }
    utils::Fog* fog() {	return fog_; }
    int level_width() {	return level_width_; }
    int level_height() { return level_height_; }
    vector<string> level_matrix() {	return level_matrix_; }
	
	//setters
	void set_level_width(int width) { level_width_ = width; }
    void set_level_height(int height) {	level_height_ = height; }
    void set_level_matrix(vector<string> matrix) { level_matrix_ = matrix; }
    void set_hero(sprite::Hero *hero) { hero_ = hero; }

  protected:
    sprite::Hero *hero_;
    std::list<sprite::WorldObject*> world_objects_, collisionless_objects;
    framework::Layer *world_layer_;
    framework::Sprite *hero_fog_;
    utils::Hud *hud_;
    utils::Fog *fog_;
    int level_width_, level_height_;
    vector<string> level_matrix_;
    int	remaining_enemies_, max_enemies_;
	utils::ImageFactory* image_factory_;
    sprite::MummyBuilder mummy_builder_;
    sprite::ItemBuilder potion_builder_;

	Vector2D ActualOffset();
	void VerifyCheats();
	bool VerifyQuit(); 
	void HandleCollisions();
    void RemoveInactiveObjects();
    void RemoveAll();

  private:
    utils::LevelManager::LevelState level_state_;
    bool good_end_, player_exit_;

};  // class World

}  // namespace scene

#endif  // HORUSEYE_GAME_SCENES_WORLD_H_
