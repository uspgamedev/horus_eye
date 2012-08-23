
%module context

%include <module/export.swig>
%include <module/ownership.swig>
%include "std_string.i"
%include "std_map.i"

%{

#include <game/context.h>

%}

%import(module="ugdk_action") <ugdk/action/entity.h>
%import(module="ugdk_action") <ugdk/action/observer.h>

%newobject context::BuildWorldObject(const std::string& scriptname);

enable_disown(sprite::WorldObject* new_obj)
%include <game/context.h>
disable_disown(sprite::WorldObject* new_obj)

confirm_exports(component)

