
#ifndef HORUSEYE_GAME_SPRITES_RESOURCES_RESOURCE_H_
#define HORUSEYE_GAME_SPRITES_RESOURCES_RESOURCE_H_

namespace resource {

/// Some sort of resource, like life, mana, etc.
/**
 * Actually a template class, since a resource type can be anything as long as it has the operators
 * <=, += and -= defined, and a copy constructor.
 * @param resource_t the resource type, which must have operators <=. += and -= defined.
 */
template <class T>
class Resource {

  public:

    typedef T resource_t;

    Resource(resource_t base_value) : resource_pool_(base_value) {}

    virtual ~Resource() {}

    virtual void Set(resource_t &quantity) {
        resource_pool_ = quantity;
    }

    void operator =(resource_t &quantity) {
        Set(quantity);
    }

    resource_t Get() {
        return resource_pool_;
    }

    operator resource_t() {
        return Get();
    }

    bool Has(resource_t quantity) {
        return quantity <= resource_pool_;
    }

    bool operator >=(resource_t quantity) {
        return Has(quantity);
    }

    virtual void Increase(resource_t quantity) {
        resource_pool_ += quantity;
    }

    void operator +=(resource_t quantity) {
        Increase(quantity);
    }

    virtual void Decrease(resource_t quantity) {
        resource_pool_ -= quantity;
    }

    void operator -=(resource_t quantity) {
        Decrease(quantity);
    }

  protected:

    resource_t& resource_pool() { return resource_pool_; }

  private:

    resource_t resource_pool_;

};

template <class resource_t>
bool operator <=(Resource<resource_t>& resource, resource_t quantity) {
    return resource.Has(quantity);
}

} /* namespace resource */

#endif /* HORUSEYE_GAME_SPRITES_RESOURCES_RESOURCE_H_ */
