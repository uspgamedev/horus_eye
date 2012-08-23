#ifndef HORUSEYE_GAME_UTILS_LEVELLOADER_H_
#define HORUSEYE_GAME_UTILS_LEVELLOADER_H_

#include <vector>
#include <string>
#include <ugdk/portable/tr1.h>
#include FROM_TR1(functional)
#include <ugdk/script/virtualobj.h>

#include "game/builders/itembuilder.h"
#include "game/builders/mummybuilder.h"
#include "game/builders/entitybuilder.h"
#include "game/builders/doodadbuilder.h"

namespace ugdk {
    class Vector2D;
}
namespace scene {
    class World;
}
namespace component {
    class Wall;
}
namespace sprite {
    class WorldObject;
}
using ugdk::Vector2D;

namespace utils {

typedef std::tr1::function<sprite::WorldObject* (const std::vector<std::string>&)> WorldObjectFactoryMethod;
typedef std::vector<std::string> ArgumentList;

class LevelLoader;
class LevelLoader {
  public:
    LevelLoader(scene::World * world) : world_(world) {}
    virtual ~LevelLoader() {}

    void Load(const std::string&);

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
    ugdk::script::VirtualObj::Vector objects_;
    ugdk::script::VirtualObj setup_;
};

}

#endif // HORUSEYE_GAME_UTILS_LEVELLOADER_H_
