
%module ai

%include <module/export.swig>
%include <module/ownership.swig>
%include <module/proxy.swig>
%include <module/virtualobj.swig>
%include "std_string.i"
%include "std_vector.i"

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
#include <game/ai/delaymodule.h>

#include <game/ai/blocks/followtarget.h>
#include <game/ai/blocks/randommovement.h>
#include <game/ai/blocks/targetdetector.h>
#include <game/ai/blocks/useweapon.h>

#include <module/ai/logicblockproxy.h>

%}

%import(module="component") <game/components/base.h>
%import(module="component") <game/components/controller.h>

%include <game/ai.h>

%ignore ai::AI::AI(sprite::WorldObject *owner, ugdk::script::LangWrapper* script_wrapper, const std::string& name);
%ignore ai::AIData::AIData(ugdk::script::LangWrapper* script_wrapper);
%ignore ai::AIData::script_wrapper() const;

proxy_class(ai::LogicBlock)

enable_disown(ai::AIModule* root)
%include <game/ai/ai.h>
disable_disown(ai::AIModule* root)

virtual_class()
%include <game/ai/aidata.h>
%include <game/ai/aimodule.h>

%include <game/ai/logicblock.h>

enable_disown(ai::AIModule* child)
%include <game/ai/conditionmodule.h>
%include <game/ai/randommodule.h>
%include <game/ai/sequencemodule.h>
%include <game/ai/delaymodule.h>
//enable_disown(ai::LogicBlock* logic)
%include <game/ai/logicmodule.h>
//disable_disown(ai::LogicBlock* logic)
disable_disown(ai::AIModule* child)

%include <game/ai/blocks/followtarget.h>
%include <game/ai/blocks/randommovement.h>
enable_disown(pyramidworks::geometry::GeometricShape* area)
%include <game/ai/blocks/targetdetector.h>
disable_disown(pyramidworks::geometry::GeometricShape* area)
%include <game/ai/blocks/useweapon.h>

namespace ai {
    export_class(AI)
    export_class(AIData)
    export_class(AIModule)
    export_class(ConditionModule)
    export_class(LogicBlock)
    export_class(LogicModule)
    export_class(RandomModule)
    export_class(SequenceModule)
    export_class(DelayModule)
    namespace blocks {
        export_class(FollowTarget)
        export_class(RandomMovement)
        export_class(TargetDetector)
        export_class(UseWeapon)
    }
}

confirm_exports(ai)
