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
            fp, pathname, description = imp.find_module('_ugdk_math', [dirname(__file__)])
        except ImportError:
            import _ugdk_math
            return _ugdk_math
        if fp is not None:
            try:
                _mod = imp.load_module('_ugdk_math', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _ugdk_math = swig_import_helper()
    del swig_import_helper
else:
    import _ugdk_math
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
        this = _ugdk_math.new_VirtualType()
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _ugdk_math.delete_VirtualType
    __del__ = lambda self : None;
    def FromLang(self, *args): return _ugdk_math.VirtualType_FromLang(self, *args)
    def RegisterFromLang(self, *args): return _ugdk_math.VirtualType_RegisterFromLang(self, *args)
VirtualType_swigregister = _ugdk_math.VirtualType_swigregister
VirtualType_swigregister(VirtualType)
cvar = _ugdk_math.cvar
LuaLang = cvar.LuaLang
PythonLang = cvar.PythonLang

class Vector2D(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, Vector2D, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, Vector2D, name)
    __repr__ = _swig_repr
    def __init__(self, *args): 
        this = _ugdk_math.new_Vector2D(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _ugdk_math.delete_Vector2D
    __del__ = lambda self : None;
    def get_x(self): return _ugdk_math.Vector2D_get_x(self)
    def get_y(self): return _ugdk_math.Vector2D_get_y(self)
    def set_x(self, *args): return _ugdk_math.Vector2D_set_x(self, *args)
    def set_y(self, *args): return _ugdk_math.Vector2D_set_y(self, *args)
    def NormOne(self): return _ugdk_math.Vector2D_NormOne(self)
    def Length(self): return _ugdk_math.Vector2D_Length(self)
    def LengthSquared(self): return _ugdk_math.Vector2D_LengthSquared(self)
    def Angle(self): return _ugdk_math.Vector2D_Angle(self)
    def Normalize(self): return _ugdk_math.Vector2D_Normalize(self)
    def Rotate(self, *args): return _ugdk_math.Vector2D_Rotate(self, *args)
    def Scale(self, *args): return _ugdk_math.Vector2D_Scale(self, *args)
    __swig_getmethods__["Add"] = lambda x: _ugdk_math.Vector2D_Add
    if _newclass:Add = staticmethod(_ugdk_math.Vector2D_Add)
    __swig_getmethods__["Subtract"] = lambda x: _ugdk_math.Vector2D_Subtract
    if _newclass:Subtract = staticmethod(_ugdk_math.Vector2D_Subtract)
    __swig_getmethods__["Multiply"] = lambda x: _ugdk_math.Vector2D_Multiply
    if _newclass:Multiply = staticmethod(_ugdk_math.Vector2D_Multiply)
    __swig_getmethods__["InnerProduct"] = lambda x: _ugdk_math.Vector2D_InnerProduct
    if _newclass:InnerProduct = staticmethod(_ugdk_math.Vector2D_InnerProduct)
    def length(self): return _ugdk_math.Vector2D_length(self)
    def angle(self): return _ugdk_math.Vector2D_angle(self)
    __swig_getmethods__["Normalized"] = lambda x: _ugdk_math.Vector2D_Normalized
    if _newclass:Normalized = staticmethod(_ugdk_math.Vector2D_Normalized)
    def __iadd__(self, *args): return _ugdk_math.Vector2D___iadd__(self, *args)
    def __isub__(self, *args): return _ugdk_math.Vector2D___isub__(self, *args)
    def __add__(self, *args): return _ugdk_math.Vector2D___add__(self, *args)
    def __neg__(self): return _ugdk_math.Vector2D___neg__(self)
    def __sub__(self, *args): return _ugdk_math.Vector2D___sub__(self, *args)
    def __div__(self, *args): return _ugdk_math.Vector2D___div__(self, *args)
    def __mul__(self, *args): return _ugdk_math.Vector2D___mul__(self, *args)
Vector2D_swigregister = _ugdk_math.Vector2D_swigregister
Vector2D_swigregister(Vector2D)

def Vector2D_Add(*args):
  return _ugdk_math.Vector2D_Add(*args)
Vector2D_Add = _ugdk_math.Vector2D_Add

def Vector2D_Subtract(*args):
  return _ugdk_math.Vector2D_Subtract(*args)
Vector2D_Subtract = _ugdk_math.Vector2D_Subtract

def Vector2D_Multiply(*args):
  return _ugdk_math.Vector2D_Multiply(*args)
Vector2D_Multiply = _ugdk_math.Vector2D_Multiply

def Vector2D_InnerProduct(*args):
  return _ugdk_math.Vector2D_InnerProduct(*args)
Vector2D_InnerProduct = _ugdk_math.Vector2D_InnerProduct

def Vector2D_Normalized(*args):
  return _ugdk_math.Vector2D_Normalized(*args)
Vector2D_Normalized = _ugdk_math.Vector2D_Normalized

class Frame(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, Frame, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, Frame, name)
    __repr__ = _swig_repr
    def __init__(self, *args): 
        this = _ugdk_math.new_Frame(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _ugdk_math.delete_Frame
    __del__ = lambda self : None;
    def width(self): return _ugdk_math.Frame_width(self)
    def height(self): return _ugdk_math.Frame_height(self)
    def left(self): return _ugdk_math.Frame_left(self)
    def set_left(self, *args): return _ugdk_math.Frame_set_left(self, *args)
    def top(self): return _ugdk_math.Frame_top(self)
    def set_top(self, *args): return _ugdk_math.Frame_set_top(self, *args)
    def right(self): return _ugdk_math.Frame_right(self)
    def set_right(self, *args): return _ugdk_math.Frame_set_right(self, *args)
    def bottom(self): return _ugdk_math.Frame_bottom(self)
    def set_bottom(self, *args): return _ugdk_math.Frame_set_bottom(self, *args)
    def Collides(self, *args): return _ugdk_math.Frame_Collides(self, *args)
Frame_swigregister = _ugdk_math.Frame_swigregister
Frame_swigregister(Frame)


def RegisterType_Vector2D(*args):
  return _ugdk_math.RegisterType_Vector2D(*args)
RegisterType_Vector2D = _ugdk_math.RegisterType_Vector2D

def GetNull_Vector2D():
  return _ugdk_math.GetNull_Vector2D()
GetNull_Vector2D = _ugdk_math.GetNull_Vector2D

def RegisterType_Frame(*args):
  return _ugdk_math.RegisterType_Frame(*args)
RegisterType_Frame = _ugdk_math.RegisterType_Frame

def GetNull_Frame():
  return _ugdk_math.GetNull_Frame()
GetNull_Frame = _ugdk_math.GetNull_Frame
print "Python ugdk_math confirm exports!"
module = __import__(__name__)
for key,value in module.__dict__.items():
	if key.startswith("RegisterType_"):
		arg = module.__dict__["GetNull_"+key[13:] ]()
		value(arg)

# This file is compatible with both classic and new-style classes.


