#ifndef MODULE_PROXY_BASE_H_
#define MODULE_PROXY_BASE_H_

#include <ugdk/script/baseproxy.h>
#include <ugdk/script/virtualobj.h>
#include <game/components/base.h>
#include <game/map.h>

namespace component {

class BaseProxy;
class BaseProxy : public Base, public ugdk::script::BaseProxy<BaseProxy> {
public:
    BaseProxy(const ugdk::script::VirtualObj& proxy) :
        ugdk::script::BaseProxy<BaseProxy>(proxy) {}

    virtual void Update(double dt) {
        ( proxy_ | "Update" )(dt);
    }

};

} // namespace component

#endif
