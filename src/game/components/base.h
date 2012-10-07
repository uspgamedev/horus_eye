#ifndef HORUSEYE_COMPONENT_BASE_H_
#define HORUSEYE_COMPONENT_BASE_H_

#include <string>

namespace component {

class Base {
  public:
    virtual ~Base() {}

    virtual const std::string& name() const { return name_; }
    virtual void Update(double dt) = 0;

  protected:
    Base() {}
    Base(const std::string& name) : name_(name) {}

    void set_name(const std::string& name) { name_ = name; }

  private:
    std::string name_;
};

}
#endif // HORUSEYE_COMPONENT_BASE_H_
