#ifndef HORUSEYE_GAME_UTILS_TILE_H_
#define HORUSEYE_GAME_UTILS_TILE_H_

#include <vector>
#include <cstddef>  // Usado para definir NULL

namespace ugdk {
class Vector2D;
}

namespace utils {

#define EMPTY                    ' '
#define FLOOR                    '.'
#define WALL                     '#'
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
#define HERO                     '@'
#define POTIONL                  'L'
#define POTIONM                  'N'
#define POTIONS                  'S'
#define BLOCK                    'K'
#define BUTTON                   'U'

class Tile;

typedef std::vector<Tile*> TileRow;
typedef std::vector<TileRow> GameMap;
typedef struct TilePos{

    TilePos(int i_ = 0, int j_ = 0) : i(i_), j(j_) {}

    int i, j;

} TilePos;

class Tile {
  public:
    Tile (int i = -1, int j = -1, char object = EMPTY)
        : pos_(i,j), object_(object), visible_(false) {}
    virtual ~Tile () {}

    bool valid() const { return pos_.i >= 0 && pos_.j >= 0; }
    bool visible() const { return visible_; }
    bool checked() const { return checked_; }
    char object() const { return object_; }
    int i() const { return pos_.i; }
    int j() const { return pos_.j; }
    const TilePos& pos() const { return pos_; }

    void set_visible(bool value) { visible_ = value; }

    void Check() { checked_ = true; }
    void Uncheck() { checked_ = false; }

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
                
        case   UP_RIGHT: aux =   Up(map); return aux ? aux->Right(map) : NULL; break;
        case    UP_LEFT: aux =   Up(map); return aux ? aux->Left(map)  : NULL; break;
        case DOWN_RIGHT: aux = Down(map); return aux ? aux->Right(map) : NULL; break;
        case  DOWN_LEFT: aux = Down(map); return aux ? aux->Left(map)  : NULL; break;

        default:break;
        }
        return NULL;
    }

    static void CleanVisibility(GameMap &map);
    static TilePos ToTilePos(ugdk::Vector2D pos);
    static ugdk::Vector2D FromTilePos(const TilePos pos);
    static Tile* GetFromMapPosition(GameMap &map, TilePos pos) {
        return SafeGetFromMap(map, pos);
    }
    static Tile* GetFromWorldPosition(GameMap &map, ugdk::Vector2D pos);

  private:

    TilePos pos_;
    char    object_;
    bool    visible_, checked_;

    static Tile* SafeGetFromMap(GameMap &map, int i, int j) {
        if (i < 0 || i >= (int)map.size())
            return NULL;
        if (j < 0 || j >= (int)map[i].size())
            return NULL;
        return map[i][j];
    }

    static Tile* SafeGetFromMap(GameMap &map, TilePos pos) {
        return SafeGetFromMap(map, pos.i, pos.j);
    }

};

}

#endif /* TILE_H_ */
