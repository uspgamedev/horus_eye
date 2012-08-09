
%module component

%include <module/export.swig>

%{

#include <game/sprites/worldobject.h>

%}

%include <game/sprites/worldobject.h>

namespace sprite {
    export_class(WorldObject)
}

confirm_exports(component)
