
#ifndef UGDK_SCRIPT_LUA_NATIVE_BOOTSTRAP_H_
#define UGDK_SCRIPT_LUA_NATIVE_BOOTSTRAP_H_

#include <ugdk/script/lua/header.h>

/*namespace ugdk {
namespace script {
namespace lua {
namespace native {*/

extern "C" {

static int ll_LoadLibs (lua_State* L) {
    //ugdk::script::lua::State L(L_);
    // luaL_checkversion(L.get());  // TODO-lua5.2: add this.
    /*L.gc(ugdk::script::lua::Constant::gc::STOP(), 0);
    L.aux().openlibs();
    L.gc(ugdk::script::lua::Constant::gc::RESTART(), 0);*/
    lua_gc(L, LUA_GCSTOP, 0);
    luaL_openlibs(L);
    lua_gc(L, LUA_GCRESTART, 0);
    return 0;
}

static int ll_PreloadModules (lua_State* L_) {
    //ugdk::script::lua::State L(L_);
    //L.settop(1);
    /*
    const ll_Args& args = GetArg<ll_Args>(L);

    L.getglobal(LUA_LOADLIBNAME);           // [pack]
    L.getfield(-1, "preload");              // [pack,preload]
    ll_Args::const_iterator it = args.begin();
    for (; it != args.end(); ++it) {
        L.push(it->init_func_);             // [pack,preload,init_func]
        L.setfield(-2, it->name_.c_str());  // [pack,preload]
    }
    L.pop(2);*/
    return 0;
}

}

//} /* namespace native */
//} /* namespace lua */
//} /* namespace script */
//} /* namespace ugdk */

#endif /* UGDK_SCRIPT_LUA_NATIVE_TRACEBACK_H_ */
