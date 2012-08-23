
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

//proxy_class(component::Logic)

// context::*

%include <game/context.h>

confirm_exports(component)

