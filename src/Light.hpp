#ifndef LIGHTS_HPP
#define LIGHTS_HPP

class Light {
public:
  virtual void turnOn() = 0;
  virtual void turnOff() = 0;
};
#endif
