#ifndef H_BEPATTERN
#define H_BEPATTERN

#include "Pattern.h"

class BookendTrace : public Pattern {

  CRGB paintLed (float position, float remaining, CRGB previous, int primaryHue) {

      // trace
    //return CHSV(primaryHue, 255, 255);
    if ((abs(sq(position - .5)-remaining)) <.7)
      return CHSV(primaryHue, 225, 255);
      // CHSV max (255, 255, 255) whereas HSB (360, 100, 100)
       //return CHSV(140, 160, 128); cyan
         
      //fade
    return previous.nscale8(192);
    }

};

#endif
