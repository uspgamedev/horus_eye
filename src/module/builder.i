
%module builder

%include <module/export.swig>
%include <module/ownership.swig>
%include "std_string.i"
%include "std_vector.i"

%{

#include <game/sprites.h>
#include <game/builders/doodadbuilder.h>
#include <game/builders/entitybuilder.h>
#include <game/builders/explosionbuilder.h>
#include <game/builders/itembuilder.h>
#include <game/builders/mummybuilder.h>
#include <game/builders/projectilebuilder.h>
#include <game/builders/scriptbuilder.h>

%}

%import(module="ugdk_action") <ugdk/action/entity.h>
%import(module="component") <game/sprites/worldobject.h>

%ignore builder::DoodadBuilder::Door(const std::vector<std::string>& arguments, scene::World* world);

%template(StringList) std::vector<std::string>;

%include <game/builders/doodadbuilder.h>
%include <game/builders/entitybuilder.h>
%include <game/builders/explosionbuilder.h>
%include <game/builders/itembuilder.h>
%include <game/builders/mummybuilder.h>
%include <game/builders/projectilebuilder.h>
%include <game/builders/scriptbuilder.h>

confirm_exports(builder)

