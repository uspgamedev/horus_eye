
#include <cstdio>

#include <ugdk/script/module.h>

#include <ugdk/script/languages/lua/luawrapper.h>
#include <ugdk/script/languages/python/pythonwrapper.h>

#define HORUS_MODULES_NUM 2

#define HORUS_MODULES_LIST(ACTION) \
    ACTION(component) \
    ACTION(context)

/// WHAT WIZARDY IS THIS!?

extern "C" {
#define HORUSLUA_DECLARE_INIT(name) extern int luaopen_##name(lua_State*);

    HORUS_MODULES_LIST(HORUSLUA_DECLARE_INIT)

#undef HORUSLUA_DECLARE_INIT
#define HORUSPYTHON_DECLARE_INIT(name) extern void init_##name(void);

    HORUS_MODULES_LIST(HORUSPYTHON_DECLARE_INIT)


#undef HORUSPYTHON_DECLARE_INIT
}

using ugdk::script::Module;
using ugdk::script::python::PyInitFunction;

#define HORUSLUA_LIST_ITEM(name) \
    ugdk::script::Module<lua_CFunction>(#name, luaopen_##name),

    static const Module<lua_CFunction> LUA_MODULES[HORUS_MODULES_NUM] = {
        HORUS_MODULES_LIST(HORUSLUA_LIST_ITEM)
    };

#undef HORUSLUA_LIST_ITEM

#define HORUSPYTHON_LIST_ITEM(name) \
    Module<PyInitFunction>("_"#name, init_##name),

    static const Module<PyInitFunction> PYTHON_MODULES[HORUS_MODULES_NUM] = {
        HORUS_MODULES_LIST(HORUSPYTHON_LIST_ITEM)
    };

#undef HORUSPYTHON_LIST_ITEM

template <class wrapper_t, class init_func_t>
static void RegisterModules(wrapper_t* wrapper,
                            const Module<init_func_t> modules[],
                            const char* lang_name) {
    for (size_t i = 0; i < HORUS_MODULES_NUM; ++i) {
        if(!wrapper->RegisterModule(modules[i])) {
            fprintf(stderr, "[%s] Load module '%s': >>ERROR<<\n", lang_name, modules[i].name().c_str());
        } else {
#ifdef DEBUG
            fprintf(stderr, "[%s] Load module '%s': ok\n", lang_name, modules[i].name().c_str());
#endif
        }
    }
}

void RegisterLuaModules(ugdk::script::lua::LuaWrapper* wrapper) {
    RegisterModules(wrapper, LUA_MODULES, "Lua");
}

void RegisterPythonModules(ugdk::script::python::PythonWrapper* wrapper) {
    RegisterModules(wrapper, PYTHON_MODULES, "Python");
}

#undef HORUS_MODULES_LIST




