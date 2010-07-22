//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/utils/editor/worldeditor.h
// Definicao da classe WorldEditor.
//

#ifndef HORUSEYE_GAME_UTILS_EDITOR_WORLDEDITOR_H_
#define HORUSEYE_GAME_UTILS_EDITOR_WORLDEDITOR_H_

#include <list>
#include "../../../framework/scene.h"
#include "../../../framework/vector2D.h"
#include "../../sprites/worldobject.h"

namespace framework {
class TimeAccumulator;
}
namespace utils {

#define WORLD() ((World *)Engine::reference()->CurrentScene())

class EditorObject;

class WorldEditor : public framework::Scene {
  public:
    WorldEditor();
    virtual ~WorldEditor();

    void Update(float delta_t);

    void AddWorldObject(sprite::WorldObject*);

    enum ObjectToken { WALL, DOOR, MUMMY, HERO, FLOOR, EMPTY };
    void LoadToken(int x, int y, ObjectToken token);

    // Funcao auxiliar que transforma VETORES de coordenadas de tela para de mundo
    static framework::Vector2D FromScreenLinearCoordinates(framework::Vector2D screen_coords);
    
    // Funcao auxiliar que transforma VETORES de coordenadas de mundo para de tela
    static framework::Vector2D FromWorldLinearCoordinates(framework::Vector2D world_coords);

    // Funcao que transforma PONTOS de coordenadas de mundo para de tela
    static framework::Vector2D FromWorldCoordinates(framework::Vector2D screen_coords);

    EditorObject* AddObjectFromToken(framework::Vector2D& world_coords, ObjectToken token);

    int level_width() { return level_width_; }
    int level_height() { return level_height_; }
    ObjectToken** level_matrix() { return level_matrix_; }

    void InitializeLevel(int width, int height);

  protected:
    std::list<sprite::WorldObject*> world_objects_;
    framework::Layer *world_layer_;
    framework::TimeAccumulator *esc_interval_;
    int level_width_, level_height_,
        selection_x_, selection_y_;
    ObjectToken** level_matrix_;
    EditorObject*** objects_matrix_, ***floor_matrix_;
    EditorObject* selection_object_;
    framework::Vector2D camera_offset_, old_mouse_position_, click_position_;
    bool mouse_pressed_, esc_pressed_;

    void RemoveObject(int x, int y);
    void RemoveFloor(int x, int y);
    void RemoveAll();
    void GetInput(float delta_t);

};  // class WorldEditor

}  // namespace utils

#endif  // HORUSEYE_GAME_UTILS_EDITOR_WORLDEDITOR_H_
