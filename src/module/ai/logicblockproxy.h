#ifndef MODULE_PROXY_AI_LOGICBLOCK_H_
#define MODULE_PROXY_AI_LOGICBLOCK_H_

#include <ugdk/script/baseproxy.h>
#include <ugdk/script/virtualobj.h>
#include <game/ai/logicblock.h>

namespace ai {

class LogicBlockProxy;

class LogicBlockProxy : public LogicBlock, public ugdk::script::BaseProxy<LogicBlockProxy> {
public:
    LogicBlockProxy(const ugdk::script::VirtualObj& proxy) :
        ugdk::script::BaseProxy<LogicBlockProxy>(proxy) {}

	virtual void Start() {
        (proxy_ | "Start")();
    }
	virtual AIModule::Status Update(double dt, AIData* data) {
        ugdk::script::VirtualObj::List args;
        ugdk::script::VirtualObj vdt = ugdk::script::VirtualObj(proxy_.wrapper());
        vdt.set_value(dt);
        args.push_back(vdt);

        ugdk::script::VirtualObj vdata = ugdk::script::VirtualObj(proxy_.wrapper());
        vdata.set_value<AIData*>(data);
        args.push_back(vdata);

        ugdk::script::VirtualObj stat = ( proxy_ | "Update" )(args);
        return static_cast<AIModule::Status>( stat.value<int>() );
    }
	virtual void Finish() {
        (proxy_ | "End")();
    }

};

} // namespace ai

#endif
