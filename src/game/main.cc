#include <string>
#include <sys/stat.h>
#include <cerrno>
#include <ugdk/base/engine.h>
#include <ugdk/base/resourcemanager.h>
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
#include <ugdk/script/python/pythonwrapper.h>

extern "C" {
extern int luaopen_ugdk_math(lua_State* L);
extern void init_ugdk_math(void);
}

#include "utils/constants.h"
#include "utils/levelmanager.h"
#include "utils/settings.h"
#include "utils/textloader.h"

using namespace utils;

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
    engine()->resource_manager()->spritesheet_container().Insert(path, sheet);
}

static void CreateFlexibleSpritesheet(const char* path, double frame_width, double frame_height, const ugdk::Vector2D& hotspot) {
    ugdk::base::ResourceManager* resources = engine()->resource_manager();
    
    ugdk::graphic::Texture* tex = resources->texture_container().Load(path);

    ugdk::graphic::FlexibleSpritesheet *sheet = new ugdk::graphic::FlexibleSpritesheet(tex);
    sheet->set_frame_size(ugdk::Vector2D(frame_width, frame_height));
    sheet->set_hotspot(hotspot);

    resources->spritesheet_container().Insert(path, sheet);
}

static void CreateSimpleFlexibleSpritesheet(const char* path) {
    ugdk::base::ResourceManager* resources = engine()->resource_manager();

    ugdk::graphic::Texture* tex = resources->texture_container().Load(path);
    
    ugdk::graphic::FlexibleSpritesheet *sheet = new ugdk::graphic::FlexibleSpritesheet(tex);
    resources->spritesheet_container().Insert(path, sheet);
}

void StartGame() {
    Settings* settings = Settings::reference();

    // Checks if restarting game. Avoids changing resolution during
    // startup, to avoid taking longer uselessly.
    if(level_manager()->RestartGameQueued())
        engine()->video_manager()->ChangeResolution(settings->resolution_vector(), settings->fullscreen());

    engine()->video_manager()->SetLightSystem(true);

    CreateFixedSpritesheet(   "images/eye.png"                , 128,  96, ugdk::Vector2D());
    CreateFixedSpritesheet(   "images/sprite-sheet_MOD3.png"  , 110, 110, ugdk::Vector2D(55.0, 102.0)); // Kha
    CreateFlexibleSpritesheet("images/mummy_blue_120x140.png" , 120, 140, ugdk::Vector2D(60.0, 120.0)); // Regular Mummy
    CreateFlexibleSpritesheet("images/mummy_green_120x140.png", 120, 140, ugdk::Vector2D(60.0, 120.0)); // Giant Mummy
    CreateFlexibleSpritesheet("images/pharaoh_120x140.png"    , 120, 140, ugdk::Vector2D(60.0, 120.0)); // Pharaoh
    CreateFlexibleSpritesheet("images/mummy_red_120x140.png"  , 120, 140, ugdk::Vector2D(60.0, 120.0)); // Shooting Mummy
    
    CreateFlexibleSpritesheet("images/blue_fire_ball.png"     ,  32,  32, ugdk::Vector2D(16.0,  16.0)); // Magic Missile
    CreateSimpleFlexibleSpritesheet("images/fireball_0.png");
    CreateSimpleFlexibleSpritesheet("images/green_fire_ball.png");
    CreateSimpleFlexibleSpritesheet("images/explosion.png");
    CreateSimpleFlexibleSpritesheet("images/quake.png");
    CreateSimpleFlexibleSpritesheet("images/life_potion2.png");
    CreateSimpleFlexibleSpritesheet("images/mana_potion.png");
    CreateSimpleFlexibleSpritesheet("images/sight_potion.png");
    CreateSimpleFlexibleSpritesheet("images/stairs3.png");
    CreateSimpleFlexibleSpritesheet("images/ground2_106x54.png");
    CreateSimpleFlexibleSpritesheet("images/stoneblock3.png");
    CreateSimpleFlexibleSpritesheet("images/door.png");
    CreateSimpleFlexibleSpritesheet("images/lightning_bolt.png");
    CreateSimpleFlexibleSpritesheet("images/yellow_fire_ball.png");
    CreateSimpleFlexibleSpritesheet("images/tile_switch.png");

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

    // testando lua
    {
        VirtualObj obj = SCRIPT_MANAGER()->LoadModule("main");

        puts("Checking result...");
        ugdk::Vector2D* vec = obj["v"].value<ugdk::Vector2D>();
        if (!vec) puts("FAILED.");
        else {
            printf("Result 1: ( %f , %f )\n", vec->x, vec->y);
        }
        const char* text = obj["str"].stringvalue();
        if (!text) puts("FAILED TEXT.");
        else printf("Result 2: %s\n", text);
        bool boolean = obj["bool"].booleanvalue();
        if (!boolean) puts("FAILED BOOLEAN.");
        else printf("Result 3: %d\n", boolean);

        obj["ls"](std::vector<VirtualObj>(1,obj));
    }

    //testando python
    printf("Python test starting...\n");
    VirtualObj wassup = SCRIPT_MANAGER()->LoadModule("wassup");
    printf("MARK got wassup\n");
    VirtualObj pyVecx = wassup["vecx"];
    printf("MARK got python vecx\n");
    ugdk::Vector2D* vecx = pyVecx.value<ugdk::Vector2D>();
    printf("MARK converted vecx to C++ Vector2D object\n");
    printf("X: ( %f , %f )\n", vecx->x, vecx->y);

    wassup["supimpa"](std::vector<VirtualObj>(1,pyVecx));
    
    printf("Python test finished. \n");
}

void InitScripts() {

    using ugdk::script::LangWrapper;
    using ugdk::script::lua::LuaWrapper;
    using ugdk::script::python::PythonWrapper;

    //inicializando lua
    LuaWrapper* lua_wrapper = new LuaWrapper();
    lua_wrapper->RegisterModule("ugdk.math", luaopen_ugdk_math);
    SCRIPT_MANAGER()->Register("Lua", lua_wrapper);

    //inicializando python
    PythonWrapper* py_wrapper = new PythonWrapper();
    printf("Registered Python Module: %d\n", (int)py_wrapper->RegisterModule("_ugdk_math", init_ugdk_math)  );
    SCRIPT_MANAGER()->Register("Python", py_wrapper);
}

int main(int argc, char *argv[]) {
    Settings* settings = Settings::reference();

	ugdk::Engine::Configuration engine_config;
	engine_config.window_title = "Horus Eye";
	engine_config.window_size  = settings->resolution_vector();
	engine_config.fullscreen   = settings->fullscreen();

	engine_config.base_path = Constants::DATA_LOCATION;
    struct stat st;
    // Removing the trailing slash.
    int s = stat(engine_config.base_path.substr(0, engine_config.base_path.size() - 1).c_str(), &st);
    if(s < 0 && errno == ENOENT)
        engine_config.base_path = "./";

#ifndef ISMAC
	engine_config.window_icon = "images/eye.bmp";
#else
    // On Mac OS X, the icon should be handled with a *.icns file inside the app
    engine_config.window_icon = "";
#endif

    InitScripts();
    engine()->Initialize(engine_config);
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
