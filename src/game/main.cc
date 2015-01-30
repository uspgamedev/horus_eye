#include <ugdk/system/config.h>

#ifdef UGDK_PYTHON_ENABLED
#include "Python.h"
#endif

#include <ugdk/system/engine.h>
#include <ugdk/resource/module.h>
#include <ugdk/resource/genericcontainer.h>

#include "initializer.h"
#include "constants.h"
#include "game/skills/initskills.h"
#include "game/utils/isometricanimationset.h"

#include "frontend/frontend.h"

#include <ugdk/script.h>

#include <ugdk/script/scriptmanager.h>
#include <ugdk/script/virtualobj.h>
#include <ugdk/debug/log.h>

#include <string>
#include "SDL.h"
#ifdef main
#undef main
#endif

extern int HORUS_MODULES_HEARTBEAT;

void ExitWithFatalError(const std::string& msg) {
    if (SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal Error", msg.c_str(), NULL) < 0) {
        ugdk::debug::Log(ugdk::debug::LogLevel::ERROR, "Horus Eye", msg);
    }
    assert(false);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    HORUS_MODULES_HEARTBEAT = 1;

    ugdk::system::Configuration engine_config;
    frontend::PopuplateUGDKConfiguration(engine_config);

    engine_config.base_path = constants::data_location();
    if(!VerifyFolderExists(engine_config.base_path))
        ExitWithFatalError("Data folder '" + engine_config.base_path + "' specified by config.h could not be found.");

    ugdk::script::InitScripts();
    if(!ugdk::system::Initialize(engine_config))
        ExitWithFatalError("Could not initialize UGDK.");

#ifdef UGDK_PYTHON_ENABLED
#ifdef EMBBEDED_UGDK
    {
        PyObject *path = PySys_GetObject("path");
        PyList_Append(path, PyString_FromString(EMBBEDED_UGDK "/src/generated"));
    }
#endif // EMBBEDED_UGDK
    {
        auto wrapper = SCRIPT_MANAGER()->GetWrapper("Python");
        wrapper->ExecuteCode("import ugdk_ui");
    }
#endif // UGDK_PYTHON_ENABLED
    
    {
        auto wrapper = SCRIPT_MANAGER()->GetWrapper("Lua"); 
        wrapper->ExecuteCode("require 'ugdk.action'");
        wrapper->ExecuteCode("require 'core'; require 'map'; require 'context'; require 'component'");
        SCRIPT_MANAGER()->LoadModule("init_constants");
    }
    
    ugdk::resource::manager()->CreateContainer<utils::IsometricAnimationSet*>();
    ugdk::resource::manager()->CreateContainer<skills::Skill*>();
    skills::InitHeroSkills();
    skills::InitMummySkills();

    //
    AddHorusShader();
    frontend::Initialize();

    frontend::Start();
    ugdk::system::Run();
    ugdk::system::Release();
    return 0;
}
