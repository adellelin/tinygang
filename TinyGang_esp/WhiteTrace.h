#ifndef H_WTPATTERN
#define H_WTPATTERN

#include "Pattern.h"

class WhiteTrace : public Pattern {

  CRGB paintLed (float position, float remaining, CRGB previous, int primaryHue) {

      // trace
      if (abs(remaining - position) < .20)
        return CHSV(primaryHue, 255, 255);
        //return CHSV(220, 200, 128); // pink
      //fade
      return previous.nscale8(192);

    }

};

#endif
