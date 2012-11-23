
%module component

%include <module/export.swig>
%include <module/ownership.swig>
%include <module/proxy.swig>
%include "std_string.i"
%include "std_map.i"

%{

#include <game/sprites/effect.h>
#include <game/sprites/worldobject.h>
#include <game/resources/energy.h>
#include <game/components/base.h>
#include <game/components/damageable.h>
#include <game/components/graphic.h>
#include <game/components/controller.h>
#include <game/components/animation.h>
#include <game/components/direction.h>
#include <game/components/caster.h>
#include <game/components/shape.h>
#include <game/components/condition.h>

#include <ugdk/script/baseproxy.h>
#include <module/component/baseproxy.h>

%}

%import(module="ugdk_action") <ugdk/action/entity.h>
%import(module="ugdk_action") <ugdk/action/observer.h>
%import(module="component") <game/sprites.h>

proxy_class(component::Base)

// sprite::WorldObject

%ignore sprite::WorldObject::set_start_to_die_callback (std::tr1::function<void (WorldObject*)> on_death_start_callback);
%ignore sprite::WorldObject::set_die_callback(std::tr1::function<void (WorldObject*)> on_death_end_callback);

enable_disown(pyramidworks::collision::CollisionObject* col)
enable_disown(component::Base* component)

%include <game/sprites/effect.h>
%include <game/sprites/worldobject.h>

disable_disown(pyramidworks::collision::CollisionObject* col)
disable_disown(component::Base* component)

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
%include <game/components/damageable.h>
%include <game/components/graphic.h>
%include <game/components/controller.h>
%include <game/components/animation.h>
%include <game/components/caster.h>
%include <game/components/shape.h>
%include <game/components/condition.h>

namespace sprite {
    export_class(WorldObject)
    export_class(Effect)
}

namespace component {
    export_class(Base)
    export_class(Direction)
    export_class(Damageable)
    export_class(Graphic)
    export_class(Controller)
    export_class(Animation)
    export_class(Caster)
    export_class(Shape)
	export_class(Condition)
}

confirm_exports(component)

