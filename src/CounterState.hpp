#ifndef COUNTER_STATE_HPP
#define COUNTER_STATE_HPP
class CounterState {
  public:
    virtual void reset(long durationMillis) = 0;
    virtual void clockTick(long currentTimeMillis) = 0;

    virtual bool isFinished(long currentTimeMillis) = 0;
};
#endif
