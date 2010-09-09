//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/utils/levelloader.h
// Definicao da classe LevelLoader.
//

#ifndef HORUSEYE_GAME_UTILS_LEVELLOADER_H_
#define HORUSEYE_GAME_UTILS_LEVELLOADER_H_
#include<vector>
#include <string>
#include "../scenes/world.h"

namespace sprite{
	class Wall;
}

namespace utils {
class LevelLoader {
  public:
    LevelLoader(scene::World * world) : world_(world) {
		image_factory_ = new utils::ImageFactory();
	}
    virtual ~LevelLoader() {}

    void Load(std::string);
    void LoadMatrix(std::string);

  protected:

	ImageFactory* image_factory_;
    scene::World * world_;
	bool InRange (int i,int j);
	bool IsWall(int i, int j);
	void TokenToWorldObject(char token, int i, int j, Vector2D position, std::vector<std::vector<sprite::Wall* > > &wall_matrix);
	void InitializeWallTypes(std::vector<std::vector<sprite::Wall *> > wall_matrix);

};

}

#endif // HORUSEYE_GAME_UTILS_LEVELLOADER_H_
