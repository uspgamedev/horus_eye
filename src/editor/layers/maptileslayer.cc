#include "maptileslayer.h"
#include "../mapobject.h"
#include "../../framework/engine.h"
#include "../../framework/videomanager.h"

using namespace framework;

namespace editor {

MapTilesLayer::MapTilesLayer(MapEditor::MapMatrix *matrix) : MapEditor::MapLayer(matrix) {}

MapTilesLayer::~MapTilesLayer() {
}

void MapTilesLayer::Update(float delta_t) {
    if(!this->IsVisible()) return;
    Layer::Update(delta_t);
}

void MapTilesLayer::Render() {
    if(!this->IsVisible()) return;
    int height = matrix_->size(), width = (*matrix_)[0].size();
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; j++) {
            (*matrix_)[i][j]->Render2D(this->offset(), scale_);
        }
    }
}

void MapTilesLayer::CenterAt(framework::Vector2D& center) {
    Vector2D screen_size = VIDEO_MANAGER()->video_size();
    set_offset(center * (MapObject::TileSize * scale_) - screen_size * 0.5f);
}

MapObject* MapTilesLayer::Select(framework::Vector2D& pos) {
    Vector2D absolute = pos + offset();
    int x = static_cast<int>(absolute.x / (MapObject::TileSize * scale_));
    int y = static_cast<int>(absolute.y / (MapObject::TileSize * scale_));
    if(x < 0 || y < 0)
        return NULL;
    if(x < static_cast<int>((*matrix_)[0].size()) && y < static_cast<int>(matrix_->size())) {
        return (*matrix_)[y][x];
    }
    return NULL;
}

Vector2D MapTilesLayer::ModifyMovement(Vector2D& movement) {
    return movement * (1.0f / (MapObject::TileSize * scale_));
}

}
