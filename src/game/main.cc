#include <ugdk/system/config.h>

#ifdef UGDK_PYTHON_ENABLED
#include "Python.h"
#endif

#include <ugdk/system/engine.h>
#include <ugdk/resource/module.h>
#include <ugdk/resource/genericcontainer.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/desktop/module.h>
#include <ugdk/desktop/window.h>
#include <ugdk/math/vector2D.h>
#include <ugdk/util/languagemanager.h>

#include "initializer.h"
#include "constants.h"
#include "utils/settings.h"
#include "game/scenes/world.h"
#include "game/skills/initskills.h"
#include "game/utils/isometricanimationset.h"
#include "game/builders/goodmenubuilder.h"

#include <ugdk/script.h>
#define MODULE_AUTO_LOAD(ACTION) ACTION(HORUS)

#include <ugdk/script/scriptmanager.h>
#include <ugdk/script/virtualobj.h>
#include <ugdk/debug/log.h>

#include <string>
#include "SDL.h"
#ifdef main
#undef main
#endif

using namespace utils;

void StartGame() {
    Settings* settings = Settings::reference();

    ugdk::desktop::manager()->primary_window()->ChangeSettings(
        settings->resolution_vector(),
        settings->fullscreen(),
        settings->vsync());
    ugdk::graphic::manager()->ResizeScreen(ugdk::desktop::manager()->primary_window()->size());

    AddHorusShader();

    if(!ugdk::system::language_manager()->Setup(settings->language_name())) {
        ugdk::debug::Log(ugdk::debug::LogLevel::ERROR, "Horus Eye", "Language Setup FAILURE");
    }
    ugdk::system::PushScene(builder::MainMenu);
}

void ExitWithFatalError(const std::string& msg) {
    if (SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal Error", msg.c_str(), NULL) < 0) {
        ugdk::debug::Log(ugdk::debug::LogLevel::ERROR, "Horus Eye", msg);
    }
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

#if defined(EMBBEDED_UGDK) && defined(UGDK_PYTHON_ENABLED)
    {
        PyObject *path = PySys_GetObject("path");
        PyList_Append(path, PyString_FromString(EMBBEDED_UGDK "/src/generated"));
    }
#endif
    
    {
        auto wrapper = SCRIPT_MANAGER()->GetWrapper("Lua"); 
        wrapper->ExecuteCode("require 'ugdk.action'");
        wrapper->ExecuteCode("require 'core'; require 'map'; require 'context'; require 'component'");
        SCRIPT_MANAGER()->LoadModule("init_constants");
    }
    
    ugdk::system::language_manager()->RegisterLanguage("en_US", "text/lang_en.txt");
    ugdk::system::language_manager()->RegisterLanguage("pt_BR", "text/lang_pt_br.txt");

    ugdk::resource::manager()->add_container<utils::IsometricAnimationSet*>(new ugdk::resource::GenericContainer<utils::IsometricAnimationSet*>);
    ugdk::resource::manager()->add_container<skills::Skill*>(new ugdk::resource::GenericContainer<skills::Skill*>);
    skills::InitHeroSkills();
    skills::InitMummySkills();

    do {
        // Initializes game data
        StartGame();

        // Transfers control to the framework.
        ugdk::system::Run();

        // Releases all loaded textures, to avoid issues when changing resolution.
        //engine()->video_manager()->Release();

    } while(RestartGameQueued());

    ugdk::system::Release();
    return 0;
}
