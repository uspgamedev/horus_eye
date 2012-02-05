#include <string>
#include <sys/stat.h>
#include <cerrno>
#include <ugdk/base/engine.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/audio/audiomanager.h>
#include <ugdk/graphic/textmanager.h>
#include <ugdk/math/vector2D.h>
#include <pyramidworks/collision/collisionmanager.h>

#include <ugdk/graphic/spritesheet/fixedspritesheet.h>
#include <ugdk/graphic/spritesheet/flexiblespritesheet.h>

#include <ugdk/script/scriptmanager.h>
#include <ugdk/script/langwrapper.h>
#include <ugdk/script/virtualobj.h>
#include <ugdk/script/lua/luawrapper.h>
#include <ugdk/script/lua/header.h>

extern "C" {
extern int luaopen_ugdk_math(lua_State* L);
}

#include "utils/constants.h"
#include "utils/levelmanager.h"
#include "utils/settings.h"
#include "utils/textloader.h"

using namespace utils;

#define LOADSPRITESHEET(VIDEO, PATH) VIDEO->AddSpritesheet(PATH, new ugdk::graphic::FlexibleSpritesheet(VIDEO->LoadTexture(PATH)));

utils::LevelManager* level_manager() {
    return utils::LevelManager::reference();
}

utils::TextLoader* text_loader() {
    return utils::TextLoader::reference();
}

ugdk::Engine* engine() {
    return ugdk::Engine::reference();
}

static void CreateFixedSpritesheet(const char* path, int frame_width, int frame_height, const ugdk::Vector2D& hotspot) {
    ugdk::graphic::FixedSpritesheetData sheet_data(path);

    sheet_data.FillWithFramesize(frame_width, frame_height, hotspot);

    ugdk::graphic::FixedSpritesheet* sheet = new ugdk::graphic::FixedSpritesheet(sheet_data);
    engine()->video_manager()->AddSpritesheet(path, sheet);
}
static void CreateFlexibleSpritesheet(const char* path, float frame_width, float frame_height, const ugdk::Vector2D& hotspot) {
    ugdk::graphic::VideoManager* videomanager = engine()->video_manager();

    ugdk::graphic::FlexibleSpritesheet *sheet = new ugdk::graphic::FlexibleSpritesheet(videomanager->LoadTexture(path));
    sheet->set_frame_size(ugdk::Vector2D(frame_width, frame_height));
    sheet->set_hotspot(hotspot);

    videomanager->AddSpritesheet(path, sheet);
}

