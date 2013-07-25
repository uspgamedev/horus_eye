#ifndef HORUSEYE_GAME_UTILS_LEVELLOADER_H_
#define HORUSEYE_GAME_UTILS_LEVELLOADER_H_

#include <vector>
#include <string>
#include <functional>
#include <ugdk/script/virtualobj.h>
#include <ugdk/math.h>

#include "game/builders/itembuilder.h"
#include "game/builders/mummybuilder.h"
#include "game/builders/doodadbuilder.h"

namespace scene {
    class World;
}
namespace component {
    class Wall;
}
namespace sprite {
    class WorldObject;
}
using ugdk::math::Vector2D;

namespace utils {

typedef std::function<sprite::WorldObject* (const std::vector<std::string>&)> WorldObjectFactoryMethod;
typedef std::vector<std::string> ArgumentList;

class LevelLoader;
class LevelLoader {
  public:
    LevelLoader(scene::World * world) : world_(world) {}
    virtual ~LevelLoader() {}

    void Load(const std::string& campaign, const std::string& level_name);

  private:
    scene::World * world_;
    bool InRange (int i,int j);
    bool IsWall(int i, int j);
    void TokenToWorldObject(char token, int i, int j, const Vector2D& position);
    void InitializeWallTypes();
    bool LoadMatrix(const std::string&);

    std::vector<std::vector<ArgumentList> > arguments_;
    std::vector<std::vector<std::string> > tags_;
    std::map<char, WorldObjectFactoryMethod> token_function_;
    std::vector<std::vector<component::Wall* > > wall_matrix_;
    ugdk::script::VirtualObj level_data_;
};

}

#endif // HORUSEYE_GAME_UTILS_LEVELLOADER_H_
