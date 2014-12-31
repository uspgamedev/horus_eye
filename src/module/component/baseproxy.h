#ifndef MODULE_PROXY_BASE_H_
#define MODULE_PROXY_BASE_H_

#include <ugdk/script/baseproxy.h>
#include <ugdk/script/virtualobj.h>
#include <game/components/base.h>
#include <game/components/orders.h>
#include <game/map.h>

#include <ugdk/script/languages/lua/luawrapper.h>
#include <ugdk/script/languages/lua/datagear.h>
#include <ugdk/script/languages/lua/luadata.h>

namespace component {

class BaseProxy;

class BaseProxy :
    public Base,
    public ugdk::script::BaseProxy<BaseProxy>
{
  public:
    BaseProxy(const ugdk::script::VirtualObj& proxy) :
        ugdk::script::BaseProxy<BaseProxy>(proxy) {}

    virtual void Update(double dt) override {
        ( proxy_ | "Update" )(dt);
    }

    virtual std::string component_name() const override {
        return proxy_["component_name"].value<std::string>();
    }

    /// The order that the component is registered with on the object.
    virtual int order() const override {
        auto order_field = FindRawget()(proxy_, "order");
        if (order_field)
            return order_field.value<int>();
        return orders::LOGIC;
    }

    /// Called when the component is added to an object.
    virtual void OnAdd(sprite::WObjRawPtr wobj) override {
        if (FindRawget()(proxy_, "OnAdd")) {
            (proxy_ | "OnAdd")(wobj);
        }
    }

    /// Called when the object is removed.
    virtual void OnObjectRemoved() override {
        if (FindRawget()(proxy_, "OnObjectRemoved")) {
            (proxy_ | "OnObjectRemoved")();
        }
    }

private:
    static ugdk::script::VirtualObj FindRawget() {
        // FIXME: HOLY CRAP, WE NEED SOMETHING BETTER
        using namespace ugdk::script;
        auto luavm = dynamic_cast<ugdk::script::lua::LuaWrapper*>(SCRIPT_MANAGER()->GetWrapper("Lua"));
        luavm->data_gear()->getglobal("rawget");
        auto luadata = luavm->NewLuaData();
        luadata->UnsafePopValue();
        return VirtualObj(VirtualData::Ptr(luadata));
    }
};

} // namespace component

#endif
