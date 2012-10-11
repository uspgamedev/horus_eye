#ifndef HORUSEYE_COMPONENT_BASE_H_
#define HORUSEYE_COMPONENT_BASE_H_

namespace component {

class Base {
  public:
    virtual ~Base() {}

    virtual void Update(double dt) = 0;

  protected:
    Base() {}
};

}
#endif // HORUSEYE_COMPONENT_BASE_H_
