#ifndef ANDROID
#include "Python.h"
#endif

#include <string>
#include <ugdk/system/engine.h>
#include <ugdk/resource/module.h>
#include <ugdk/resource/genericcontainer.h>
#include <ugdk/audio/module.h>
#include <ugdk/graphic/textmanager.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/node.h>
#include <ugdk/desktop/module.h>
#include <ugdk/desktop/window.h>
#include <ugdk/math/vector2D.h>
#include <ugdk/util/languagemanager.h>
#include "initializer.h"

#include "constants.h"
#include "utils/levelmanager.h"
#include "utils/settings.h"
#include "game/scenes/world.h"
#include "game/skills/initskills.h"
#include "game/utils/isometricanimationset.h"

#include <ugdk/script.h>
#define MODULE_AUTO_LOAD(ACTION) ACTION(HORUS)

#include <ugdk/script/scriptmanager.h>
#include <ugdk/script/virtualobj.h>
#include <ugdk/script/languages/lua/luawrapper.h>
#include <ugdk/script/languages/python/pythonwrapper.h>

#ifdef WIN32
#include <windows.h>
#endif

using namespace utils;

utils::LevelManager* level_manager() {
    return utils::LevelManager::reference();
}

void StartGame() {
    Settings* settings = Settings::reference();

    ugdk::desktop::manager()->primary_window()->ChangeSettings(
        settings->resolution_vector(),
        settings->fullscreen());
    ugdk::graphic::manager()->canvas()->Resize(settings->resolution_vector());

    AddHorusShader();
    ugdk::system::PushScene(CreateHorusLightrenderingScene());

    if(!ugdk::system::language_manager()->Setup(settings->language_name())) {
        fprintf(stderr, "Language Setup FAILURE!!\n\n");
    }
    level_manager()->Initialize();
}

void ExitWithFatalError(const std::string& msg) {
#ifdef _WIN32
    MessageBox(HWND_DESKTOP, msg.c_str(), "Fatal Error", MB_OK | MB_ICONERROR);
#else
    fprintf(stderr, "Fatal Error: %s\n", msg.c_str());
#endif
    assert(false);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    Settings* settings = Settings::reference();

    ugdk::system::Configuration engine_config;
    engine_config.canvas_size = settings->resolution_vector();
    engine_config.windows_list[0].title      = "Horus Eye";
    engine_config.windows_list[0].size       = settings->resolution_vector();
    engine_config.windows_list[0].fullscreen = settings->fullscreen();

    engine_config.base_path = constants::data_location();
    if(!VerifyFolderExists(engine_config.base_path))
        ExitWithFatalError("Data folder '" + engine_config.base_path + "' specified by config.h could not be found.");

#ifndef ISMAC
    engine_config.windows_list[0].icon = "images/eye.bmp";
#else
    // On Mac OS X, the icon should be handled with a *.icns file inside the app
    engine_config.window_icon = "";
#endif

    ugdk::script::InitScripts();
    if(!ugdk::system::Initialize(engine_config))
        ExitWithFatalError("Could not initialize UGDK.");

#ifdef EMBBEDED_UGDK
#ifndef ANDROID
    {
        PyObject *path = PySys_GetObject("path");
        PyList_Append(path, PyString_FromString(EMBBEDED_UGDK "/src/generated"));
    }
#endif
#endif
    
    {
        SCRIPT_MANAGER()->LoadModule("init_constants");
    }
    
    ugdk::system::language_manager()->RegisterLanguage("en_US", "text/lang_en.txt");
    ugdk::system::language_manager()->RegisterLanguage("pt_BR", "text/lang_pt_br.txt");

    ugdk::resource::manager()->add_container<skills::Skill*>(new ugdk::resource::GenericContainer<skills::Skill*>);
    ugdk::resource::manager()->add_container<utils::IsometricAnimationSet*>(new ugdk::resource::GenericContainer<utils::IsometricAnimationSet*>);
    skills::InitHeroSkills();
    skills::InitMummySkills();

    do {
        // Initializes game data
        StartGame();

        // Transfers control to the framework.
        ugdk::system::Run();

        // Releases data persistant between levels.
        level_manager()->Finish();

        // Releases all loaded textures, to avoid issues when changing resolution.
        //engine()->video_manager()->Release();

    } while(level_manager()->RestartGameQueued());
    ugdk::system::Release();
    return 0;
}
