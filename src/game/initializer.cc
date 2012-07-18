#include <sys/stat.h>
#include <cerrno>
#include <ugdk/base/engine.h>
#include <ugdk/base/resourcemanager.h>
#include <ugdk/math/vector2D.h>
#include <ugdk/graphic/spritesheet/fixedspritesheet.h>
#include <ugdk/graphic/spritesheet/flexiblespritesheet.h>
#include "initializer.h"

#define level_manager() utils::LevelManager::reference()
#define engine() ugdk::Engine::reference()

static void CreateFixedSpritesheet(const char* path, int frame_width, int frame_height, const ugdk::Vector2D& hotspot) {
    ugdk::graphic::FixedSpritesheetData sheet_data(path);

    sheet_data.FillWithFramesize(frame_width, frame_height, hotspot);

    ugdk::graphic::FixedSpritesheet* sheet = new ugdk::graphic::FixedSpritesheet(sheet_data);
    engine()->resource_manager()->spritesheet_container().Insert(path, sheet);
}

void LoadSpritesheets() {    
    CreateFixedSpritesheet("images/blue_fire_ball.png"     ,  32,  32, ugdk::Vector2D( 16.0,  16.0)); // Magic Missile
    CreateFixedSpritesheet("images/yellow_fire_ball.png"   ,  32,  32, ugdk::Vector2D( 16.0,  16.0)); // Blue Gem
    CreateFixedSpritesheet("images/shield.png"             , 128, 128, ugdk::Vector2D( 64.0, 110.0)); // Shield
    CreateFixedSpritesheet("images/fireball_0.png"         ,  64,  64, ugdk::Vector2D( 32.0,  32.0)); // Fireball
    CreateFixedSpritesheet("images/green_fire_ball.png"    ,  32,  32, ugdk::Vector2D( 16.0,  16.0)); // MummyProjectileImage
    CreateFixedSpritesheet("images/explosion.png"          , 256, 128, ugdk::Vector2D(128.0,  64.0)); // Fireball Explosion
    CreateFixedSpritesheet("images/quake.png"              , 256, 128, ugdk::Vector2D(128.0,  64.0)); // Quake Explosion
    CreateFixedSpritesheet("images/stairs3.png"            , 153, 109, ugdk::Vector2D( 76.5,  63.5)); // Door stairs
    CreateFixedSpritesheet("images/stoneblock3.png"        , 106, 157, ugdk::Vector2D( 53.0,  130.0)); // Wall 
    CreateFixedSpritesheet("images/door.png"               , 106, 157, ugdk::Vector2D( 53.0,  130.0)); // Entry
    CreateFixedSpritesheet("images/lightning_bolt.png"     ,  64,  64, ugdk::Vector2D( 32.0,   32.0)); // Lightning Bolt
    CreateFixedSpritesheet("images/yellow_fire_ball.png"   ,  32,  32, ugdk::Vector2D( 16.0,   16.0)); // Light
	CreateFixedSpritesheet("images/tile_switch.png"        , 106,  54, ugdk::Vector2D( 53.0,   27.0)); // Door stairs
}

bool VerifyFolderExists(const std::string& path) {
    struct stat st;
    int s;
    if(path[path.size() - 1] == '/')
        s = stat(path.substr(0, path.size() - 1).c_str(), &st);
    else
        s = stat(path.c_str(), &st);
    return !(s < 0 && errno == ENOENT);
}
