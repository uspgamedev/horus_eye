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
%import(module="component") <game/sprites.h>
%import(module="component") <game/components/base.h>
%import(module="component") <game/components/controller.h>
%import(module="component") <game/sprites/worldobject.h>

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