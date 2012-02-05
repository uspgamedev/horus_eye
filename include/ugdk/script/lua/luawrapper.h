
#ifndef UGDK_SCRIPT_LUA_LUAWRAPPER_H_
#define UGDK_SCRIPT_LUA_LUAWRAPPER_H_

#include <string>
#include <vector>

#include <ugdk/script/lua/header.h>
#include <ugdk/script/lua/gear.h>
#include <ugdk/script/langwrapper.h>

namespace ugdk {
namespace script {
namespace lua {

class LuaWrapper: public LangWrapper, public Identifiable {

  public:

    LuaWrapper() :
        LangWrapper("lua",LANG(Lua)),
        L_(NULL),
        shared_gear_(NULL) {}
    ~LuaWrapper() {
        if (L_) Finalize();
    }

    /// Overwritten methods.

    bool RegisterModule(const std::string& name, lua_CFunction init_func);

    bool Initialize();

    void Finalize();

    VirtualData::Ptr NewData();

    VirtualObj LoadModule(const std::string& name);

    /// Other methods.

    Gear MakeGear() { return Gear(L_, id()); }

    void Share(Gear* gear) {
        shared_gear_ = gear;
    }

    Gear* shared_gear() {
        return shared_gear_;
    }

  private:

    lua_State*          L_;
    std::vector<Module> modules_;
    Gear*               shared_gear_;

};

} /* namespace lua */
} /* namespace script */
} /* namespace ugdk */

#endif /* UGDK_SCRIPT_LUA_LUAWRAPPER_H_ */
