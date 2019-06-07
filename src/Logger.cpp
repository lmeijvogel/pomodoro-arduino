#ifndef LOGGER_CPP
#define LOGGER_CPP

#ifdef ARDUINO
#include "Arduino.h"
#else
#include <stdio.h>
#endif

  class Logger {
    public:
      static void println(char *message) {
#ifdef ARDUINO
        Serial.println(message);
#else
        message = message;
#endif
      }

      static void println(const char *message) {
#ifdef ARDUINO
        Serial.println(message);
#else
        message = message;
#endif
      }

      static void print(char *message) {
#ifdef ARDUINO
        Serial.print(message);
#else
        message = message;
#endif
      }

      static void print(const char *message) {
#ifdef ARDUINO
        Serial.print(message);
#else
        message = message;
#endif
      }
  };

#endif
