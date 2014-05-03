%module core

%include <module/util_ugdk.swig>
%include "std_string.i"

%{

#include <game/campaigns/campaigndescriptor.h>
#include <game/campaigns/campaign.h>
#include <game/core/coordinates.h>

%}

%include <game/campaigns/campaigndescriptor.h>
%include <game/campaigns/campaign.h>
%include <game/core/coordinates.h>

namespace campaigns {
    export_class(Campaign)
}

confirm_exports(core)