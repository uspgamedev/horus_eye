#ifndef HORUSEYE_EDITOR_MAPEDITOR_H_
#define HORUSEYE_EDITOR_MAPEDITOR_H_

#include <cstdlib>
#include <string>
#include <vector>
#include <ugdk/action/scene.h>
#include <ugdk/graphic/node.h>

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

    class MapLayer {
      public:
        virtual ~MapLayer() {}
		virtual void LoadMapMatrix(MapEditor::MapMatrix *matrix) = 0;
        virtual void set_scale(float scale) { scale_ = scale; }
		virtual void CenterAt(ugdk::Vector2D& center) { node_->modifier()->set_offset(center); }
		ugdk::graphic::Node* node() { return node_; }

        virtual MapObject* Select(ugdk::Vector2D& pos) { return NULL; }
        virtual ugdk::Vector2D ModifyMovement(ugdk::Vector2D& movement) { return movement; }

      protected:
        MapLayer(MapEditor* editor) : matrix_(NULL), editor_(editor), scale_(1.0f), node_(new ugdk::graphic::Node) {}
        MapEditor::MapMatrix *matrix_;
		MapEditor* editor_;
        float scale_;
		ugdk::graphic::Node* node_;
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
	//ugdk::Layer *fps_layer_; // TODO
	std::string map_filename_;
	std::vector<std::string> map_list_;

	void processKeyEditCommands();
    void End();
};

}

#endif /* HORUSEYE_EDITOR_MAPEDITOR_H_ */
