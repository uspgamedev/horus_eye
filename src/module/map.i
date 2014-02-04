
%module map

%include <module/util_ugdk.swig>

%include "std_string.i"
%include "std_vector.i"
%include "std_map.i"
%include "memory.i"

%{

#include <game/map.h>
#include <game/map/room.h>
#include <game/map/tile.h>
#include <game/sprites/worldobject.h>

%}

%import(module="component") <game/sprites.h>
%import(module="component") <game/sprites/worldobject.h>

%include <game/map.h>    
%include <game/map/room.h>
%include <game/map/tile.h>

namespace map {
    export_class(Room)
    export_class(Tile)
}

confirm_exports(map)
