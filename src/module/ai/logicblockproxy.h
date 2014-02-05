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
        ugdk::script::VirtualObj stat = ( proxy_ | "Update" )(dt, data);
        return static_cast<AIModule::Status>( stat.value<int>() );
    }
	virtual void Finish() {
        (proxy_ | "End")();
    }

};

} // namespace ai

#endif
