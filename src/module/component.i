
%module component

%include <module/export.swig>
//%include <module/proxy.swig>
%include "std_string.i"
%include "std_map.i"

%{

#include <game/context.h>
#include <game/sprites/worldobject.h>
#include <game/components/logic.h>
#include <game/components/damageable.h>
#include <game/components/graphic.h>
#include <game/components/controller.h>
#include <game/components/animation.h>
#include <game/components/direction.h>

%}

%import(module="ugdk_action") <ugdk/action/entity.h>
%include <game/context.h>
%include <game/sprites/worldobject.h>
%include <game/components/direction.h>
%include <game/components/logic.h>
%include <game/components/damageable.h>
%include <game/components/graphic.h>
%include <game/components/controller.h>
%include <game/components/animation.h>

// TODO
//proxy_class(component::Logic)

namespace sprite {
    export_class(WorldObject)
}

namespace component {
    export_class(Direction)
    export_class(Logic)
    export_class(Damageable)
    export_class(Graphic)
    export_class(Controller)
    export_class(Animation)
}

confirm_exports(component)
