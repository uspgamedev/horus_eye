#ifndef HORUSEYE_EDITOR_MAPOBJECT_H_
#define HORUSEYE_EDITOR_MAPOBJECT_H_

#include <ugdk/graphic/image.h>
#include <ugdk/graphic/flexiblespritesheet.h>
#include <ugdk/action/sprite.h>

#include "game/utils/tile.h"

namespace editor {

class MapObject : public ugdk::Sprite {
  public:
    const static float TileSize;

    MapObject(int i, int j, char type, int level_width, int level_height);

    void Update(float delta_t);

    virtual ~MapObject();
    virtual void Render2D(ugdk::Vector2D offset, float scale = 1.0f);

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
    ugdk::Image *tile_image_;
    ugdk::FlexibleSpritesheet *sprite_image_;
};

}

#endif /* HORUSEYE_EDITOR_MAPOBJECT_H_ */
