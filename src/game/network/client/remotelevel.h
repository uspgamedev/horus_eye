#ifndef HORUSEYE_NETWORK_CLIENT_REMOTELEVEL_H_
#define HORUSEYE_NETWORK_CLIENT_REMOTELEVEL_H_

#include <ugdk/action/scene.h>

#include "game/components.h"
#include <list>

namespace network {

class RemoteLevel : public ugdk::action::Scene {
public:
    RemoteLevel();

    // STUFF DAHORA
    static RemoteLevel* reference();
    void AddGraphic(component::Graphic*);
    void RemoveGraphic(component::Graphic*);

private:
    std::list<component::Graphic*> graphic_components_;
};

} // namespace network

#endif // HORUSEYE_NETWORK_CLIENT_REMOTELEVEL_H_