void StartGame() {
    Settings* settings = Settings::reference();

    // Checks if restarting game. Avoids changing resolution during
    // startup, to avoid taking longer uselessly.
    if(level_manager()->RestartGameQueued())
        engine()->video_manager()->ChangeResolution(settings->resolution_vector(), settings->fullscreen());

    engine()->video_manager()->SetLightSystem(true);

    ugdk::graphic::VideoManager* videomanager = engine()->video_manager();

    CreateFixedSpritesheet(   "images/sprite-sheet_MOD3.png"  , 110, 110, ugdk::Vector2D(55.0f, 102.0f)); // Kha
    CreateFlexibleSpritesheet("images/mummy_blue_120x140.png" , 120, 140, ugdk::Vector2D(60.0f, 120.0f)); // Regular Mummy
    CreateFlexibleSpritesheet("images/mummy_green_120x140.png", 120, 140, ugdk::Vector2D(60.0f, 120.0f)); // Giant Mummy
    CreateFlexibleSpritesheet("images/pharaoh_120x140.png"    , 120, 140, ugdk::Vector2D(60.0f, 120.0f)); // Pharaoh
    CreateFlexibleSpritesheet("images/mummy_red_120x140.png"  , 120, 140, ugdk::Vector2D(60.0f, 120.0f)); // Shooting Mummy
    
    CreateFlexibleSpritesheet("images/blue_fire_ball.png"     ,  32,  32, ugdk::Vector2D(16.0f,  16.0f)); // Magic Missile
    LOADSPRITESHEET(videomanager, "images/fireball_0.png");
    LOADSPRITESHEET(videomanager, "images/green_fire_ball.png");
    LOADSPRITESHEET(videomanager, "images/explosion.png");
    LOADSPRITESHEET(videomanager, "images/quake.png");
    LOADSPRITESHEET(videomanager, "images/life_potion2.png");
    LOADSPRITESHEET(videomanager, "images/mana_potion.png");
    LOADSPRITESHEET(videomanager, "images/sight_potion.png");
    LOADSPRITESHEET(videomanager, "images/stairs3.png");
    LOADSPRITESHEET(videomanager, "images/ground2_106x54.png");
    LOADSPRITESHEET(videomanager, "images/stoneblock3.png");
    LOADSPRITESHEET(videomanager, "images/door.png");
    LOADSPRITESHEET(videomanager, "images/lightning_bolt.png");
    LOADSPRITESHEET(videomanager, "images/yellow_fire_ball.png");
    LOADSPRITESHEET(videomanager, "images/tile_switch.png");
    CreateFixedSpritesheet("images/eye.png", 128, 96, ugdk::Vector2D());

    text_loader()->Initialize(settings->language_file());
    level_manager()->Initialize();

    pyramidworks::collision::CollisionManager* colmanager 
        = pyramidworks::collision::CollisionManager::reference();
    colmanager->Generate("WorldObject");

    colmanager->Generate("Creature", "WorldObject");
    colmanager->Generate("Hero", "Creature");
    colmanager->Generate("Mummy", "Creature");

    colmanager->Generate("Wall", "WorldObject");
    colmanager->Generate("Block", "Wall");
    colmanager->Generate("Door", "Wall");

    colmanager->Generate("Item", "WorldObject");
    colmanager->Generate("Projectile", "WorldObject");
    colmanager->Generate("Button", "WorldObject");
    colmanager->Generate("Explosion", "WorldObject");

    using ugdk::script::VirtualObj;

    VirtualObj obj = SCRIPT_MANAGER()->LoadModule("main");

    ugdk::Vector2D* vec = obj["v"].value<ugdk::Vector2D>();
    printf("Result: ( %f , %f )\n", vec->x, vec->y);


}

void InitScripts() {

    using ugdk::script::LangWrapper;
    using ugdk::script::lua::LuaWrapper;

    LuaWrapper* lua_wrapper = new LuaWrapper();
    lua_wrapper->RegisterModule("ugdk.math", luaopen_ugdk_math);

    SCRIPT_MANAGER()->Register("Lua", lua_wrapper);

}

int main(int argc, char *argv[]) {
    Settings* settings = Settings::reference();

    std::string rootpath = Constants::DATA_LOCATION;
    struct stat st;
    // Removing the trailing slash.
    int s = stat(rootpath.substr(0, rootpath.size() - 1).c_str(), &st);
    if(s < 0 && errno == ENOENT)
        rootpath = "./";

#ifndef ISMAC
    const std::string iconpath = "images/eye.bmp";
#else
    // On Mac OS X, the icon should be handled with a *.icns file inside the app
    const std::string iconpath = "";
#endif
    InitScripts();
    engine()->Initialize("Horus Eye", settings->resolution_vector(), settings->fullscreen(), rootpath, iconpath);
    do {
        // Initializes game data
        StartGame();

        // Transfers control to the framework.
        engine()->Run();

        // Releases data persistant between levels.
        level_manager()->Finish();

        // Clears all CollisionClasses.
        pyramidworks::collision::CollisionManager::Delete();

        // Releases all loaded textures, to avoid issues when changing resolution.
        engine()->video_manager()->Release();
        engine()->text_manager()->Release();

    } while(level_manager()->RestartGameQueued());
    engine()->Release();
    return 0;
}
