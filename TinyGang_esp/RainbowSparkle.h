#ifndef H_RPATTERN
#define H_RPATTERN

#include "Pattern.h"


class RainbowSparkle : public Pattern {

    CRGB paintLed (float position, float remaining, CRGB previous, int primaryHue) {

      // println("position:" + position +", remaining:" + remaining);
      if (random(100) < 10) 
      return CRGB(position * 255, 128* (remaining + 0.2), 128 * (remaining + 0.2));

      return previous.nscale8(32);

    }

};

#endif
