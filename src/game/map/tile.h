#ifndef HORUSEYE_GAME_MAP_TILE_H_
#define HORUSEYE_GAME_MAP_TILE_H_

#include <ugdk/graphic.h>
#include <ugdk/math.h>
#include <ugdk/math/vector2D.h>
#include "game/map.h"

#include <vector>

namespace map {

#define EMPTY                    ' '
#define FLOOR                    '.'
#define WALL                     '#'
#define INVISIBLEWALL            '%'
#define BURNTWALL                '&'
#define DOOR                     'D'
#define ENTRY                    'E'
#define STANDING_MUMMY           'M'
#define STANDING_PAPER_MUMMY     'z'
#define STANDING_RANGED_MUMMY    'R'
#define STANDING_BIG_MUMMY       'B'
#define STANDING_PHARAOH         'P'
#define HERO                     '@'
#define POTIONL                  'L'
#define POTIONM                  'N'
#define SCRIPT                   '!'

typedef struct TilePos {
    TilePos(int i_ = 0, int j_ = 0) : i(i_), j(j_) {}
    
    int i, j;
} TilePos;

class Tile {
  public:
    Tile (int i = -1, int j = -1, char object = EMPTY);
    virtual ~Tile () {}

    bool valid() const { return pos_.i >= 0 && pos_.j >= 0; }
    char object() const { return object_; }
    void set_object(char object) { object_ = object; }
    int i() const { return pos_.i; }
    int j() const { return pos_.j; }
    const TilePos& pos() const { return pos_; }
    bool has_floor() const { return object_ != EMPTY; }

    Tile* Up(GameMap &map) const {
        return SafeGetFromMap(map, pos_.i-1, pos_.j);
    }
    Tile* Down(GameMap &map) const {
        return SafeGetFromMap(map, pos_.i+1, pos_.j);
    }
    Tile* Right(GameMap &map) const {
        return SafeGetFromMap(map, pos_.i, pos_.j+1);
    }
    Tile* Left(GameMap &map) const {
        return SafeGetFromMap(map, pos_.i, pos_.j-1);
    }

    enum TileDir {
        BEGIN = 0, UP = 0, DOWN, RIGHT, LEFT,
        UP_RIGHT, UP_LEFT, DOWN_RIGHT, DOWN_LEFT, END
    };

    Tile* Next(GameMap &map, TileDir dir) {
        Tile* aux;
        switch (dir) {
        case         UP: return    Up(map); break;
        case       DOWN: return  Down(map); break;
        case      RIGHT: return Right(map); break;
        case       LEFT: return  Left(map); break;
                
        case   UP_RIGHT: aux =   Up(map); return aux ? aux->Right(map) : nullptr; break;
        case    UP_LEFT: aux =   Up(map); return aux ? aux->Left(map)  : nullptr; break;
        case DOWN_RIGHT: aux = Down(map); return aux ? aux->Right(map) : nullptr; break;
        case  DOWN_LEFT: aux = Down(map); return aux ? aux->Left(map)  : nullptr; break;

        default:break;
        }
        return nullptr;
    }

    static TilePos ToTilePos(ugdk::math::Vector2D pos);
    static ugdk::math::Vector2D FromTilePos(const TilePos pos);
    static Tile* GetFromMapPosition(GameMap &map, TilePos pos) {
        return SafeGetFromMap(map, pos);
    }
    static Tile* GetFromWorldPosition(GameMap &map, ugdk::math::Vector2D pos);

  private:

    TilePos pos_;
    char    object_;


    static Tile* SafeGetFromMap(GameMap &map, int i, int j) {
        if (i < 0 || i >= (int)map.size())
            return nullptr;
        if (j < 0 || j >= (int)map[i].size())
            return nullptr;
        return map[i][j];
    }

    static Tile* SafeGetFromMap(GameMap &map, TilePos pos) {
        return SafeGetFromMap(map, pos.i, pos.j);
    }

};

} // namespace map

#endif /* HORUSEYE_GAME_MAP_TILE_H_ */
