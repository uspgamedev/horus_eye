#ifndef HORUSEYE_EDITOR_LAYERS_MAPTILESLAYER_H_
#define HORUSEYE_EDITOR_LAYERS_MAPTILESLAYER_H_

#include "ugdk/layer.h"
#include "../mapeditor.h"

namespace editor {

class MapTilesLayer : public MapEditor::MapLayer {
  public:
    MapTilesLayer(MapEditor::MapMatrix *matrix, MapEditor* editor);
    virtual ~MapTilesLayer();
    void Update(float delta_t);
    void Render();
    void CenterAt(ugdk::Vector2D& center);
    MapObject* Select(ugdk::Vector2D& pos);
    ugdk::Vector2D ModifyMovement(ugdk::Vector2D& movement);
};

}

#endif /* MAPTILESLAYER_H_ */
