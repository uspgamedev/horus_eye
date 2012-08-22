#ifndef MODULE_PROXY_LOGIC_H_
#define MODULE_PROXY_LOGIC_H_

#include <ugdk/script/baseproxy.h>
#include <ugdk/script/virtualobj.h>
#include <game/components/logic.h>
#include <game/scenes.h>

namespace component {

class LogicProxy;

class LogicProxy : public Logic, public ugdk::script::BaseProxy<LogicProxy> {
public:
    LogicProxy(const ugdk::script::VirtualObj& proxy) :
        ugdk::script::BaseProxy<LogicProxy>(proxy) {}

    virtual void Update(double dt) {
        ugdk::script::VirtualObj::List args;
        ugdk::script::VirtualObj vdt = ugdk::script::VirtualObj(proxy_.wrapper());
        vdt.set_value(dt);
        args.push_back(vdt);
        ( proxy_ | "Update" )(args);
    }

    virtual void OnWorldAdd(scene::World* scene) {
        if(proxy_["OnSceneAdd"]) {
            ugdk::script::VirtualObj::List args;
            ugdk::script::VirtualObj vscene = ugdk::script::VirtualObj(proxy_.wrapper());
            vscene.set_value<scene::World*>(scene);
            args.push_back(vscene);
            ( proxy_ | "OnSceneAdd" )(args);
        }
    }

};

} // namespace component

#endif
