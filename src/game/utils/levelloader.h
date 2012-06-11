#ifndef HORUSEYE_GAME_UTILS_LEVELLOADER_H_
#define HORUSEYE_GAME_UTILS_LEVELLOADER_H_

#include <vector>
#include <string>
#include <functional>
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

class LevelLoader;
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
    void TokenToWorldObject(char token, int i, int j, const Vector2D& position);
    void InitializeWallTypes();

    std::vector<std::vector<std::string> > arguments_;
    std::map<char, std::tr1::function<sprite::WorldObject* (LevelLoader*, const std::string&)> > token_function_;
    std::vector<std::vector<component::Wall* > > wall_matrix_;

    builder::DoodadBuilder doodad_builder_;
    builder::MummyBuilder mummy_builder_;
    builder::ItemBuilder potion_builder_;
    builder::EntityBuilder entity_builder_;

    sprite::WorldObject* GenerateBlock(const std::string&);
    sprite::WorldObject* GenerateStandingMummy(const std::string&);
    sprite::WorldObject* GenerateMummy(const std::string&);
    sprite::WorldObject* GenerateStandingBigMummy(const std::string&);
    sprite::WorldObject* GenerateBigMummy(const std::string&);
    sprite::WorldObject* GenerateStandingRangedMummy(const std::string&);
    sprite::WorldObject* GenerateRangedMummy(const std::string&);
    sprite::WorldObject* GenerateStandingPharaoh(const std::string&);
    sprite::WorldObject* GeneratePharaoh(const std::string&);
    sprite::WorldObject* GenerateLifePotion(const std::string&);
    sprite::WorldObject* GenerateManaPotion(const std::string&);
    sprite::WorldObject* GenerateSightPotion(const std::string&);
    sprite::WorldObject* GenerateBlueGem(const std::string&);
    sprite::WorldObject* GenerateButton(const std::string&);

};

}

#endif // HORUSEYE_GAME_UTILS_LEVELLOADER_H_
