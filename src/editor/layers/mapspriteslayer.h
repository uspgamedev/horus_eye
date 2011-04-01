#ifndef HORUSEYE_EDITOR_LAYERS_MAPSPRITESLAYER_H_
#define HORUSEYE_EDITOR_LAYERS_MAPSPRITESLAYER_H_

#include "../../framework/layer.h"
#include "../mapeditor.h"

namespace editor {

class MapSpritesLayer : public MapEditor::MapLayer {
  public:
    MapSpritesLayer(MapEditor* editor);
    virtual ~MapSpritesLayer();
	void LoadMapMatrix(MapEditor::MapMatrix *matrix);
    void Update(float delta_t);
    void CenterAt(framework::Vector2D& center);
    MapObject* Select(framework::Vector2D& pos);
    framework::Vector2D ModifyMovement(framework::Vector2D& movement);
};

}

#endif /* HORUSEYE_EDITOR_LAYERS_MAPSPRITESLAYER_H_ */
