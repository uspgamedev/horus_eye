
#ifndef HORUSEYE_GAME_UTILS_TILE_H_
#define HORUSEYE_GAME_UTILS_TILE_H_

#include <vector>

namespace utils {

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

class Tile;

typedef std::vector<Tile> TileRow;
typedef std::vector<TileRow> GameMap;

class Tile {
  public:
    Tile (int i = 0, int j = 0, char object = EMPTY)
        : i_(i), j_(j), object_(object), visible_(false) {}
    virtual ~Tile () {}

    bool visible() const { return visible_; }
    char object() const { return object_; }
    int  i() const { return i_; }
    int  j() const { return j_; }

    static void CleanVisibility(GameMap& map);

  private:
    int  i_, j_;
    char object_;
    bool visible_;

};

}

#endif /* TILE_H_ */
