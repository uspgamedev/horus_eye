
%module constants

%include <module/export.swig>
%include "std_string.i"

%{

#include <game/constants.h>

%}


%include <game/constants.h>

confirm_exports(constants)

