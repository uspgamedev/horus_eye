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

/** arguments[0] is the script name. */
AI* AIScript(sprite::WorldObject* owner, const vector<string>& arguments) {
    if (arguments.empty()) return NULL;
    VirtualObj script_generator = SCRIPT_MANAGER()->LoadModule("ais." + arguments[0]);
    if(!script_generator) return NULL;
    if(!script_generator["generate"]) {
        fprintf(stderr, "Function 'generate' not found in 'ais.%s'.\n", arguments[0].c_str());
        return NULL;
    }

    AI* aiobj = new AI(owner, script_generator.wrapper(), arguments[0]);

    VirtualObj::List args;
    {
        VirtualObj obj(script_generator.wrapper());
        obj.set_value<AI*>(aiobj); /*TODO: do this without passing ownership of the ptr to the script.*/
        args.push_back(obj);
    }

    for (vector<string>::const_iterator it = arguments.begin()+1; it != arguments.end(); it++) {
        VirtualObj obj(script_generator.wrapper());
        obj.set_value<string>(*it);
        args.push_back(obj);
    }
    VirtualObj script_data = script_generator["generate"](args);

	/*TODO: We need to return the AI* we created here. However, since we passed it to the script (SEE ABOVE), the script has the ownership.
			So we must remove the ownership of the AI* from the script otherwise it will be deleted.*/
	return args.front().value<AI*>(true); /*basically the same as returning 'aiobj', but making sure the ownership is right.*/
}

AI* AIScript(sprite::WorldObject* owner, const std::string& script_name) {
    return AIScript(owner, vector<string>(1, script_name));
}

}
}
