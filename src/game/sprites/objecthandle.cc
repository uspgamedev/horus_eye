
#include "objecthandle.h"

#include "game/sprites/worldobject.h"

#include <ugdk/graphic/opengl/Exception.h> // TODO: This is SOOOOO wrong.

namespace sprite {

ObjectHandle::ObjectHandle() {}

ObjectHandle::ObjectHandle(const ObjectHandle& r) : pointer_(r.pointer_) {}

ObjectHandle::ObjectHandle(const WObjPtr& r) : pointer_(r) {}

ObjectHandle ObjectHandle::operator = (const WObjPtr& r) {
    pointer_ = r;
    return *this;
}

bool ObjectHandle::attached() const {
    return static_cast<bool>(pointer_.lock());
}

WorldObject* ObjectHandle::operator-> () const {
    WObjPtr ptr = pointer_.lock();
    if (!ptr)
        throw love::Exception("Attempting to operate on unattached ObjectHandle.");
    return ptr.get();
}

}