# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.4
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_ugdk_graphic', [dirname(__file__)])
        except ImportError:
            import _ugdk_graphic
            return _ugdk_graphic
        if fp is not None:
            try:
                _mod = imp.load_module('_ugdk_graphic', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _ugdk_graphic = swig_import_helper()
    del swig_import_helper
else:
    import _ugdk_graphic
del version_info
try:
    _swig_property = property
except NameError:
    pass # Python < 2.2 doesn't have 'property'.
def _swig_setattr_nondynamic(self,class_type,name,value,static=1):
    if (name == "thisown"): return self.this.own(value)
    if (name == "this"):
        if type(value).__name__ == 'SwigPyObject':
            self.__dict__[name] = value
            return
    method = class_type.__swig_setmethods__.get(name,None)
    if method: return method(self,value)
    if (not static):
        self.__dict__[name] = value
    else:
        raise AttributeError("You cannot add attributes to %s" % self)

def _swig_setattr(self,class_type,name,value):
    return _swig_setattr_nondynamic(self,class_type,name,value,0)

def _swig_getattr(self,class_type,name):
    if (name == "thisown"): return self.this.own()
    method = class_type.__swig_getmethods__.get(name,None)
    if method: return method(self)
    raise AttributeError(name)

def _swig_repr(self):
    try: strthis = "proxy of " + self.this.__repr__()
    except: strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)

try:
    _object = object
    _newclass = 1
except AttributeError:
    class _object : pass
    _newclass = 0


class VirtualType(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, VirtualType, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, VirtualType, name)
    __repr__ = _swig_repr
    def __init__(self): 
        this = _ugdk_graphic.new_VirtualType()
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _ugdk_graphic.delete_VirtualType
    __del__ = lambda self : None;
    def FromLang(self, *args): return _ugdk_graphic.VirtualType_FromLang(self, *args)
    def RegisterFromLang(self, *args): return _ugdk_graphic.VirtualType_RegisterFromLang(self, *args)
VirtualType_swigregister = _ugdk_graphic.VirtualType_swigregister
VirtualType_swigregister(VirtualType)
cvar = _ugdk_graphic.cvar
LuaLang = cvar.LuaLang
PythonLang = cvar.PythonLang

print "Python ugdk_graphic confirm exports!"
module = __import__(__name__)
for key,value in module.__dict__.items():
	if key.startswith("RegisterType_"):
		arg = module.__dict__["GetNull_"+key[13:] ]()
		value(arg)

# This file is compatible with both classic and new-style classes.


