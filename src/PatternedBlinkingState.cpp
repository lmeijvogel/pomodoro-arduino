#ifndef PATTERNED_BLINKING_STATE_CPP
#define PATTERNED_BLINKING_STATE_CPP

#include "CountdownState.cpp"
#include "Light.hpp"

typedef Light* LightPtr;
typedef const char** Pattern;

class PatternedBlinkingState : public CountdownState {
  public:
    PatternedBlinkingState(unsigned long durationMillis, LightPtr *lights, int numberOfLights, const char *name, const char **patterns, int numberOfPatterns, int numberOfRepetitions) : CountdownState(durationMillis, lights, numberOfLights) {
      this->_name = name;

      this->buildPatternCache(patterns, numberOfPatterns, numberOfRepetitions);
    }

    virtual const char* name() {
      return this->_name;
    }

    virtual void resetLightToDefaultState(LightPtr) {
      // Do nothing -- it will only flash briefly
    }

    virtual void turnLightsOn(double timeFractionPassed) {
      const char **currentPattern = this->currentPattern(timeFractionPassed);

      this->applyPattern(currentPattern);
    }

  private:
    const char *_name;

    const char **patterns;
    int numberOfPatterns;

    void buildPatternCache(const char **patterns, int numberOfPatterns, int numberOfRepetitions) {
      this->patterns = (const char **)malloc(sizeof(const char**) * numberOfPatterns * numberOfRepetitions);

      for (int i = 0 ; i < numberOfRepetitions ; i++) {
        for (int j = 0 ; j < numberOfPatterns ; j++) {
          this->patterns[i * numberOfPatterns + j] = patterns[j];
        }
      }

      this->numberOfPatterns = numberOfRepetitions * numberOfPatterns;
    }

    const char **currentPattern(double timeFractionPassed) {
      for (int i = 0 ; i < this->numberOfPatterns ; i++) {
        double patternStart = (1.0 / this->numberOfPatterns) * i;
        double patternEnd = patternStart + (1.0 / this->numberOfPatterns);

        if (patternStart <= timeFractionPassed && timeFractionPassed < patternEnd) {
          printf("Found pattern: %d\n", i);
          return &this->patterns[i];
        }
      }

      printf("Did not find pattern\n");
      return &this->patterns[0];
    }

    void applyPattern(const char **pattern) {
      for (int i = 0 ; i < this->numberOfLights ; i++) {
        bool turnOn = (*pattern)[i] == '1';

        if (turnOn) {
          this->lights[i]->turnOn();
        } else {
          this->lights[i]->turnOff();
        }
      }
    }
};
#endif
