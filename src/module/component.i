
%module component

%include <module/export.swig>

%{

#include <game/sprites/worldobject.h>
#include <game/context.h>

%}

%import(module="ugdk_action") <ugdk/action/entity.h>
%include <game/sprites/worldobject.h>
%include <game/context.h>

namespace sprite {
    export_class(WorldObject)
}

confirm_exports(component)
