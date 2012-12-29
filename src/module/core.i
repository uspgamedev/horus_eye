%module core

%include <module/export.swig>
%include <module/ownership.swig>

%{

#include <game/core/coordinates.h>

%}

%include <game/core/coordinates.h>

confirm_exports(core)