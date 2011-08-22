#ifndef HORUSEYE_EDITOR_MAPEDITOR_H_
#define HORUSEYE_EDITOR_MAPEDITOR_H_

#include <cstdlib>
#include <string>
#include <vector>
#include "ugdk/scene.h"
#include "ugdk/layer.h"

namespace editor {

class MapObject;

class MapEditor : public ugdk::Scene {
  public:
    MapEditor();
    virtual ~MapEditor();
    virtual void Update(float delta_t);
	void LoadMap(std::string& file_name);
	void SaveMap();
	bool map_loaded() { return this->map_loaded_; }
	std::vector<std::string>& map_list() { return this->map_list_; }

    typedef std::vector< std::vector<MapObject*> > MapMatrix;

    class MapLayer : public ugdk::Layer {
      public:
        virtual ~MapLayer() {}
		virtual void LoadMapMatrix(MapEditor::MapMatrix *matrix) {}
        virtual void set_scale(float scale) { scale_ = scale; }
        virtual void CenterAt(ugdk::Vector2D& center) { set_offset(center); }

        virtual MapObject* Select(ugdk::Vector2D& pos) { return NULL; }
        virtual ugdk::Vector2D ModifyMovement(ugdk::Vector2D& movement) { return movement; }

      protected:
        MapLayer(MapMatrix *matrix, MapEditor* editor) : ugdk::Layer(), matrix_(matrix), editor_(editor), scale_(1.0f) {}
        MapEditor::MapMatrix *matrix_;
		MapEditor* editor_;
        float scale_;
    };

  private:
	std::string music_;
    int width_, height_;
    int scale_level_;
    ugdk::Vector2D click_start_position_, last_mouse_position_, offset_;
    bool drag_click_;
	bool map_loaded_;
	MapObject* selected_object_;
    MapMatrix map_matrix_;
    MapLayer *main_layer_, *tiles_layer_, *sprites_layer_;
	ugdk::Layer *fps_layer_;
	std::string map_filename_;
	std::vector<std::string> map_list_;

	void processKeyEditCommands();
};

}

#endif /* HORUSEYE_EDITOR_MAPEDITOR_H_ */
