
#include "objecthandle.h"

#include "game/sprites/worldobject.h"
#include "game/sprites/exceptions.h"

namespace sprite {

ObjectHandle::ObjectHandle() {}

ObjectHandle::ObjectHandle(const ObjectHandle& r) : pointer_(r.pointer_) {}

ObjectHandle::ObjectHandle(const WObjPtr& r) : pointer_(r) {}

ObjectHandle::ObjectHandle(const WObjWeakPtr& r) : pointer_(r) {}

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
        throw InvalidObject();
    return ptr.get();
}

}