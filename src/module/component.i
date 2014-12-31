
%module component

%include <module/util_ugdk.swig>

%include "std_string.i"
%include "std_map.i"
%include "module/memory.i"

%{

#include <ugdk/action/scene.h>
#include <game/sprites/effect.h>
#include <game/sprites/worldobject.h>
#include <game/sprites/objecthandle.h>
#include <game/resources/energy.h>
#include <game/components/base.h>
#include <game/components/damageable.h>
#include <game/components/animator.h>
#include <game/components/graphic.h>
#include <game/components/lightemitter.h>
#include <game/components/controller.h>
#include <game/components/animation.h>
#include <game/components/direction.h>
#include <game/components/caster.h>
#include <game/components/body.h>
#include <game/components/condition.h>
#include <game/components/timedlife.h>

#include <ugdk/script/baseproxy.h>
#include <module/component/baseproxy.h>
#include <module/ugdk/action/observerproxy.h>

%}

proxy_class(ugdk::action::Observer)
proxy_class(component::Base)

%import(module="ugdk_action") <ugdk/action.h>
%import(module="ugdk_action") <ugdk/action/observer.h>
%import(module="ugdk_action") <ugdk/action/animationplayer.h>
%import(module="ugdk_graphic") <ugdk/graphic/spritetypes.h>
%import(module="component") <game/sprites.h>
%import(module="component") <game/components.h>

// sprite::WorldObject

%ignore sprite::WorldObject::set_start_to_die_callback (std::function<void (WorldObject*)> on_death_start_callback);
%ignore sprite::WorldObject::set_die_callback(std::function<void (WorldObject*)> on_death_end_callback);

%template(WObjSharedFromThis) std::enable_shared_from_this<sprite::WorldObject>;

enable_disown(pyramidworks::collision::CollisionObject*)

%include <game/core/gamelayer.h>
%include <game/utils/isometricanimationset.h>
%include <game/sprites/effect.h>
%include <game/sprites/worldobject.h>
%include <game/sprites/objecthandle.h>

%template(WObjPtr) std::shared_ptr<sprite::WorldObject>;
%template(WObjWeakPtr) std::weak_ptr<sprite::WorldObject>;

// resource::Energy and dependencies

%ignore resource::Resource::operator=;
%ignore resource::Resource::operator resource_t;

%include <game/resources/resource.h>
%template(DoubleResource) resource::Resource<double>;
%template(IntegerResource) resource::Resource<int>;

%include <game/resources/containedresource.h>
%template(DoubleContainedResource) resource::ContainedResource<double>;

%include <game/resources/energy.h>

// component::*

%ignore component::Direction::operator!;
%ignore component::Direction::operator|=;
%ignore component::Direction::operator|;
%ignore component::Direction::operator&;
%ignore component::Direction::operator const bool;

%include <game/components/base.h>
%include <game/components/direction.h>
%include <game/components/animator.h>
%include <game/components/graphic.h>
%include <game/components/damageable.h>
%include <game/components/lightemitter.h>
%include <game/components/controller.h>
%include <game/components/animation.h>
%include <game/components/caster.h>
%include <game/components/body.h>
%include <game/components/condition.h>
%include <game/components/timedlife.h>

disable_disown(pyramidworks::collision::CollisionObject*)

namespace sprite {
    export_class(WorldObject)
    export_class(Effect)

    export_class(WObjPtr)
    export_class(WObjWeakPtr)
    export_class(ObjectHandle)
}
namespace component {
    export_class(Base)
    export_class(Direction)
    export_class(Damageable)
    export_class(Animator)
    export_class(Graphic)
    export_class(LightEmitter)
    export_class(Controller)
    export_class(Animation)
    export_class(Caster)
    export_class(Body)
    export_class(Condition)
	export_class(TimedLife)
}
confirm_exports(component)
