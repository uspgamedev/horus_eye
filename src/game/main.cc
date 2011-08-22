
#include <string>
#include "config.h"
#include <ugdk/engine.h>
#include <ugdk/videomanager.h>
#include <ugdk/audiomanager.h>
#include <ugdk/textmanager.h>
#include <ugdk/vector2D.h>
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
    Settings settings = Settings();

    // Checks if restarting game. Avoids changing resolution during
    // startup, to avoid taking longer uselessly.
    if(level_manager()->RestartGameQueued())
        engine()->video_manager()->ChangeResolution(settings.resolution_vector(), settings.fullscreen());

    text_loader()->Initialize(settings.language_file());
    level_manager()->Initialize();
}

#include "../framework/animationparser.h"
#include "../framework/animationset.h"

int main(int argc, char *argv[]) {
	Settings settings = Settings();

	// Loads rootpath from the file specified on settings
	char rootpath[1024];
	strcpy(rootpath, "./");
	FILE *root_file = fopen(settings.root_file_path().c_str(), "r");
	if(root_file != NULL) {
		fgets(rootpath, 1024, root_file);
		fclose(root_file);
		int i = strlen(rootpath) - 1;
		if(rootpath[i] != '/' && rootpath[i] != '\\') {
			rootpath[i + 1] = '/';
			rootpath[i + 2] = '\0';
		}
	}

    engine()->Initialize("Horus Eye", settings.resolution_vector(), settings.fullscreen(), rootpath, "data/images/eye.bmp");
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
