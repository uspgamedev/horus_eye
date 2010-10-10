#include <string>
#include "../framework/engine.h"
#include "../framework/videomanager.h"
#include "../framework/audiomanager.h"
#include "../framework/vector2D.h"
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

framework::Engine* engine() {
    return framework::Engine::reference();
}

void StartGame() {
    Settings settings = Settings();
    engine()->video_manager()->ChangeResolution(settings.resolution_vector(), settings.fullscreen());

    text_loader()->Initialize(settings.language_file());

    if(settings.background_music())
        engine()->audio_manager()->LoadMusic(utils::Constants::BACKGROUND_MUSIC)->PlayForever();
    else
        engine()->audio_manager()->LoadMusic(utils::Constants::BACKGROUND_MUSIC)->Stop();
    level_manager()->Initialize();
}


int main(int argc, char *argv[]) {
    framework::Vector2D default_resolution = framework::Vector2D(1024, 768);
    engine()->Initialize("Horus Eye", default_resolution, false);
    do {
        StartGame();
        engine()->Run();
        level_manager()->Finish();
    } while(level_manager()->RestartGameQueued());
    engine()->Release();
    return 0;
}
