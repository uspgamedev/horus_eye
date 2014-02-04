
%module constants

%include <module/util_ugdk.swig>

%include "std_string.i"

%{

#include <game/constants.h>

%}


%include <game/constants.h>

confirm_exports(constants)

