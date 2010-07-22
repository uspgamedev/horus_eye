//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/utils/editor/worldeditor.cc
// Implementacao da classe WorldEditor.
//

#include "../../../framework/engine.h"
#include "../../../framework/inputmanager.h"
#include "../../../framework/timeaccumulator.h"
#include "worldeditor.h"
#include "editorobject.h"
#include <cmath>
#include <iostream>
#include "../constants.h"

#define SQRT_3 1.7320508075688772935274463415059

namespace utils {

using namespace framework;
using namespace sprite;

WorldEditor::WorldEditor() : Scene(), world_layer_(new framework::Layer()) {
    AddLayer(world_layer_);
    camera_offset_ = Vector2D(0,0)-VIDEO_MANAGER()->video_size()*0.5;
    mouse_pressed_ = esc_pressed_ = false;
    selection_object_ = NULL;
    esc_interval_ = new TimeAccumulator(0);
}

WorldEditor::~WorldEditor() {
    RemoveAll();
    for(int j = 0; j < level_height_; ++j) {
        delete level_matrix_[j];
        delete objects_matrix_[j];
        delete floor_matrix_[j];
    }
    delete level_matrix_;
    delete objects_matrix_;
    delete floor_matrix_;
    delete esc_interval_;
}

void WorldEditor::GetInput(float delta_t) {
    InputManager *input_ = Engine::reference()->input_manager();
    Vector2D movement;
    Vector2D mouse_position = input_->GetMousePosition();
    if(input_->MouseDown(M_BUTTON_LEFT)) {
        if(mouse_pressed_)
            camera_offset_ = camera_offset_ - ((mouse_position - old_mouse_position_) * 80 * delta_t);
        else
            click_position_ = mouse_position;
        mouse_pressed_ = true;
        old_mouse_position_ = input_->GetMousePosition();
    } else {
        if(mouse_pressed_ && (click_position_ - mouse_position).length() < 3.0f) {
            Vector2D worldpos = FromScreenLinearCoordinates((camera_offset_ + mouse_position) * (1.0/40.0));
            int x = (int)(worldpos.x),
                y = (int)(level_height_ - worldpos.y - 1);

            if(0 <= x && x < level_width_ && 0 <= y && y < level_height_) {
                if(objects_matrix_[y][x] != NULL)
                    selection_object_->MarkSelected(false);
                selection_object_ = NULL;

                if(objects_matrix_[y][x] != NULL)
                    selection_object_ = objects_matrix_[y][x];
                else if(floor_matrix_[y][x] != NULL)
                    selection_object_ = floor_matrix_[y][x];

                if(selection_object_ != NULL)
                    selection_object_->MarkSelected(true);
                selection_x_ = x;
                selection_y_ = y;
            }
        }
        mouse_pressed_ = false;
        if(input_->KeyDown(K_DOWN)) movement.y += 1;
        if(input_->KeyDown(K_UP)) movement.y -= 1;
        if(input_->KeyDown(K_LEFT)) movement.x -= 1;
        if(input_->KeyDown(K_RIGHT)) movement.x += 1;
        camera_offset_ = camera_offset_ + movement * 250 * delta_t;
    }
}

void WorldEditor::Update(float delta_t) {
    InputManager *input_ = Engine::reference()->input_manager();
    if(input_->KeyDown(K_ESCAPE)) {
        if(!esc_interval_->Expired()) {
            RemoveAll();
            Engine::reference()->quit();
            return;
        }
        esc_pressed_ = true;
    } else if(esc_pressed_) {
        esc_pressed_ = false;
        esc_interval_->Restart(1250);
    }
    Scene::Update(delta_t);
    GetInput(delta_t);
    world_layer_->set_offset(camera_offset_);

    if(0 <= selection_x_ && selection_x_ < level_width_ && 0 <= selection_y_ && selection_y_ < level_height_) {
        Vector2D pos(selection_x_, level_height_ - selection_y_ - 1);
        if(input_->KeyDown(K_m)) {
            ; // Muda pra mummy
        } else if(input_->KeyDown(K_h)) {
            ; // Muda pra hero
        } else if(input_->KeyDown(K_w)) {
            // Muda pra wall
            if(level_matrix_[selection_y_][selection_x_] != WALL) {
                world_layer_->RemoveSprite(selection_object_);
                delete selection_object_;

                selection_object_ = objects_matrix_[selection_y_][selection_x_] =
                        AddObjectFromToken(pos, WALL);
                RemoveFloor(selection_x_,selection_y_);
                level_matrix_[selection_y_][selection_x_] = WALL;
            }
        } else if(input_->KeyDown(K_f)) {
            // Muda pra floor
            if(level_matrix_[selection_y_][selection_x_] != FLOOR) {
                world_layer_->RemoveSprite(selection_object_);
                delete selection_object_;

                selection_object_ = objects_matrix_[selection_y_][selection_x_] =
                        AddObjectFromToken(pos, FLOOR);
                RemoveFloor(selection_x_, selection_y_);
                level_matrix_[selection_y_][selection_x_] = FLOOR;
            }
        } else if(input_->KeyDown(K_d)) {
            ; // Muda pra door
        } else if(input_->KeyDown(K_DELETE)) {
            // Muda pra vazio
            RemoveObject(selection_x_, selection_y_);
            RemoveFloor(selection_x_, selection_y_);
            level_matrix_[selection_y_][selection_x_] = EMPTY;
            selection_object_ = NULL;
        }

        if(selection_object_ != NULL)
            selection_object_->MarkSelected(true);
    }
}

void WorldEditor::InitializeLevel(int width, int height) {
    level_width_ = width;
    level_height_ = height;

    level_matrix_ = new ObjectToken*[level_height_];
    objects_matrix_ = new EditorObject**[level_height_];
    floor_matrix_ = new EditorObject**[level_height_];
    for (int i = 0; i < level_height_; i++) {
        level_matrix_[i] = new ObjectToken[level_width_];
        objects_matrix_[i] = new EditorObject*[level_width_];
        floor_matrix_[i] = new EditorObject*[level_width_];
        for (int j = 0; j < level_width_; ++j) {
            level_matrix_[i][j] = EMPTY;
            objects_matrix_[i][j] = NULL;
            floor_matrix_[i][j] = NULL;
        }
    }

    camera_offset_ = camera_offset_ + FromWorldCoordinates(Vector2D(level_width_/2.0, level_height_/2.0 - 1));
}

void WorldEditor::AddWorldObject(sprite::WorldObject* new_object) {
    world_layer_->AddSprite(new_object);
}

EditorObject* WorldEditor::AddObjectFromToken(Vector2D &world_coords, ObjectToken token) {
    if(token == EMPTY)
        return NULL;
    EditorObject* obj = new EditorObject(token);
    obj->set_world_position(world_coords);
    AddWorldObject(obj);
    return obj;
}

void WorldEditor::LoadToken(int x, int y, ObjectToken token) {
    Vector2D pos(x, level_height_ - y - 1);
    level_matrix_[y][x] = token;
    if(token != FLOOR)
        objects_matrix_[y][x] = AddObjectFromToken(pos, token);
    if(token == HERO || token == MUMMY || token == FLOOR)
        floor_matrix_[y][x] = AddObjectFromToken(pos, FLOOR);
}

void WorldEditor::RemoveObject(int x, int y) {
    if(objects_matrix_[y][x] != NULL) {
        world_layer_->RemoveSprite(objects_matrix_[y][x]);
        delete objects_matrix_[y][x];
        objects_matrix_[y][x] = NULL;
    }
}
void WorldEditor::RemoveFloor(int x, int y) {
    if(floor_matrix_[y][x] != NULL) {
        world_layer_->RemoveSprite(floor_matrix_[y][x]);
        delete floor_matrix_[y][x];
        floor_matrix_[y][x] = NULL;
    }

}

void WorldEditor::RemoveAll() {
    for(int j = 0; j < level_height_; ++j)
        for(int i = 0; i < level_width_; ++i) {
            RemoveObject(i, j);
            RemoveFloor(i, j);
        }
}


Vector2D WorldEditor::FromScreenLinearCoordinates(Vector2D screen_coords) {
    Vector2D tx(1/SQRT_3, -1/SQRT_3);
    Vector2D ty(-1, -1);
    return (tx * screen_coords.x)   +  (ty * screen_coords.y);
}

Vector2D WorldEditor::FromWorldLinearCoordinates(Vector2D world_coords) {
    Vector2D tx(SQRT_3/2, -.5);
    Vector2D ty(-SQRT_3/2, -.5);
    return (tx * world_coords.x)   +  (ty * world_coords.y);
}

Vector2D WorldEditor::FromWorldCoordinates(Vector2D world_coords) {
    Vector2D transformed = FromWorldLinearCoordinates(world_coords);
    return (transformed * 40);
}

} // namespace scene
