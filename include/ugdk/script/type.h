
#ifndef UGDK_SCRIPT_VIRTUALTYPE_H_
#define UGDK_SCRIPT_VIRTUALTYPE_H_

#include <vector>

#include <ugdk/script/defs.h>

namespace ugdk {

namespace script {

class VirtualType {

  public:

    VirtualType() : types_(10, (struct swig_type_info*)NULL) {}
    ~VirtualType() {}

    struct swig_type_info* FromLang(LangID id) const {
        return types_[id];
    }

    void RegisterFromLang(struct swig_type_info* info, LangID id) {
        if (!types_[id]) types_[id] = info;
    }

  private:

    typedef std::vector<struct swig_type_info*> TypeFromLang;

    TypeFromLang types_;

};


template <class T>
class TypeRegistry {

  public:

    static VirtualType& type () {
        return type_;
    }

  private:

    TypeRegistry () {}

    static VirtualType type_;

};

template <class T>
VirtualType TypeRegistry<T>::type_;

} /* namespace script */

} /* namespace ugdk */

template <class T>
static void RegisterType(T* tp) {
    (void)tp;
}

template <typename T>
T* GetNull () { return NULL; }

#endif /* UGDK_SCRIPT_VIRTUALTYPE_H_ */
