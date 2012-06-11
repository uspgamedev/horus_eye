#ifndef HORUSEYE_GAME_UTILS_LEVELLOADER_H_
#define HORUSEYE_GAME_UTILS_LEVELLOADER_H_

#include <vector>
#include <string>

namespace ugdk {
    class Vector2D;
}
namespace scene {
    class World;
}
namespace component {
    class Wall;
}
using ugdk::Vector2D;

namespace utils {
class LevelLoader {
  public:
    LevelLoader(scene::World * world) : world_(world) {}
    virtual ~LevelLoader() {}

    void Load(std::string);
    bool LoadMatrix(std::string);

  protected:
    scene::World * world_;
    bool InRange (int i,int j);
    bool IsWall(int i, int j);
    void TokenToWorldObject(char token, int i, int j, const Vector2D& position, std::vector<std::vector<component::Wall* > > &wall_matrix);
    void InitializeWallTypes(std::vector<std::vector<component::Wall*> > wall_matrix);

    std::vector<std::vector<std::string> > arguments_;

};

}

#endif // HORUSEYE_GAME_UTILS_LEVELLOADER_H_
