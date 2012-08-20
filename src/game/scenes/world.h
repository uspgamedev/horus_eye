#ifndef HORUSEYE_GAME_SCENES_WORLD_H_
#define HORUSEYE_GAME_SCENES_WORLD_H_

#include <list>
#include <vector>
#include <ugdk/portable/tr1.h>
#include FROM_TR1(unordered_map)
#include <ugdk/action/scene.h>
#include <ugdk/math/vector2D.h>
#include <pyramidworks/collision.h>

#include "game/components.h"
#include "game/scenes/gamelayer.h"
#include "game/utils/levelmanager.h"
#include "game/utils/tilefwd.h"
#include "game/resources/resource.h"

namespace utils {
class Hud;
}
namespace sprite {
class WorldObject;
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
    World(sprite::WorldObject *hero);
    virtual ~World();

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
    sprite::WorldObject * hero() const { return hero_; }
    sprite::WorldObject * hero_world_object() const;
    int level_width() const { return level_width_; }
    int level_height() const { return level_height_; }
    utils::GameMap& level_matrix() { return level_matrix_; }
    resource::Resource<int>& num_button_not_pressed() { return num_button_not_pressed_; }
    pyramidworks::collision::CollisionManager* collision_manager() { return collision_manager_; }
    
    //setters
    void set_level_width(int width) { level_width_ = width; }
    void set_level_height(int height) {	level_height_ = height; }
    void set_level_matrix(utils::GameMap matrix) { level_matrix_ = matrix; }
    void set_hero(sprite::WorldObject *hero) { hero_ = hero; }

    sprite::WorldObject* WorldObjectByTag (const std::string& tag);
    void RemoveTag(const std::string& tag);

    ugdk::graphic::Node* layer_node(GameLayer layer) { 
        return layers_[layer];
    }

  protected:
    sprite::WorldObject *hero_;

    utils::Hud *hud_;
    int level_width_, level_height_;
    utils::GameMap level_matrix_;
    int	remaining_enemies_, max_enemies_;

  private:
    typedef std::tr1::unordered_map<std::string, sprite::WorldObject*> TagTable;

    utils::LevelManager::LevelState level_state_;
    bool konami_used_, lights_on_;
    resource::Resource<int> num_button_not_pressed_;
    pyramidworks::collision::CollisionManager* collision_manager_;
    ugdk::graphic::Node *layers_[2];
    TagTable tagged_;

};  // class World

}  // namespace scene

#endif  // HORUSEYE_GAME_SCENES_WORLD_H_
