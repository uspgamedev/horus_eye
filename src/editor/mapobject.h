#ifndef HORUSEYE_EDITOR_MAPOBJECT_H_
#define HORUSEYE_EDITOR_MAPOBJECT_H_

#define WALL                     'W'
#define DOOR                     'D'
#define ENTRY                    'E'
#define MUMMY                    'm'
#define STANDING_MUMMY           'M'
#define RANGED_MUMMY             'r'
#define STANDING_RANGED_MUMMY    'R'
#define BIG_MUMMY                'b'
#define STANDING_BIG_MUMMY       'B'
#define PHARAOH                  'p'
#define STANDING_PHARAOH         'P'
#define HERO                     'H'
#define FLOOR                    'X'
#define EMPTY                    'O'
#define POTIONL                  'L'
#define POTIONM                  'N'
#define POTIONS                  'S'

//#include "../framework/texture.h"
#include "../framework/image.h"
#include "../framework/sprite.h"

namespace editor {

class MapObject : public framework::Sprite {
  public:
    const static float TileSize;

    MapObject(int i, int j, char type, int level_width, int level_height);

    void Update(float delta_t);

    virtual ~MapObject();
    virtual void Render2D(framework::Vector2D offset, float scale = 1.0f);

    void Select(bool on);

	int x() { return this->x_; }
	int y() { return this->y_; }
	char type() { return this->type_; } 

  private:
    int x_, y_;
    char type_;
    framework::Image *tile_image_, *sprite_image_;
};

}

#endif /* HORUSEYE_EDITOR_MAPOBJECT_H_ */
