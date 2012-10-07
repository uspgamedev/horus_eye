#ifndef HORUSEYE_COMPONENT_BASE_H_
#define HORUSEYE_COMPONENT_BASE_H_

#include <string>

namespace component {

class Base {
  public:
    virtual ~Base() {}

    virtual void Update(double dt) = 0;

    const std::string& name() const { return name_; }
    int order() const { return order_; }

  protected:
    Base(const std::string& name, int order = 0) : name_(name), order_(order) {}

    void set_order(int order) { order_ = order; }

  private:
    std::string name_;
    int order_;
};

}
#endif // HORUSEYE_COMPONENT_BASE_H_
