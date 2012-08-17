
%module component

%include <module/export.swig>
%include <module/ownership.swig>
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

// context::*

%include <game/context.h>

// sprite::WorldObject

%ignore sprite::WorldObject::set_start_to_die_callback (std::tr1::function<void (WorldObject*)> on_death_start_callback);
%ignore sprite::WorldObject::set_die_callback(std::tr1::function<void (WorldObject*)> on_death_end_callback);

enable_disown(pyramidworks::collision::CollisionObject* col)
enable_disown(component::Logic* logic)
enable_disown(component::Damageable* damageable)
enable_disown(component::Graphic* graphic)
enable_disown(component::Controller* controller)
enable_disown(component::Animation* animation)

%include <game/sprites/worldobject.h>

disable_disown(pyramidworks::collision::CollisionObject* col)
disable_disown(component::Logic* logic)
disable_disown(component::Damageable* damageable)
disable_disown(component::Graphic* graphic)
disable_disown(component::Controller* controller)
disable_disown(component::Animation* animation)

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
