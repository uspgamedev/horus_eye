#ifndef HORUSEYE_COMPONENT_LOGIC_H_
#define HORUSEYE_COMPONENT_LOGIC_H_

namespace component {

class Logic {
  public:
    virtual ~Logic() {}

    virtual void Update(double dt) = 0;
    
  protected:
    Logic() {}
};

}
#endif // HORUSEYE_COMPONENT_LOGIC_H_
