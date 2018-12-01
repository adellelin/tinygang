#ifndef H_BFPATTERN
#define H_BFPATTERN

#include "Pattern.h"

class BookendFlip : public Pattern {

    CRGB paintLed (float position, float remaining, CRGB previous, int primaryHue) {


      //return CHSV(140, 160, 128); cyan
      if ((abs(-0.5 * sq(position - .5) - remaining)) < .95)
        return CHSV(primaryHue, 160, 64);
       
      //fade
      return previous.nscale8(192);

      /*
        return color(
        hue(previous),
        saturation(previous),
        brightness(previous)-10);

      */
    }

};

#endif
