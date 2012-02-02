#include "maptileslayer.h"
#include "editor/mapobject.h"
#include <ugdk/base/engine.h>
#include <ugdk/graphic/videomanager.h>

using namespace ugdk;

namespace editor {

MapTilesLayer::MapTilesLayer(MapEditor* editor) : MapEditor::MapLayer(editor) {}

MapTilesLayer::~MapTilesLayer() {}

void MapTilesLayer::LoadMapMatrix(MapEditor::MapMatrix *matrix) {
	if(node_ != NULL) delete node_;
	node_ = new Node;
	matrix_ = matrix;
	int height = matrix_->size(), width = (*matrix_)[0].size();
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; j++) {
			node_->AddChild((*matrix_)[i][j]->tile_node());
        }
    }
}

void MapTilesLayer::CenterAt(ugdk::Vector2D& center) {
	if (!editor_->map_loaded()) return;

    Vector2D screen_size = VIDEO_MANAGER()->video_size();
	node()->modifier()->set_offset(screen_size * 0.5f - center * MapObject::TileSize);
}

MapObject* MapTilesLayer::Select(ugdk::Vector2D& pos) {
	if (!editor_->map_loaded()) return NULL;

    Vector2D absolute = pos + node()->modifier()->offset();
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
