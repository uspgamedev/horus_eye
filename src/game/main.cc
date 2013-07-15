#include "Python.h"

#include <string>
#include <ugdk/system/engine.h>
#include <ugdk/resource/module.h>
#include <ugdk/resource/genericcontainer.h>
#include <ugdk/audio/module.h>
#include <ugdk/graphic/textmanager.h>
#include <ugdk/graphic/module.h>
#include <ugdk/math/vector2D.h>
#include <ugdk/util/languagemanager.h>
#include "SDL.h"
#include "initializer.h"

#include "constants.h"
#include "utils/levelmanager.h"
#include "utils/settings.h"
#include "game/builders/recipes/init.h"
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

    ugdk::graphic::VideoSettings video_settings;
    video_settings.resolution = settings->resolution_vector();
    video_settings.fullscreen = settings->fullscreen();
    video_settings.vsync = true; // TODO: configurable
    video_settings.light_system = true;
    ugdk::graphic::manager()->ChangeSettings(video_settings);

    AddHorusShader();

    if(!ugdk::system::language_manager()->Setup(settings->language_name())) {
        fprintf(stderr, "Language Setup FAILURE!!\n\n");
    }
    level_manager()->Initialize();
}

int main(int argc, char *argv[]) {
    Settings* settings = Settings::reference();

    ugdk::system::Configuration engine_config;
    engine_config.window_title      = "Horus Eye";
    engine_config.window_resolution = settings->resolution_vector();
    engine_config.fullscreen        = settings->fullscreen();

    engine_config.base_path = constants::data_location();
    if(!VerifyFolderExists(engine_config.base_path)) {
#ifdef WIN32
        MessageBox(HWND_DESKTOP, "Horus Eye could not find the Data folder.", "Fatal Error", MB_OK | MB_ICONERROR);
        return 2;
#else
        engine_config.base_path = "./";
#ifdef DEBUG
        fprintf(stderr, "Warning: data folder '%s' specified by config.h could not be found. Using default './'\n", constants::data_location().c_str());
#endif
#endif
    }

#ifndef ISMAC
    engine_config.window_icon = "images/eye.bmp";
#else
    // On Mac OS X, the icon should be handled with a *.icns file inside the app
    engine_config.window_icon = "";
#endif

    ugdk::script::InitScripts();
    ugdk::system::Initialize(engine_config);
#ifdef EMBBEDED_UGDK
    {
        PyObject *path = PySys_GetObject("path");
        PyList_Append(path, PyString_FromString(EMBBEDED_UGDK "/src/generated"));
    }
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
    builder::InitRecipes();

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
