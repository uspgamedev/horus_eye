#ifndef HORUSEYE_EDITOR_MAPOBJECT_H_
#define HORUSEYE_EDITOR_MAPOBJECT_H_

#include <ugdk/action/entity.h>

namespace ugdk {
    namespace graphic {
        class SolidRectangle;
        class Spritesheet;
        class Node;
    }
}

namespace editor {

class MapObject : public ugdk::Entity {
  public:
    const static float TileSize;

    MapObject(int i, int j, char type, int level_width, int level_height);

    void Update(float delta_t);

	ugdk::graphic::Node* tile_node() { return tile_node_; }
	ugdk::graphic::Node* isometric_node() { return isometric_node_; }

    virtual ~MapObject();

    void Select(bool on);

	int x() { return this->x_; }
	int y() { return this->y_; }
	char type() { return this->type_; } 
	void set_is_in_fill(bool is) { this->is_in_fill_ = is;}
	bool is_in_fill() { return this->is_in_fill_; }

  private:
    int x_, y_;
    char type_;
	bool is_in_fill_;
	ugdk::graphic::SolidRectangle *tile_image_;
    ugdk::graphic::Spritesheet *sprite_image_;

	ugdk::graphic::Node *tile_node_, *isometric_node_;

};

}

#endif /* HORUSEYE_EDITOR_MAPOBJECT_H_ */
