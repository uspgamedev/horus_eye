
%module context

%include <module/util_ugdk.swig>

%include "std_string.i"
%include "std_map.i"
%include "std_vector.i"
%include "module/memory.i"

%{

#include <game/context.h>

%}

%import(module="ugdk_action") <ugdk/action/observer.h>
%import(module="ugdk_math") <ugdk/math/vector2D.h>
%import(module="pyramidworks_geometry") <pyramidworks/geometry.h>

%newobject context::BuildWorldObject(const std::string& scriptname);

enable_disown(sprite::WObjRawPtr new_obj)
enable_disown(ugdk::graphic::TextureAtlas* atlas)
%include <game/context.h>
disable_disown(sprite::WObjRawPtr new_obj)
disable_disown(ugdk::graphic::TextureAtlas* atlas)

confirm_exports(context)

