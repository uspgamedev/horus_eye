
%module builder

%include <module/util_ugdk.swig>

%include "std_string.i"
%include "std_vector.i"

%{

#include <game/sprites.h>
#include <game/builders/aibuilder.h>
#include <game/builders/collision.h>
#include <game/builders/doodadbuilder.h>
#include <game/builders/explosionbuilder.h>
#include <game/builders/itembuilder.h>
#include <game/builders/mummybuilder.h>
#include <game/builders/projectilebuilder.h>
#include <game/builders/scriptbuilder.h>
#include <game/builders/herobuilder.h>

%}

%import(module="pyramidworks_collision") <pyramidworks/collision/collisiondata.h>
%import(module="component") "component.i"
%import(module="ai") <game/ai/ai.h>

%ignore builder::DoodadBuilder::Door(const std::vector<std::string>& arguments, scene::World* world);

%template(StringList) std::vector<std::string>;

%include <game/builders/aibuilder.h>
%include <game/builders/collision.h>
%include <game/builders/doodadbuilder.h>
%include <game/builders/explosionbuilder.h>
%include <game/builders/itembuilder.h>
%include <game/builders/mummybuilder.h>
%include <game/builders/projectilebuilder.h>
%include <game/builders/scriptbuilder.h>
%include <game/builders/herobuilder.h>

confirm_exports(builder)
