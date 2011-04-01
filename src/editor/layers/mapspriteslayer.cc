#include "mapspriteslayer.h"
#include "../mapobject.h"
#include "../../game/scenes/world.h"
#include "../../framework/engine.h"
#include "../../framework/videomanager.h"


using namespace framework;

namespace editor {

MapSpritesLayer::MapSpritesLayer(MapEditor* editor) : MapEditor::MapLayer(NULL, editor)  {
}

MapSpritesLayer::~MapSpritesLayer() {
}

void MapSpritesLayer::LoadMapMatrix(MapEditor::MapMatrix *matrix) {
	matrix_ = matrix;
	int height = matrix_->size(), width = (*matrix_)[0].size();
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; j++) {
            this->AddSprite((*matrix_)[i][j]);
        }
    }
}

void MapSpritesLayer::Update(float delta_t) {
    if(!this->IsVisible()) return;
    Layer::Update(delta_t);
}

void MapSpritesLayer::CenterAt(framework::Vector2D& center) {
	if (!editor_->map_loaded()) return;

    Vector2D screen_size = VIDEO_MANAGER()->video_size();
    Vector2D correct_center = Vector2D(center.x, matrix_->size() - center.y);
    set_offset(scene::World::FromWorldCoordinates(correct_center) - screen_size * 0.5f);
}

MapObject* MapSpritesLayer::Select(framework::Vector2D& pos) {
	if (!editor_->map_loaded()) return NULL;

    Vector2D    global_screen_coords = pos + offset(),
                transformed = scene::World::FromScreenLinearCoordinates(global_screen_coords);
    Vector2D absolute = (transformed * (1.0f/60.373835392f));
    int x = static_cast<int>(absolute.x + 0.5f);
    int y = matrix_->size() - static_cast<int>(absolute.y + 0.5f) - 1;
    if(x < 0 || y < 0)
        return NULL;
    if(x < static_cast<int>((*matrix_)[0].size()) && y < static_cast<int>(matrix_->size())) {
        return (*matrix_)[y][x];
    }
    return NULL;
}

Vector2D MapSpritesLayer::ModifyMovement(Vector2D& movement) {
    Vector2D resp = scene::World::FromScreenLinearCoordinates(movement) * (1.5f/60.373835392f);
    return Vector2D(resp.x, -resp.y);
}

}
