#ifndef HORUSEYE_GAME_SCENES_WORLD_H_
#define HORUSEYE_GAME_SCENES_WORLD_H_

#include <list>
#include <vector>
#include "../../framework/scene.h"
#include "../../framework/vector2D.h"
#include "../utils/levelmanager.h"
#include "../utils/tile.h"

namespace framework {
class Music;
}
namespace utils {
class Hud;
class ImageFactory;
}
namespace sprite {
class Hero;
class WorldObject;
}

using framework::Vector2D;
using framework::Image;
using std::vector;

namespace scene {

#define WORLD() (utils::LevelManager::reference()->get_current_level() )

// Classe World
// O World e' uma cena onde o jogo se desencadeara'. O World contem
// elementos como: heroi, mumias, cenario e hud.
class World : public framework::Scene {
  public:
    World(sprite::Hero *hero);
    virtual ~World();

	void Start();

    void Update(float delta_t);

    void AddWorldObject(sprite::WorldObject*, framework::Vector2D pos);
    void AddHero(framework::Vector2D pos);

    void AddNewWorldObjects();

    int CountRemainingEnemies();
	void IncreaseNumberOfEnemies();
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

	static const Vector2D ConvertLightRadius(float radius);

    vector<sprite::Mummy *> Mummies();
    
    //getters
    sprite::Hero * hero() { return hero_; }
    int level_width() {	return level_width_; }
    int level_height() { return level_height_; }
    utils::GameMap& level_matrix() {	return level_matrix_; }
    utils::ImageFactory* image_factory() { return image_factory_; }
	
    //setters
	void set_music(std::string &music);
    void set_level_width(int width) { level_width_ = width; }
    void set_level_height(int height) {	level_height_ = height; }
    void set_level_matrix(utils::GameMap matrix) { level_matrix_ = matrix; }
    void set_hero(sprite::Hero *hero) { hero_ = hero; }

  protected:
    sprite::Hero *hero_;
    std::list<sprite::WorldObject*> world_objects_, collisionless_objects, new_world_objects;
    framework::Layer *world_layer_;
    framework::Sprite *hero_fog_;
	framework::Music *music_;
    utils::Hud *hud_;
    int level_width_, level_height_;
    utils::GameMap level_matrix_;
    int	remaining_enemies_, max_enemies_;
    utils::ImageFactory* image_factory_;

    Vector2D ActualOffset();
    bool verifyCollision(sprite::WorldObject *obj1, sprite::WorldObject *obj2);
    void VerifyCheats(float delta_t);
    bool VerifyPause();
    void HandleCollisions();
    void RemoveInactiveObjects();
    void RemoveAll();
    void UpdateVisibility();

  private:
    utils::LevelManager::LevelState level_state_;
    bool good_end_, player_exit_;
	bool konami_used_;

};  // class World

}  // namespace scene

#endif  // HORUSEYE_GAME_SCENES_WORLD_H_
