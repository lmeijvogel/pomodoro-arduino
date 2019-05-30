#ifndef COUNTER_STATE_HPP
#define COUNTER_STATE_HPP

#include <string>

class CounterState {
  public:
    virtual void reset(long durationMillis) = 0;
    virtual void clockTick(long currentTimeMillis) = 0;

    virtual bool isFinished(long currentTimeMillis) = 0;

    virtual std::string name() = 0;
};
#endif
