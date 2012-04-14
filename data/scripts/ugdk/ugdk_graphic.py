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


class SwigPyIterator(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, SwigPyIterator, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, SwigPyIterator, name)
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _ugdk_graphic.delete_SwigPyIterator
    __del__ = lambda self : None;
    def value(self): return _ugdk_graphic.SwigPyIterator_value(self)
    def incr(self, n = 1): return _ugdk_graphic.SwigPyIterator_incr(self, n)
    def decr(self, n = 1): return _ugdk_graphic.SwigPyIterator_decr(self, n)
    def distance(self, *args): return _ugdk_graphic.SwigPyIterator_distance(self, *args)
    def equal(self, *args): return _ugdk_graphic.SwigPyIterator_equal(self, *args)
    def copy(self): return _ugdk_graphic.SwigPyIterator_copy(self)
    def next(self): return _ugdk_graphic.SwigPyIterator_next(self)
    def __next__(self): return _ugdk_graphic.SwigPyIterator___next__(self)
    def previous(self): return _ugdk_graphic.SwigPyIterator_previous(self)
    def advance(self, *args): return _ugdk_graphic.SwigPyIterator_advance(self, *args)
    def __eq__(self, *args): return _ugdk_graphic.SwigPyIterator___eq__(self, *args)
    def __ne__(self, *args): return _ugdk_graphic.SwigPyIterator___ne__(self, *args)
    def __iadd__(self, *args): return _ugdk_graphic.SwigPyIterator___iadd__(self, *args)
    def __isub__(self, *args): return _ugdk_graphic.SwigPyIterator___isub__(self, *args)
    def __add__(self, *args): return _ugdk_graphic.SwigPyIterator___add__(self, *args)
    def __sub__(self, *args): return _ugdk_graphic.SwigPyIterator___sub__(self, *args)
    def __iter__(self): return self
SwigPyIterator_swigregister = _ugdk_graphic.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)
cvar = _ugdk_graphic.cvar
LuaLang = cvar.LuaLang
PythonLang = cvar.PythonLang

import ugdk_math
class Drawable(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, Drawable, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, Drawable, name)
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    TOP_LEFT = _ugdk_graphic.Drawable_TOP_LEFT
    TOP = _ugdk_graphic.Drawable_TOP
    TOP_RIGHT = _ugdk_graphic.Drawable_TOP_RIGHT
    LEFT = _ugdk_graphic.Drawable_LEFT
    CENTER = _ugdk_graphic.Drawable_CENTER
    RIGHT = _ugdk_graphic.Drawable_RIGHT
    BOTTOM_LEFT = _ugdk_graphic.Drawable_BOTTOM_LEFT
    BOTTOM = _ugdk_graphic.Drawable_BOTTOM
    BOTTOM_RIGHT = _ugdk_graphic.Drawable_BOTTOM_RIGHT
    __swig_destroy__ = _ugdk_graphic.delete_Drawable
    __del__ = lambda self : None;
    def Draw(self, *args): return _ugdk_graphic.Drawable_Draw(self, *args)
    def size(self): return _ugdk_graphic.Drawable_size(self)
    def set_hotspot(self, *args): return _ugdk_graphic.Drawable_set_hotspot(self, *args)
    def width(self): return _ugdk_graphic.Drawable_width(self)
    def height(self): return _ugdk_graphic.Drawable_height(self)
    def hotspot(self): return _ugdk_graphic.Drawable_hotspot(self)
Drawable_swigregister = _ugdk_graphic.Drawable_swigregister
Drawable_swigregister(Drawable)

import ugdk_base
import ugdk_action
import ugdk_drawable
class Texture(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, Texture, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, Texture, name)
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined")
    __repr__ = _swig_repr
    __swig_destroy__ = _ugdk_graphic.delete_Texture
    __del__ = lambda self : None;
    __swig_getmethods__["CreateFromFile"] = lambda x: _ugdk_graphic.Texture_CreateFromFile
    if _newclass:CreateFromFile = staticmethod(_ugdk_graphic.Texture_CreateFromFile)
    __swig_getmethods__["CreateRawTexture"] = lambda x: _ugdk_graphic.Texture_CreateRawTexture
    if _newclass:CreateRawTexture = staticmethod(_ugdk_graphic.Texture_CreateRawTexture)
    def gltexture(self): return _ugdk_graphic.Texture_gltexture(self)
    def width(self): return _ugdk_graphic.Texture_width(self)
    def height(self): return _ugdk_graphic.Texture_height(self)
Texture_swigregister = _ugdk_graphic.Texture_swigregister
Texture_swigregister(Texture)

def Texture_CreateFromFile(*args):
  return _ugdk_graphic.Texture_CreateFromFile(*args)
Texture_CreateFromFile = _ugdk_graphic.Texture_CreateFromFile

