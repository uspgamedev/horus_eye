
%{
#include <ugdk/script/virtualobj.h>
#include <ugdk/script/scriptmanager.h>
%}

#ifdef SWIGPYTHON
%{
#include <ugdk/script/languages/python/pythondata.h>
#include <ugdk/script/languages/python/pythonwrapper.h>
%}
#elif SWIGLUA
%{
#include <ugdk/script/languages/lua/luadata.h>
#include <ugdk/script/languages/lua/luawrapper.h>
%}
#endif

/*
This module defines a 'virtual_class()' macro for each target language.

virtual_class() creates the typemaps needed to seamless incorporation of the VirtualObj
classe in the script system. As such, it needs to be called before the %include's
in a module (it needs to go before the swig export definition (which is done via the %include)).

With this, functions/methods/attributes that receive or return a VirtualObj may be exported
to script, without the scripts never knowing what a VirtualObj is.
Besides that, the VirtualObj may contain any value it can hold - be it a script object,
instance of a C++ exported class, proxy class instance and on and on...
*/


#ifdef SWIGLUA
%{
#include <module/luaproxy.h>
%}

/******
YO WIL
implemente aqui os typemaps pra VirtualObj
codigo deles deve ser bem parecido com o existente no proxy.swig
******/

%define virtual_class()
/*VObj* typemaps*/
%typemap(in) ugdk::script::VirtualObj* {

}
%typemap(out) ugdk::script::VirtualObj* {

}
/*VObj& typemaps*/
%typemap(in) ugdk::script::VirtualObj& {

}
%typemap(out) ugdk::script::VirtualObj& {

}
/*const VObj& typemaps*/
%typemap(in) const ugdk::script::VirtualObj& {

}
%typemap(out) const ugdk::script::VirtualObj& {

}
/*VObj typemaps*/
%typemap(in) ugdk::script::VirtualObj {

}
%typemap(out) ugdk::script::VirtualObj {

}
%enddef
#elif SWIGPYTHON
%define virtual_class()
/*VObj* typemaps*/
%typemap(in) ugdk::script::VirtualObj* {
    /*converting script object to C++ VObj* ... Just encapsulate the script object in a VObj =D */
    Py_INCREF($input);
    ugdk::script::python::PythonWrapper* wrapper = static_cast<ugdk::script::python::PythonWrapper*>( SCRIPT_MANAGER()->GetWrapper("Python") );
    ugdk::script::VirtualData::Ptr vdata( new ugdk::script::python::PythonData(wrapper, $input, true) ); 
    $1 = new ugdk::script::VirtualObj(vdata);
}
%typemap(out) ugdk::script::VirtualObj* {
    /*Sending VObj to script... Just extract the contained script object and pass it on =D */
    if ($1) {
        $result = static_cast<PyObject*>(($1)->unsafe_data());
        if ($result == NULL)    $result = Py_None;
    }
    else {
        $result = Py_None;
    }
    Py_INCREF($result);
}
/*VObj& typemaps*/
%typemap(in) ugdk::script::VirtualObj& {
    Py_INCREF($input);
    ugdk::script::python::PythonWrapper* wrapper = static_cast<ugdk::script::python::PythonWrapper*>( SCRIPT_MANAGER()->GetWrapper("Python") );
    ugdk::script::VirtualData::Ptr vdata( new ugdk::script::python::PythonData(wrapper, $input, true) ); 
    $1 = new ugdk::script::VirtualObj(vdata);
}
%typemap(out) ugdk::script::VirtualObj& {
    if ($1) {
        $result = static_cast<PyObject*>(($1).unsafe_data());
        if ($result == NULL)    $result = Py_None;
    }
    else {
        $result = Py_None;
    }
    Py_INCREF($result);
}
/*const VObj& typemaps*/
%typemap(in) const ugdk::script::VirtualObj& {
    Py_INCREF($input);
    ugdk::script::python::PythonWrapper* wrapper = static_cast<ugdk::script::python::PythonWrapper*>( SCRIPT_MANAGER()->GetWrapper("Python") );
    ugdk::script::VirtualData::Ptr vdata( new ugdk::script::python::PythonData(wrapper, $input, true) ); 
    $1 = new ugdk::script::VirtualObj(vdata);
}
%typemap(out) const ugdk::script::VirtualObj& {
    if ($1) {
        $result = static_cast<PyObject*>(($1).unsafe_data());
        if ($result == NULL)    $result = Py_None;
    }
    else {
        $result = Py_None;
    }
    Py_INCREF($result);
}
/*VObj typemaps*/
%typemap(in) ugdk::script::VirtualObj {
    Py_INCREF($input);
    ugdk::script::python::PythonWrapper* wrapper = static_cast<ugdk::script::python::PythonWrapper*>( SCRIPT_MANAGER()->GetWrapper("Python") );
    ugdk::script::VirtualData::Ptr vdata( new ugdk::script::python::PythonData(wrapper, $input, true) ); 
    $1 = new ugdk::script::VirtualObj(vdata);
}
%typemap(out) ugdk::script::VirtualObj {
    if ($1) {
        $result = static_cast<PyObject*>(($1).unsafe_data());
        if ($result == NULL)    $result = Py_None;
    }
    else {
        $result = Py_None;
    }
    Py_INCREF($result);
}
%enddef
#endif

