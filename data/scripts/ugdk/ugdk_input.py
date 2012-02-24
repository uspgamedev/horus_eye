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
            fp, pathname, description = imp.find_module('_ugdk_input', [dirname(__file__)])
        except ImportError:
            import _ugdk_input
            return _ugdk_input
        if fp is not None:
            try:
                _mod = imp.load_module('_ugdk_input', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _ugdk_input = swig_import_helper()
    del swig_import_helper
else:
    import _ugdk_input
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
        this = _ugdk_input.new_VirtualType()
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _ugdk_input.delete_VirtualType
    __del__ = lambda self : None;
    def FromLang(self, *args): return _ugdk_input.VirtualType_FromLang(self, *args)
    def RegisterFromLang(self, *args): return _ugdk_input.VirtualType_RegisterFromLang(self, *args)
VirtualType_swigregister = _ugdk_input.VirtualType_swigregister
VirtualType_swigregister(VirtualType)
cvar = _ugdk_input.cvar
LuaLang = cvar.LuaLang
PythonLang = cvar.PythonLang

import ugdk_math
K_UNKNOWN = _ugdk_input.K_UNKNOWN
K_FIRST = _ugdk_input.K_FIRST
K_BACKSPACE = _ugdk_input.K_BACKSPACE
K_TAB = _ugdk_input.K_TAB
K_CLEAR = _ugdk_input.K_CLEAR
K_RETURN = _ugdk_input.K_RETURN
K_PAUSE = _ugdk_input.K_PAUSE
K_ESCAPE = _ugdk_input.K_ESCAPE
K_SPACE = _ugdk_input.K_SPACE
K_EXCLAIM = _ugdk_input.K_EXCLAIM
K_QUOTEDBL = _ugdk_input.K_QUOTEDBL
K_HASH = _ugdk_input.K_HASH
K_DOLLAR = _ugdk_input.K_DOLLAR
K_AMPERSAND = _ugdk_input.K_AMPERSAND
K_QUOTE = _ugdk_input.K_QUOTE
K_LEFTPAREN = _ugdk_input.K_LEFTPAREN
K_RIGHTPAREN = _ugdk_input.K_RIGHTPAREN
K_ASTERISK = _ugdk_input.K_ASTERISK
K_PLUS = _ugdk_input.K_PLUS
K_COMMA = _ugdk_input.K_COMMA
K_MINUS = _ugdk_input.K_MINUS
K_PERIOD = _ugdk_input.K_PERIOD
K_SLASH = _ugdk_input.K_SLASH
K_0 = _ugdk_input.K_0
K_1 = _ugdk_input.K_1
K_2 = _ugdk_input.K_2
K_3 = _ugdk_input.K_3
K_4 = _ugdk_input.K_4
K_5 = _ugdk_input.K_5
K_6 = _ugdk_input.K_6
K_7 = _ugdk_input.K_7
K_8 = _ugdk_input.K_8
K_9 = _ugdk_input.K_9
K_COLON = _ugdk_input.K_COLON
K_SEMICOLON = _ugdk_input.K_SEMICOLON
K_LESS = _ugdk_input.K_LESS
K_EQUALS = _ugdk_input.K_EQUALS
K_GREATER = _ugdk_input.K_GREATER
K_QUESTION = _ugdk_input.K_QUESTION
K_AT = _ugdk_input.K_AT
K_LEFTBRACKET = _ugdk_input.K_LEFTBRACKET
K_BACKSLASH = _ugdk_input.K_BACKSLASH
K_RIGHTBRACKET = _ugdk_input.K_RIGHTBRACKET
K_CARET = _ugdk_input.K_CARET
K_UNDERSCORE = _ugdk_input.K_UNDERSCORE
K_BACKQUOTE = _ugdk_input.K_BACKQUOTE
K_a = _ugdk_input.K_a
K_b = _ugdk_input.K_b
K_c = _ugdk_input.K_c
K_d = _ugdk_input.K_d
K_e = _ugdk_input.K_e
K_f = _ugdk_input.K_f
K_g = _ugdk_input.K_g
K_h = _ugdk_input.K_h
K_i = _ugdk_input.K_i
K_j = _ugdk_input.K_j
K_k = _ugdk_input.K_k
K_l = _ugdk_input.K_l
K_m = _ugdk_input.K_m
K_n = _ugdk_input.K_n
K_o = _ugdk_input.K_o
K_p = _ugdk_input.K_p
K_q = _ugdk_input.K_q
K_r = _ugdk_input.K_r
K_s = _ugdk_input.K_s
K_t = _ugdk_input.K_t
K_u = _ugdk_input.K_u
K_v = _ugdk_input.K_v
K_w = _ugdk_input.K_w
K_x = _ugdk_input.K_x
K_y = _ugdk_input.K_y
K_z = _ugdk_input.K_z
K_DELETE = _ugdk_input.K_DELETE
K_KP0 = _ugdk_input.K_KP0
K_KP1 = _ugdk_input.K_KP1
K_KP2 = _ugdk_input.K_KP2
K_KP3 = _ugdk_input.K_KP3
K_KP4 = _ugdk_input.K_KP4
K_KP5 = _ugdk_input.K_KP5
K_KP6 = _ugdk_input.K_KP6
K_KP7 = _ugdk_input.K_KP7
K_KP8 = _ugdk_input.K_KP8
K_KP9 = _ugdk_input.K_KP9
K_KP_PERIOD = _ugdk_input.K_KP_PERIOD
K_KP_DIVIDE = _ugdk_input.K_KP_DIVIDE
K_KP_MULTIPLY = _ugdk_input.K_KP_MULTIPLY
K_KP_MINUS = _ugdk_input.K_KP_MINUS
K_KP_PLUS = _ugdk_input.K_KP_PLUS
K_KP_ENTER = _ugdk_input.K_KP_ENTER
K_KP_EQUALS = _ugdk_input.K_KP_EQUALS
K_UP = _ugdk_input.K_UP
K_DOWN = _ugdk_input.K_DOWN
K_RIGHT = _ugdk_input.K_RIGHT
K_LEFT = _ugdk_input.K_LEFT
K_INSERT = _ugdk_input.K_INSERT
K_HOME = _ugdk_input.K_HOME
K_END = _ugdk_input.K_END
K_PAGEUP = _ugdk_input.K_PAGEUP
K_PAGEDOWN = _ugdk_input.K_PAGEDOWN
K_F1 = _ugdk_input.K_F1
K_F2 = _ugdk_input.K_F2
K_F3 = _ugdk_input.K_F3
K_F4 = _ugdk_input.K_F4
K_F5 = _ugdk_input.K_F5
K_F6 = _ugdk_input.K_F6
K_F7 = _ugdk_input.K_F7
K_F8 = _ugdk_input.K_F8
K_F9 = _ugdk_input.K_F9
K_F10 = _ugdk_input.K_F10
K_F11 = _ugdk_input.K_F11
K_F12 = _ugdk_input.K_F12
K_F13 = _ugdk_input.K_F13
K_F14 = _ugdk_input.K_F14
K_F15 = _ugdk_input.K_F15
K_NUMLOCK = _ugdk_input.K_NUMLOCK
K_CAPSLOCK = _ugdk_input.K_CAPSLOCK
K_SCROLLOCK = _ugdk_input.K_SCROLLOCK
K_RSHIFT = _ugdk_input.K_RSHIFT
K_LSHIFT = _ugdk_input.K_LSHIFT
K_RCTRL = _ugdk_input.K_RCTRL
K_LCTRL = _ugdk_input.K_LCTRL
K_RALT = _ugdk_input.K_RALT
K_LALT = _ugdk_input.K_LALT
K_RMETA = _ugdk_input.K_RMETA
K_LMETA = _ugdk_input.K_LMETA
K_LSUPER = _ugdk_input.K_LSUPER
K_RSUPER = _ugdk_input.K_RSUPER
K_MODE = _ugdk_input.K_MODE
K_COMPOSE = _ugdk_input.K_COMPOSE
K_HELP = _ugdk_input.K_HELP
K_PRINT = _ugdk_input.K_PRINT
K_SYSREQ = _ugdk_input.K_SYSREQ
K_BREAK = _ugdk_input.K_BREAK
K_MENU = _ugdk_input.K_MENU
K_POWER = _ugdk_input.K_POWER
K_EURO = _ugdk_input.K_EURO
K_UNDO = _ugdk_input.K_UNDO
K_LAST = _ugdk_input.K_LAST
M_BUTTON_LEFT = _ugdk_input.M_BUTTON_LEFT
M_BUTTON_MIDDLE = _ugdk_input.M_BUTTON_MIDDLE
M_BUTTON_RIGHT = _ugdk_input.M_BUTTON_RIGHT
M_BUTTON_WHEELUP = _ugdk_input.M_BUTTON_WHEELUP
M_BUTTON_WHEELDOWN = _ugdk_input.M_BUTTON_WHEELDOWN
BUFFER_SIZE = _ugdk_input.BUFFER_SIZE
class InputManager(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, InputManager, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, InputManager, name)
    __repr__ = _swig_repr
    def __init__(self): 
        this = _ugdk_input.new_InputManager()
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _ugdk_input.delete_InputManager
    __del__ = lambda self : None;
    def Update(self, *args): return _ugdk_input.InputManager_Update(self, *args)
    def GetMousePosition(self): return _ugdk_input.InputManager_GetMousePosition(self)
    def ShowCursor(self, *args): return _ugdk_input.InputManager_ShowCursor(self, *args)
    def KeyPressed(self, *args): return _ugdk_input.InputManager_KeyPressed(self, *args)
    def KeyDown(self, *args): return _ugdk_input.InputManager_KeyDown(self, *args)
    def KeyUp(self, *args): return _ugdk_input.InputManager_KeyUp(self, *args)
    def MousePressed(self, *args): return _ugdk_input.InputManager_MousePressed(self, *args)
    def MouseDown(self, *args): return _ugdk_input.InputManager_MouseDown(self, *args)
    def MouseUp(self, *args): return _ugdk_input.InputManager_MouseUp(self, *args)
    def CheckSequence(self, *args): return _ugdk_input.InputManager_CheckSequence(self, *args)
    def SimulateKeyPress(self, *args): return _ugdk_input.InputManager_SimulateKeyPress(self, *args)
    def SimulateKeyRelease(self, *args): return _ugdk_input.InputManager_SimulateKeyRelease(self, *args)
InputManager_swigregister = _ugdk_input.InputManager_swigregister
InputManager_swigregister(InputManager)


def RegisterType_Key(*args):
  return _ugdk_input.RegisterType_Key(*args)
RegisterType_Key = _ugdk_input.RegisterType_Key

def GetNull_Key():
  return _ugdk_input.GetNull_Key()
GetNull_Key = _ugdk_input.GetNull_Key

def RegisterType_MouseButton(*args):
  return _ugdk_input.RegisterType_MouseButton(*args)
RegisterType_MouseButton = _ugdk_input.RegisterType_MouseButton

def GetNull_MouseButton():
  return _ugdk_input.GetNull_MouseButton()
GetNull_MouseButton = _ugdk_input.GetNull_MouseButton

def RegisterType_InputManager(*args):
  return _ugdk_input.RegisterType_InputManager(*args)
RegisterType_InputManager = _ugdk_input.RegisterType_InputManager

def GetNull_InputManager():
  return _ugdk_input.GetNull_InputManager()
GetNull_InputManager = _ugdk_input.GetNull_InputManager
print "Python ugdk_input confirm exports!"
module = __import__(__name__)
for key,value in module.__dict__.items():
	if key.startswith("RegisterType_"):
		arg = module.__dict__["GetNull_"+key[13:] ]()
		value(arg)

# This file is compatible with both classic and new-style classes.


