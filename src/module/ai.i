
%module ai

%include <module/export.swig>
%include <module/ownership.swig>
%include <module/proxy.swig>

%{

#include <game/ai.h>
#include <game/ai/ai.h>
#include <game/ai/aidata.h>
#include <game/ai/aimodule.h>
#include <game/ai/conditionmodule.h>
#include <game/ai/logicblock.h>
#include <game/ai/logicmodule.h>
#include <game/ai/randommodule.h>
#include <game/ai/sequencemodule.h>

#include <ugdk/script/baseproxy.h>
#include <module/ai/logicblockproxy.h>

%}

%import(module="component") <game/components/controller.h>

%include <game/ai.h>

proxy_class(ai::LogicBlock)

enable_disown(ai::AIModule* root)
%include <game/ai/ai.h>
disable_disown(ai::AIModule* root)

%include <game/ai/aidata.h>
%include <game/ai/aimodule.h>

enable_disown(ai::AIModule* child)
%include <game/ai/conditionmodule.h>
%include <game/ai/randommodule.h>
%include <game/ai/sequencemodule.h>
enable_disown(ai::LogicBlock* logic)
%include <game/ai/logicmodule.h>
disable_disown(ai::LogicBlock* logic)
disable_disown(ai::AIModule* child)

%include <game/ai/logicblock.h>

namespace ai {
    export_class(AI)
    export_class(AIData)
    export_class(AIModule)
    export_class(ConditionModule)
    export_class(LogicBlock)
    export_class(LogicModule)
    export_class(RandomModule)
    export_class(SequenceModule)
}

confirm_exports(ai)
