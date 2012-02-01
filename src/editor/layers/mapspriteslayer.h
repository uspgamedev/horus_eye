#ifndef HORUSEYE_EDITOR_LAYERS_MAPSPRITESLAYER_H_
#define HORUSEYE_EDITOR_LAYERS_MAPSPRITESLAYER_H_

#include "editor/mapeditor.h"

namespace editor {

class MapSpritesLayer : public MapEditor::MapLayer {
  public:
    MapSpritesLayer(MapEditor* editor);
    virtual ~MapSpritesLayer();

	void LoadMapMatrix(MapEditor::MapMatrix *matrix);

    void CenterAt(ugdk::Vector2D& center);
    MapObject* Select(ugdk::Vector2D& pos);
    ugdk::Vector2D ModifyMovement(ugdk::Vector2D& movement);
};

}

#endif /* HORUSEYE_EDITOR_LAYERS_MAPSPRITESLAYER_H_ */
