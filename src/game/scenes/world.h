#ifndef HORUSEYE_GAME_SCENES_WORLD_H_
#define HORUSEYE_GAME_SCENES_WORLD_H_

#include <list>
#include <vector>
#include <ugdk/action/scene.h>
#include <pyramidworks/collision.h>

#include "game/utils/levelmanager.h"
#include "game/utils/tilefwd.h"
#include "game/resources/resource.h"

namespace utils {
class Hud;
class ImageFactory;
}
namespace sprite {
class Hero;
class Mummy;
class WorldObject;
}
namespace ugdk {
    class Vector2D;
}
using ugdk::Vector2D;
using std::vector;

namespace scene {

#define WORLD() (utils::LevelManager::reference()->get_current_level() )

// Classe World
// O World e' uma cena onde o jogo se desencadeara'. O World contem
// elementos como: heroi, mumias, cenario e hud.
class World : public ugdk::action::Scene {
  typedef ugdk::action::Scene super;
  public:
    World(sprite::Hero *hero, utils::ImageFactory *factory);
    virtual ~World();

    void Update(double delta_t);

    void AddWorldObject(sprite::WorldObject*, const ugdk::Vector2D& pos);
    void AddHero(const ugdk::Vector2D& pos);

    int CountRemainingEnemies();
    void IncreaseNumberOfEnemies();
    int max_enemies() { return max_enemies_; }
    void DecreaseEnemyCount() { remaining_enemies_--; }
    void FinishLevel(utils::LevelManager::LevelState state) {
        level_state_ = state;
    }
    void SetupCollisionManager();
    void End();

    // Funcao auxiliar que transforma VETORES de coordenadas de tela para de mundo
    static Vector2D FromScreenLinearCoordinates(const Vector2D& screen_coords);
    
    // Funcao auxiliar que transforma VETORES de coordenadas de mundo para de tela
    static Vector2D FromWorldLinearCoordinates(const Vector2D& world_coords);

    // Funcao que transforma PONTOS de coordenadas de mundo para de tela
    static Vector2D FromWorldCoordinates(const Vector2D& screen_coords);

    // Funcao que transforma PONTOS de coordenadas de tela para de mundo
    static Vector2D FromScreenCoordinates(const Vector2D& screen_coords);

    static const Vector2D ConvertLightRadius(double radius);

    //getters
    sprite::Hero * hero() const { return hero_; }
    sprite::WorldObject * hero_world_object() const;
    int level_width() const { return level_width_; }
    int level_height() const { return level_height_; }
    utils::GameMap& level_matrix() { return level_matrix_; }
    utils::ImageFactory* image_factory() const { return image_factory_; }
    resource::Resource<int>& num_button_not_pressed() { return num_button_not_pressed_; }
    pyramidworks::collision::CollisionManager* collision_manager() { return collision_manager_; }
    
    //setters
    void set_level_width(int width) { level_width_ = width; }
    void set_level_height(int height) {	level_height_ = height; }
    void set_level_matrix(utils::GameMap matrix) { level_matrix_ = matrix; }
    void set_hero(sprite::Hero *hero) { hero_ = hero; }

  protected:
    sprite::Hero *hero_;

    utils::Hud *hud_;
    int level_width_, level_height_;
    utils::GameMap level_matrix_;
    int	remaining_enemies_, max_enemies_;
    utils::ImageFactory* image_factory_;

    Vector2D ActualOffset();
    bool verifyCollision(sprite::WorldObject *obj1, sprite::WorldObject *obj2);
    bool VerifyPause();
    void UpdateVisibility();

  private:
    utils::LevelManager::LevelState level_state_;
    bool konami_used_, lights_on_;
    resource::Resource<int> num_button_not_pressed_;
    pyramidworks::collision::CollisionManager* collision_manager_;

};  // class World

}  // namespace scene

#endif  // HORUSEYE_GAME_SCENES_WORLD_H_
