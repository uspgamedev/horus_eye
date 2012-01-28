#include <string>
#include <sys/stat.h>
#include <ugdk/base/engine.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/audio/audiomanager.h>
#include <ugdk/graphic/textmanager.h>
#include <ugdk/math/vector2D.h>
#include <pyramidworks/collision/collisionmanager.h>

#include <ugdk/graphic/spritesheet/fixedspritesheet.h>
#include <ugdk/graphic/spritesheet/flexiblespritesheet.h>


#include "utils/constants.h"
#include "utils/levelmanager.h"
#include "utils/settings.h"
#include "utils/textloader.h"

using namespace utils;

#define LOADSPRITESHEET(VIDEO, PATH) VIDEO->AddSpritesheet(PATH, new ugdk::FlexibleSpritesheet(VIDEO->LoadTexture(PATH)));

utils::LevelManager* level_manager() {
    return utils::LevelManager::reference();
}

utils::TextLoader* text_loader() {
    return utils::TextLoader::reference();
}

ugdk::Engine* engine() {
    return ugdk::Engine::reference();
}

void StartGame() {
    Settings* settings = Settings::reference();

    // Checks if restarting game. Avoids changing resolution during
    // startup, to avoid taking longer uselessly.
    if(level_manager()->RestartGameQueued())
        engine()->video_manager()->ChangeResolution(settings->resolution_vector(), settings->fullscreen());

    engine()->video_manager()->SetLightSystem(true);

    ugdk::VideoManager* videomanager = engine()->video_manager();
    {
        ugdk::FixedSpritesheetData hero_sheet_data("data/images/sprite-sheet_MOD3.png");

        hero_sheet_data.FillWithFramesize(110, 110, ugdk::Vector2D(55.0f, 102.0f));

        ugdk::FixedSpritesheet* hero_sheet = new ugdk::FixedSpritesheet(hero_sheet_data);
        videomanager->AddSpritesheet("data/images/sprite-sheet_MOD3.png", hero_sheet);
    }


    LOADSPRITESHEET(videomanager, "data/images/mummy_blue_120x140.png");
    LOADSPRITESHEET(videomanager, "data/images/mummy_green_120x140.png");
    LOADSPRITESHEET(videomanager, "data/images/pharaoh_120x140.png");
    LOADSPRITESHEET(videomanager, "data/images/mummy_red_120x140.png");
    LOADSPRITESHEET(videomanager, "data/images/blue_fire_ball.png");
    LOADSPRITESHEET(videomanager, "data/images/fireball_0.png");
    LOADSPRITESHEET(videomanager, "data/images/green_fire_ball.png");
    LOADSPRITESHEET(videomanager, "data/images/explosion.png");
    LOADSPRITESHEET(videomanager, "data/images/quake.png");
    LOADSPRITESHEET(videomanager, "data/images/life_potion2.png");
    LOADSPRITESHEET(videomanager, "data/images/mana_potion.png");
    LOADSPRITESHEET(videomanager, "data/images/sight_potion.png");
    LOADSPRITESHEET(videomanager, "data/images/stairs3.png");
    LOADSPRITESHEET(videomanager, "data/images/ground2_106x54.png");
    LOADSPRITESHEET(videomanager, "data/images/stoneblock3.png");
    LOADSPRITESHEET(videomanager, "data/images/door.png");
    LOADSPRITESHEET(videomanager, "data/images/lightning_bolt.png");
    LOADSPRITESHEET(videomanager, "data/images/yellow_fire_ball.png");
    LOADSPRITESHEET(videomanager, "data/images/tile_switch.png");
    {   ugdk::FlexibleSpritesheet *menu_eye_sheet = new ugdk::FlexibleSpritesheet(videomanager->LoadTexture("data/images/eye.png"));
        menu_eye_sheet->set_frame_size(ugdk::Vector2D(128.0f, 96.0f));
        videomanager->AddSpritesheet("data/images/eye.png", menu_eye_sheet); }

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
}

int main(int argc, char *argv[]) {
    Settings* settings = Settings::reference();

    std::string rootpath = Constants::INSTALL_LOCATION;
    struct stat st;
    if(stat(rootpath.c_str(), &st) != 0)
        rootpath = "./";

    engine()->Initialize("Horus Eye", settings->resolution_vector(), settings->fullscreen(), rootpath, "data/images/eye.bmp");
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
