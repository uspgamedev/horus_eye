#include "game/builders/aibuilder.h"

#include "game/ai/ai.h"

#include <ugdk/debug/log.h>
#include <ugdk/script/scriptmanager.h>
#include <ugdk/script/virtualobj.h>


namespace builder {
namespace AIBuilder {

using ai::AI;
using std::vector;
using std::string;
using ugdk::script::VirtualObj;

/** arguments[0] is the script name. */
AI* AIScript(const vector<string>& arguments) {
    if (arguments.empty()) return nullptr;
    
    VirtualObj script_generator = SCRIPT_MANAGER()->LoadModule("ais." + arguments[0]);
    if (!script_generator) return nullptr;
    if (!script_generator["generate"]) {
        ugdk::debug::Log(ugdk::debug::LogLevel::WARNING, "Horus Eye",
                         "Function 'generate' not found in 'ais.", arguments[0], "'");
        return nullptr;
    }

    AI* aiobj = new AI(script_generator.wrapper(), arguments[0]);

    VirtualObj::List args;
    {
        VirtualObj obj(script_generator.wrapper());
        obj.set_value<AI*>(aiobj, false);
        args.push_back(obj);
    }

    for (vector<string>::const_iterator it = arguments.begin()+1; it != arguments.end(); it++) {
        VirtualObj obj(script_generator.wrapper());
        obj.set_value<string>(*it);
        args.push_back(obj);
    }
    VirtualObj script_data = script_generator["generate"].Call(args);

    return aiobj;
}

AI* AIScript(const std::string& script_name) {
    return AIScript(vector<string>(1, script_name));
}

}
}
