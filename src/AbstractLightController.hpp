#ifndef ABSTRACT_LIGHT_CONTROLLER_H
#define ABSTRACT_LIGHT_CONTROLLER_H

class AbstractLightController {
public:
  virtual void setOn() = 0;
  virtual void setOff() = 0;
};

#endif
