# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.5
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
            fp, pathname, description = imp.find_module('_pyramidworks_geometry', [dirname(__file__)])
        except ImportError:
            import _pyramidworks_geometry
            return _pyramidworks_geometry
        if fp is not None:
            try:
                _mod = imp.load_module('_pyramidworks_geometry', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _pyramidworks_geometry = swig_import_helper()
    del swig_import_helper
else:
    import _pyramidworks_geometry
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


import ugdk_math
import ugdk_util
import ugdk_action
import ugdk_gdd
import ugdk_graphic
class GeometricShape(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, GeometricShape, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, GeometricShape, name)
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _pyramidworks_geometry.delete_GeometricShape
    __del__ = lambda self : None;
    def Intersects(self, *args): return _pyramidworks_geometry.GeometricShape_Intersects(self, *args)
    def GetBoundingBox(self, *args): return _pyramidworks_geometry.GeometricShape_GetBoundingBox(self, *args)
GeometricShape_swigregister = _pyramidworks_geometry.GeometricShape_swigregister
GeometricShape_swigregister(GeometricShape)
cvar = _pyramidworks_geometry.cvar
LuaLang = cvar.LuaLang
PythonLang = cvar.PythonLang

class Circle(GeometricShape):
    __swig_setmethods__ = {}
    for _s in [GeometricShape]: __swig_setmethods__.update(getattr(_s,'__swig_setmethods__',{}))
    __setattr__ = lambda self, name, value: _swig_setattr(self, Circle, name, value)
    __swig_getmethods__ = {}
    for _s in [GeometricShape]: __swig_getmethods__.update(getattr(_s,'__swig_getmethods__',{}))
    __getattr__ = lambda self, name: _swig_getattr(self, Circle, name)
    __repr__ = _swig_repr
    def __init__(self, *args): 
        this = _pyramidworks_geometry.new_Circle(*args)
        try: self.this.append(this)
        except: self.this = this
    def radius(self): return _pyramidworks_geometry.Circle_radius(self)
    def set_radius(self, *args): return _pyramidworks_geometry.Circle_set_radius(self, *args)
    def Intersects(self, *args): return _pyramidworks_geometry.Circle_Intersects(self, *args)
    def GetBoundingBox(self, *args): return _pyramidworks_geometry.Circle_GetBoundingBox(self, *args)
    __swig_destroy__ = _pyramidworks_geometry.delete_Circle
    __del__ = lambda self : None;
Circle_swigregister = _pyramidworks_geometry.Circle_swigregister
Circle_swigregister(Circle)

class Rect(GeometricShape):
    __swig_setmethods__ = {}
    for _s in [GeometricShape]: __swig_setmethods__.update(getattr(_s,'__swig_setmethods__',{}))
    __setattr__ = lambda self, name, value: _swig_setattr(self, Rect, name, value)
    __swig_getmethods__ = {}
    for _s in [GeometricShape]: __swig_getmethods__.update(getattr(_s,'__swig_getmethods__',{}))
    __getattr__ = lambda self, name: _swig_getattr(self, Rect, name)
    __repr__ = _swig_repr
    def __init__(self, *args): 
        this = _pyramidworks_geometry.new_Rect(*args)
        try: self.this.append(this)
        except: self.this = this
    def width(self): return _pyramidworks_geometry.Rect_width(self)
    def height(self): return _pyramidworks_geometry.Rect_height(self)
    def Intersects(self, *args): return _pyramidworks_geometry.Rect_Intersects(self, *args)
    def GetBoundingBox(self, *args): return _pyramidworks_geometry.Rect_GetBoundingBox(self, *args)
    __swig_destroy__ = _pyramidworks_geometry.delete_Rect
    __del__ = lambda self : None;
Rect_swigregister = _pyramidworks_geometry.Rect_swigregister
Rect_swigregister(Rect)


def RegisterType_GeometricShape(*args):
  return _pyramidworks_geometry.RegisterType_GeometricShape(*args)
RegisterType_GeometricShape = _pyramidworks_geometry.RegisterType_GeometricShape

def GetNull_GeometricShape():
  return _pyramidworks_geometry.GetNull_GeometricShape()
GetNull_GeometricShape = _pyramidworks_geometry.GetNull_GeometricShape

def RegisterType_Circle(*args):
  return _pyramidworks_geometry.RegisterType_Circle(*args)
RegisterType_Circle = _pyramidworks_geometry.RegisterType_Circle

def GetNull_Circle():
  return _pyramidworks_geometry.GetNull_Circle()
GetNull_Circle = _pyramidworks_geometry.GetNull_Circle

def RegisterType_Rect(*args):
  return _pyramidworks_geometry.RegisterType_Rect(*args)
RegisterType_Rect = _pyramidworks_geometry.RegisterType_Rect

def GetNull_Rect():
  return _pyramidworks_geometry.GetNull_Rect()
GetNull_Rect = _pyramidworks_geometry.GetNull_Rect
print "Python pyramidworks_geometry confirm exports!"
module = __import__(__name__)
for key,value in module.__dict__.items():
	if key.startswith("RegisterType_"):
		arg = module.__dict__["GetNull_"+key[13:] ]()
		value(arg)

# This file is compatible with both classic and new-style classes.


