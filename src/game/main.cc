#include <string>
#include <sys/stat.h>
#include <ugdk/base/engine.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/audio/audiomanager.h>
#include <ugdk/graphic/textmanager.h>
#include <ugdk/math/vector2D.h>

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

void StartGame() {
    Settings* settings = Settings::reference();

    // Checks if restarting game. Avoids changing resolution during
    // startup, to avoid taking longer uselessly.
    if(level_manager()->RestartGameQueued())
        engine()->video_manager()->ChangeResolution(settings->resolution_vector(), settings->fullscreen());

    text_loader()->Initialize(settings->language_file());
    level_manager()->Initialize();
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

        // Releases all loaded textures, to avoid issues when changing resolution.
        engine()->video_manager()->Release();
        engine()->text_manager()->Release();

    } while(level_manager()->RestartGameQueued());
    engine()->Release();
    return 0;
}