def Texture_CreateRawTexture(*args):
  return _ugdk_graphic.Texture_CreateRawTexture(*args)
Texture_CreateRawTexture = _ugdk_graphic.Texture_CreateRawTexture

class Font(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, Font, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, Font, name)
    __repr__ = _swig_repr
    LEFT = _ugdk_graphic.Font_LEFT
    CENTER = _ugdk_graphic.Font_CENTER
    RIGHT = _ugdk_graphic.Font_RIGHT
    def __init__(self, *args): 
        this = _ugdk_graphic.new_Font(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _ugdk_graphic.delete_Font
    __del__ = lambda self : None;
    def id(self): return _ugdk_graphic.Font_id(self)
    def ident(self): return _ugdk_graphic.Font_ident(self)
    def IsFancy(self): return _ugdk_graphic.Font_IsFancy(self)
Font_swigregister = _ugdk_graphic.Font_swigregister
Font_swigregister(Font)

class Light(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, Light, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, Light, name)
    __repr__ = _swig_repr
    def __init__(self): 
        this = _ugdk_graphic.new_Light()
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _ugdk_graphic.delete_Light
    __del__ = lambda self : None;
    def dimension(self): return _ugdk_graphic.Light_dimension(self)
    def set_dimension(self, *args): return _ugdk_graphic.Light_set_dimension(self, *args)
    def color(self): return _ugdk_graphic.Light_color(self)
    def set_color(self, *args): return _ugdk_graphic.Light_set_color(self, *args)
    def Draw(self): return _ugdk_graphic.Light_Draw(self)
Light_swigregister = _ugdk_graphic.Light_swigregister
Light_swigregister(Light)

class Modifier(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, Modifier, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, Modifier, name)
    __repr__ = _swig_repr
    NOTHING = _ugdk_graphic.Modifier_NOTHING
    HAS_TRANSFORMATION = _ugdk_graphic.Modifier_HAS_TRANSFORMATION
    HAS_COLOR = _ugdk_graphic.Modifier_HAS_COLOR
    TRUNCATES_WHEN_APPLIED = _ugdk_graphic.Modifier_TRUNCATES_WHEN_APPLIED
    def __init__(self, *args): 
        this = _ugdk_graphic.new_Modifier(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _ugdk_graphic.delete_Modifier
    __del__ = lambda self : None;
    def offset(self): return _ugdk_graphic.Modifier_offset(self)
    def scale(self): return _ugdk_graphic.Modifier_scale(self)
    def rotation(self): return _ugdk_graphic.Modifier_rotation(self)
    def mirror(self): return _ugdk_graphic.Modifier_mirror(self)
    def color(self): return _ugdk_graphic.Modifier_color(self)
    def alpha(self): return _ugdk_graphic.Modifier_alpha(self)
    def flags(self): return _ugdk_graphic.Modifier_flags(self)
    def visible(self): return _ugdk_graphic.Modifier_visible(self)
    def set_offset(self, *args): return _ugdk_graphic.Modifier_set_offset(self, *args)
    def set_scale(self, *args): return _ugdk_graphic.Modifier_set_scale(self, *args)
    def set_rotation(self, *args): return _ugdk_graphic.Modifier_set_rotation(self, *args)
    def set_mirror(self, *args): return _ugdk_graphic.Modifier_set_mirror(self, *args)
    def set_color(self, *args): return _ugdk_graphic.Modifier_set_color(self, *args)
    def set_alpha(self, *args): return _ugdk_graphic.Modifier_set_alpha(self, *args)
    def set_visible(self, *args): return _ugdk_graphic.Modifier_set_visible(self, *args)
    def ToggleFlag(self, *args): return _ugdk_graphic.Modifier_ToggleFlag(self, *args)
    def ComposeAlpha(self, *args): return _ugdk_graphic.Modifier_ComposeAlpha(self, *args)
    def ComposeOffset(self, *args): return _ugdk_graphic.Modifier_ComposeOffset(self, *args)
    def ComposeScale(self, *args): return _ugdk_graphic.Modifier_ComposeScale(self, *args)
    def ComposeRotation(self, *args): return _ugdk_graphic.Modifier_ComposeRotation(self, *args)
    def ComposeMirror(self, *args): return _ugdk_graphic.Modifier_ComposeMirror(self, *args)
    def ComposeColor(self, *args): return _ugdk_graphic.Modifier_ComposeColor(self, *args)
    def ComposeVisible(self, *args): return _ugdk_graphic.Modifier_ComposeVisible(self, *args)
    def Compose(self, *args): return _ugdk_graphic.Modifier_Compose(self, *args)
    __swig_getmethods__["ComposeNew"] = lambda x: _ugdk_graphic.Modifier_ComposeNew
    if _newclass:ComposeNew = staticmethod(_ugdk_graphic.Modifier_ComposeNew)
    __swig_getmethods__["Copy"] = lambda x: _ugdk_graphic.Modifier_Copy
    if _newclass:Copy = staticmethod(_ugdk_graphic.Modifier_Copy)
Modifier_swigregister = _ugdk_graphic.Modifier_swigregister
Modifier_swigregister(Modifier)

def Modifier_ComposeNew(*args):
  return _ugdk_graphic.Modifier_ComposeNew(*args)
Modifier_ComposeNew = _ugdk_graphic.Modifier_ComposeNew

def Modifier_Copy(*args):
  return _ugdk_graphic.Modifier_Copy(*args)
Modifier_Copy = _ugdk_graphic.Modifier_Copy
Modifier.IDENTITY = _ugdk_graphic.cvar.Modifier_IDENTITY

class Node(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, Node, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, Node, name)
    __repr__ = _swig_repr
    def __init__(self, drawable = None, modifier = None): 
        this = _ugdk_graphic.new_Node(drawable, modifier)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _ugdk_graphic.delete_Node
    __del__ = lambda self : None;
    def Render(self, *args): return _ugdk_graphic.Node_Render(self, *args)
    def RenderLight(self): return _ugdk_graphic.Node_RenderLight(self)
    def set_drawable(self, *args): return _ugdk_graphic.Node_set_drawable(self, *args)
    def set_light(self, *args): return _ugdk_graphic.Node_set_light(self, *args)
    def set_active(self, *args): return _ugdk_graphic.Node_set_active(self, *args)
    def set_zindex(self, *args): return _ugdk_graphic.Node_set_zindex(self, *args)
    def modifier(self, *args): return _ugdk_graphic.Node_modifier(self, *args)
    def drawable(self, *args): return _ugdk_graphic.Node_drawable(self, *args)
    def light(self, *args): return _ugdk_graphic.Node_light(self, *args)
    def active(self): return _ugdk_graphic.Node_active(self)
    def zindex(self): return _ugdk_graphic.Node_zindex(self)
    def AddChild(self, *args): return _ugdk_graphic.Node_AddChild(self, *args)
    def RemoveChild(self, *args): return _ugdk_graphic.Node_RemoveChild(self, *args)
    __swig_getmethods__["CompareByZIndex"] = lambda x: _ugdk_graphic.Node_CompareByZIndex
    if _newclass:CompareByZIndex = staticmethod(_ugdk_graphic.Node_CompareByZIndex)
    def SortChildren(self): return _ugdk_graphic.Node_SortChildren(self)
Node_swigregister = _ugdk_graphic.Node_swigregister
Node_swigregister(Node)

def Node_CompareByZIndex(*args):
  return _ugdk_graphic.Node_CompareByZIndex(*args)
Node_CompareByZIndex = _ugdk_graphic.Node_CompareByZIndex

class Spritesheet(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, Spritesheet, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, Spritesheet, name)
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _ugdk_graphic.delete_Spritesheet
    __del__ = lambda self : None;
    def Draw(self, *args): return _ugdk_graphic.Spritesheet_Draw(self, *args)
    def frame_count(self): return _ugdk_graphic.Spritesheet_frame_count(self)
    def frame_size(self, *args): return _ugdk_graphic.Spritesheet_frame_size(self, *args)
Spritesheet_swigregister = _ugdk_graphic.Spritesheet_swigregister
Spritesheet_swigregister(Spritesheet)

class TextManager(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, TextManager, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, TextManager, name)
    __repr__ = _swig_repr
    def __init__(self): 
        this = _ugdk_graphic.new_TextManager()
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _ugdk_graphic.delete_TextManager
    __del__ = lambda self : None;
    def Initialize(self): return _ugdk_graphic.TextManager_Initialize(self)
    def Release(self): return _ugdk_graphic.TextManager_Release(self)
    def GetText(self, *args): return _ugdk_graphic.TextManager_GetText(self, *args)
    def GetTextFromFile(self, *args): return _ugdk_graphic.TextManager_GetTextFromFile(self, *args)
    def AddFont(self, *args): return _ugdk_graphic.TextManager_AddFont(self, *args)
TextManager_swigregister = _ugdk_graphic.TextManager_swigregister
TextManager_swigregister(TextManager)

class VideoManager(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, VideoManager, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, VideoManager, name)
    __repr__ = _swig_repr
    COLOR_DEPTH = _ugdk_graphic.VideoManager_COLOR_DEPTH
    def __init__(self): 
        this = _ugdk_graphic.new_VideoManager()
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _ugdk_graphic.delete_VideoManager
    __del__ = lambda self : None;
    def Initialize(self, *args): return _ugdk_graphic.VideoManager_Initialize(self, *args)
    def Release(self): return _ugdk_graphic.VideoManager_Release(self)
    def Render(self, *args): return _ugdk_graphic.VideoManager_Render(self, *args)
    def ChangeResolution(self, *args): return _ugdk_graphic.VideoManager_ChangeResolution(self, *args)
    def SetVSync(self, *args): return _ugdk_graphic.VideoManager_SetVSync(self, *args)
    def SetLightSystem(self, *args): return _ugdk_graphic.VideoManager_SetLightSystem(self, *args)
    def video_size(self): return _ugdk_graphic.VideoManager_video_size(self)
    def fullscreen(self): return _ugdk_graphic.VideoManager_fullscreen(self)
    def title(self): return _ugdk_graphic.VideoManager_title(self)
    def light_texture(self): return _ugdk_graphic.VideoManager_light_texture(self)
    def virtual_bounds(self): return _ugdk_graphic.VideoManager_virtual_bounds(self)
    def PushAndApplyModifier(self, *args): return _ugdk_graphic.VideoManager_PushAndApplyModifier(self, *args)
    def PopModifier(self): return _ugdk_graphic.VideoManager_PopModifier(self)
    def CurrentModifier(self): return _ugdk_graphic.VideoManager_CurrentModifier(self)
VideoManager_swigregister = _ugdk_graphic.VideoManager_swigregister
VideoManager_swigregister(VideoManager)


def RegisterType_Drawable(*args):
  return _ugdk_graphic.RegisterType_Drawable(*args)
RegisterType_Drawable = _ugdk_graphic.RegisterType_Drawable

def GetNull_Drawable():
  return _ugdk_graphic.GetNull_Drawable()
GetNull_Drawable = _ugdk_graphic.GetNull_Drawable

def RegisterType_Texture(*args):
  return _ugdk_graphic.RegisterType_Texture(*args)
RegisterType_Texture = _ugdk_graphic.RegisterType_Texture

def GetNull_Texture():
  return _ugdk_graphic.GetNull_Texture()
GetNull_Texture = _ugdk_graphic.GetNull_Texture

def RegisterType_Font(*args):
  return _ugdk_graphic.RegisterType_Font(*args)
RegisterType_Font = _ugdk_graphic.RegisterType_Font

def GetNull_Font():
  return _ugdk_graphic.GetNull_Font()
GetNull_Font = _ugdk_graphic.GetNull_Font

def RegisterType_Light(*args):
  return _ugdk_graphic.RegisterType_Light(*args)
RegisterType_Light = _ugdk_graphic.RegisterType_Light

def GetNull_Light():
  return _ugdk_graphic.GetNull_Light()
GetNull_Light = _ugdk_graphic.GetNull_Light

def RegisterType_Modifier(*args):
  return _ugdk_graphic.RegisterType_Modifier(*args)
RegisterType_Modifier = _ugdk_graphic.RegisterType_Modifier

def GetNull_Modifier():
  return _ugdk_graphic.GetNull_Modifier()
GetNull_Modifier = _ugdk_graphic.GetNull_Modifier

def RegisterType_Node(*args):
  return _ugdk_graphic.RegisterType_Node(*args)
RegisterType_Node = _ugdk_graphic.RegisterType_Node

def GetNull_Node():
  return _ugdk_graphic.GetNull_Node()
GetNull_Node = _ugdk_graphic.GetNull_Node

def RegisterType_Spritesheet(*args):
  return _ugdk_graphic.RegisterType_Spritesheet(*args)
RegisterType_Spritesheet = _ugdk_graphic.RegisterType_Spritesheet

def GetNull_Spritesheet():
  return _ugdk_graphic.GetNull_Spritesheet()
GetNull_Spritesheet = _ugdk_graphic.GetNull_Spritesheet

def RegisterType_TextManager(*args):
  return _ugdk_graphic.RegisterType_TextManager(*args)
RegisterType_TextManager = _ugdk_graphic.RegisterType_TextManager

def GetNull_TextManager():
  return _ugdk_graphic.GetNull_TextManager()
GetNull_TextManager = _ugdk_graphic.GetNull_TextManager

def RegisterType_VideoManager(*args):
  return _ugdk_graphic.RegisterType_VideoManager(*args)
RegisterType_VideoManager = _ugdk_graphic.RegisterType_VideoManager

def GetNull_VideoManager():
  return _ugdk_graphic.GetNull_VideoManager()
GetNull_VideoManager = _ugdk_graphic.GetNull_VideoManager
print "Python ugdk_graphic confirm exports!"
module = __import__(__name__)
for key,value in module.__dict__.items():
	if key.startswith("RegisterType_"):
		arg = module.__dict__["GetNull_"+key[13:] ]()
		value(arg)

# This file is compatible with both classic and new-style classes.

