#ifndef HORUSEYE_EDITOR_LAYERS_MAPTILESLAYER_H_
#define HORUSEYE_EDITOR_LAYERS_MAPTILESLAYER_H_

#include "../../framework/layer.h"
#include "../mapeditor.h"

namespace editor {

class MapTilesLayer : public MapEditor::MapLayer {
  public:
    MapTilesLayer(MapEditor::MapMatrix *matrix, MapEditor* editor);
    virtual ~MapTilesLayer();
    void Update(float delta_t);
    void Render();
    void CenterAt(framework::Vector2D& center);
    MapObject* Select(framework::Vector2D& pos);
    framework::Vector2D ModifyMovement(framework::Vector2D& movement);
};

}

#endif /* MAPTILESLAYER_H_ */
