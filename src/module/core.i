%module core

%include <module/util_ugdk.swig>
%include "std_string.i"

%{

#include <ugdk/action/scene.h>
#include <game/campaigns/campaigndescriptor.h>
#include <game/campaigns/campaign.h>
#include <game/scenes/world.h>
#include <game/core/coordinates.h>

%}

%import(module="ugdk_system") <ugdk/system/taskplayer.h>
%import(module="ugdk_action") <ugdk/action/scene.h>
%import(module="ugdk_action") <ugdk/action/entity.h>
%import(module="ugdk_math") <ugdk/math/vector2D.h>
%import(module="component") "component.i"

%include <game/campaigns/campaigndescriptor.h>
%include <game/campaigns/campaign.h>
%include <game/scenes/world.h>
%include <game/core/coordinates.h>

namespace campaigns {
    export_class(Campaign)
}
namespace scene {
    export_class(World)
}

confirm_exports(core)