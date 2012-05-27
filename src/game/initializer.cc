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

static void CreateFlexibleSpritesheet(const char* path, double frame_width, double frame_height, const ugdk::Vector2D& hotspot) {
    ugdk::base::ResourceManager* resources = engine()->resource_manager();
    
    ugdk::graphic::Texture* tex = resources->texture_container().Load(path, path);

    ugdk::graphic::FlexibleSpritesheet *sheet = new ugdk::graphic::FlexibleSpritesheet(tex);
    sheet->set_frame_size(ugdk::Vector2D(frame_width, frame_height));
    sheet->set_hotspot(hotspot);

    resources->spritesheet_container().Insert(path, sheet);
}

static void CreateSimpleFlexibleSpritesheet(const char* path) {
    ugdk::base::ResourceManager* resources = engine()->resource_manager();

    ugdk::graphic::Texture* tex = resources->texture_container().Load(path, path);
    
    ugdk::graphic::FlexibleSpritesheet *sheet = new ugdk::graphic::FlexibleSpritesheet(tex);
    resources->spritesheet_container().Insert(path, sheet);
}

void LoadSpritesheets() {
    CreateFixedSpritesheet(   "images/eye.png"                , 128,  96, ugdk::Vector2D());
    CreateFixedSpritesheet(   "images/sprite-sheet_MOD3.png"  , 110, 110, ugdk::Vector2D( 55.0, 102.0)); // Kha
    CreateFlexibleSpritesheet("images/mummy_blue_120x140.png" , 120, 140, ugdk::Vector2D( 60.0, 120.0)); // Regular Mummy
    CreateFlexibleSpritesheet("images/mummy_green_120x140.png", 120, 140, ugdk::Vector2D( 60.0, 120.0)); // Giant Mummy
    CreateFlexibleSpritesheet("images/pharaoh_120x140.png"    , 120, 140, ugdk::Vector2D( 60.0, 120.0)); // Pharaoh
    CreateFlexibleSpritesheet("images/mummy_red_120x140.png"  , 120, 140, ugdk::Vector2D( 60.0, 120.0)); // Shooting Mummy
    
    CreateFlexibleSpritesheet("images/blue_fire_ball.png"     ,  32,  32, ugdk::Vector2D( 16.0,  16.0)); // Magic Missile
    CreateFlexibleSpritesheet("images/yellow_fire_ball.png"   ,  32,  32, ugdk::Vector2D( 16.0,  16.0)); // Blue Gem
    CreateFlexibleSpritesheet("images/shield.png"             , 128, 128, ugdk::Vector2D( 64.0, 110.0)); // Shield
    CreateSimpleFlexibleSpritesheet("images/fireball_0.png");
    CreateSimpleFlexibleSpritesheet("images/green_fire_ball.png");
    CreateFlexibleSpritesheet("images/explosion.png"          , 256, 128, ugdk::Vector2D(128.0,  64.0)); // Fireball Explosion
    CreateFlexibleSpritesheet("images/quake.png"              , 256, 128, ugdk::Vector2D(128.0,  64.0)); // Quake Explosion
    //CreateSimpleFlexibleSpritesheet("images/life_potion2.png");
    //CreateSimpleFlexibleSpritesheet("images/mana_potion.png");
    //CreateSimpleFlexibleSpritesheet("images/sight_potion.png");
    CreateSimpleFlexibleSpritesheet("images/stairs3.png");
    CreateSimpleFlexibleSpritesheet("images/ground2_106x54.png");
    CreateSimpleFlexibleSpritesheet("images/stoneblock3.png");
    CreateSimpleFlexibleSpritesheet("images/door.png");
    CreateSimpleFlexibleSpritesheet("images/lightning_bolt.png");
    CreateSimpleFlexibleSpritesheet("images/yellow_fire_ball.png");
    CreateSimpleFlexibleSpritesheet("images/tile_switch.png");
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
