#include <ugdk/script/scriptmanager.h>
#include <ugdk/script/virtualobj.h>

#include "game/builders/aibuilder.h"

#include "game/ai/ai.h"


namespace builder {
namespace AIBuilder {

using ai::AI;
using std::vector;
using std::string;
using ugdk::script::VirtualObj;

/*Always remember: we should create the AIs (and their AIModule trees) in a top-down method...
				like in the following order:
	AI -> root AIModule -> AI.SetRootModule -> [ other module -> set module as child from parent ]* -> return AI */

/** arguments[0] is the script name. */
AI* Script(sprite::WorldObject* owner, const vector<string>& arguments) {
    if (arguments.empty()) return NULL;
    VirtualObj script_generator = SCRIPT_MANAGER()->LoadModule("ai." + arguments[0]);
    if(!script_generator) return NULL;
    if(!script_generator["generate"]) {
        fprintf(stderr, "Function 'generate' not found in 'ai.%s'.\n", arguments[0].c_str());
        return NULL;
    }

    AI* aiobj = new AI(owner, script_generator.wrapper(), arguments[0]);

    VirtualObj::List args;
    {
        VirtualObj obj(script_generator.wrapper());
        obj.set_value<AI*>(aiobj);
        args.push_back(obj);
    }

    for (vector<string>::const_iterator it = arguments.begin()+1; it != arguments.end(); it++) {
        VirtualObj obj(script_generator.wrapper());
        obj.set_value<string>(*it);
        args.push_back(obj);
    }
    VirtualObj script_data = script_generator["generate"](args);

    return aiobj;
}

AI* Script(sprite::WorldObject* owner, const std::string& script_name) {
    return Script(owner, vector<string>(1, script_name));
}

}
}
