
%module component

%include <module/export.swig>
%include <module/ownership.swig>
%include <module/proxy.swig>
%include "std_string.i"
%include "std_map.i"

%{

#include <game/sprites/condition.h>
#include <game/sprites/worldobject.h>
#include <game/resources/energy.h>
#include <game/components/base.h>
#include <game/components/logic.h>
#include <game/components/damageable.h>
#include <game/components/graphic.h>
#include <game/components/controller.h>
#include <game/components/animation.h>
#include <game/components/direction.h>
#include <game/components/caster.h>

#include <ugdk/script/baseproxy.h>
#include <module/component/logicproxy.h>

%}

%import(module="ugdk_action") <ugdk/action/entity.h>
%import(module="ugdk_action") <ugdk/action/observer.h>
%import(module="component") <game/sprites.h>

proxy_class(component::Logic)

// sprite::WorldObject

%ignore sprite::WorldObject::set_start_to_die_callback (std::tr1::function<void (WorldObject*)> on_death_start_callback);
%ignore sprite::WorldObject::set_die_callback(std::tr1::function<void (WorldObject*)> on_death_end_callback);

enable_disown(pyramidworks::collision::CollisionObject* col)
enable_disown(component::Logic* logic)
enable_disown(component::Damageable* damageable)
enable_disown(component::Graphic* graphic)
enable_disown(component::Controller* controller)
enable_disown(component::Animation* animation)
enable_disown(component::Caster* caster)

%include <game/sprites/condition.h>
%include <game/sprites/worldobject.h>

disable_disown(pyramidworks::collision::CollisionObject* col)
disable_disown(component::Logic* logic)
disable_disown(component::Damageable* damageable)
disable_disown(component::Graphic* graphic)
disable_disown(component::Controller* controller)
disable_disown(component::Animation* animation)
disable_disown(component::Caster* caster)

// resource::Energy and dependencies

%ignore resource::Resource;
%ignore resource::Resource::operator=;
%ignore resource::ContainedResource;

%include <game/resources/resource.h>
%template(Resource) resource::Resource<double>;
%template(IntegerResource) resource::Resource<int>;

%include <game/resources/containedresource.h>
%template(NumericContainedResource) resource::ContainedResource<double>;

%include <game/resources/energy.h>

// component::*

%ignore component::Direction::operator!;
%ignore component::Direction::operator|=;
%ignore component::Direction::operator|;
%ignore component::Direction::operator&;
%ignore component::Direction::operator const bool;

%include <game/components/direction.h>
%include <game/components/base.h>
%include <game/components/logic.h>
%include <game/components/damageable.h>
%include <game/components/graphic.h>
%include <game/components/controller.h>
%include <game/components/animation.h>
%include <game/components/caster.h>

namespace sprite {
    export_class(WorldObject)
    export_class(Condition)
}

namespace component {
    export_class(Base)
    export_class(Direction)
    export_class(Logic)
    export_class(Damageable)
    export_class(Graphic)
    export_class(Controller)
    export_class(Animation)
    export_class(Caster)
}

confirm_exports(component)

