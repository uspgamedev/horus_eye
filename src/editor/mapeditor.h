#ifndef HORUSEYE_EDITOR_MAPEDITOR_H_
#define HORUSEYE_EDITOR_MAPEDITOR_H_

#include <string>
#include <vector>
#include "../framework/scene.h"
#include "../framework/layer.h"

namespace editor {

class MapObject;

class MapEditor : public framework::Scene {
  public:
    MapEditor(std::string& file);
    virtual ~MapEditor();
    virtual void Update(float delta_t);

    typedef std::vector< std::vector<MapObject*> > MapMatrix;

    class MapLayer : public framework::Layer {
      public:
        virtual ~MapLayer() {}
        virtual void set_scale(float scale) { scale_ = scale; }
        virtual void CenterAt(framework::Vector2D& center) { set_offset(center); }

        virtual MapObject* Select(framework::Vector2D& pos) { return NULL; }
        virtual framework::Vector2D ModifyMovement(framework::Vector2D& movement) { return movement; }

      protected:
        MapLayer(MapMatrix *matrix) : framework::Layer(), matrix_(matrix), scale_(1.0f) {}
        MapEditor::MapMatrix *matrix_;
        float scale_;
    };

  private:
    int width_, height_;
    int scale_level_;
    framework::Vector2D click_start_position_, last_mouse_position_, offset_;
    bool drag_click_;
    MapObject* selected_object_;
    MapMatrix map_matrix_;
    MapLayer *main_layer_, *tiles_layer_, *sprites_layer_;
	std::string map_filename_;

	void processKeyEditCommands();
	void saveMap();
};

}

#endif /* HORUSEYE_EDITOR_MAPEDITOR_H_ */
