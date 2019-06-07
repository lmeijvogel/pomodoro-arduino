#ifndef COUNTER_STATE_HPP
#define COUNTER_STATE_HPP

#include <string.h>

class State {
  public:
    virtual void reset(long durationMillis) = 0;
    virtual void clockTick(long currentTimeMillis) = 0;

    virtual bool isFinished(long currentTimeMillis) = 0;

    virtual const char *name() = 0;
};
#endif
