
#ifndef HORUSEYE_MODULES_H_
#define HORUSEYE_MODULES_H_

#include <ugdk/script.h>

/// Loads all Horus's Lua modules using the given wrapper.
/**
 ** @param wrapper - The wrapper used to register the modules.
 */
void RegisterLuaModules(ugdk::script::lua::LuaWrapper* wrapper);

/// Loads all Horus's Python modules using the given wrapper.
/**
 ** @param wrapper - The wrapper used to register the modules.
 */
void RegisterPythonModules(ugdk::script::python::PythonWrapper* wrapper);

#endif /* HORUSEYE_MODULES_H_ */
