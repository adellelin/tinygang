#ifndef H_TWINKLERPATTERN
#define H_TWINKLERPATTERN

#include "Pattern.h"

class Twinkler : public Pattern {

    CRGB paintLed (float position, float remaining, CRGB previous, int primaryHue) {


      // increases toward larger position
      float closeness = 1.0 - abs((1.0 - position) - remaining);

      // decreases toward larger position
      float closenessBackwards = 1.0 - abs((1.0 - position) - remaining);

      // time and position are close
      if (closeness > .90) {
        return CHSV (80*remaining+140, 128*remaining, 64 * closeness * remaining);
      }

///*
      // time and position are not close
      if (closenessBackwards < .30) {
        return CHSV (80*remaining+140, 128*remaining, 80 * remaining);
      }
//*/

    ///*
      // randomly add bright pixels
      if (remaining > 0.95) {
        if (5 > random(100)) {
          return CHSV (80*remaining+140, 128*remaining, 64);
        }
      }

      //*/

      
      /*
          if (remaining > 0.85) {
            if (5 > random(100)) {
              return color (100, 0, 255);
            }
          } */


      if (1 > random(100)) {

        return previous.nscale8(192);

      }


      return previous;


    }

};

#endif
