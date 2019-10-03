#ifndef H_BASS
#define H_BASS

#include "Pattern.h"

class BassShader : 
public Pattern {

public:
  CRGB paintLed (float position, float remaining, CRGB previous, int primaryHue) {

    float closeness = 1.0 - abs(position - remaining);

    float normalizedHue = position + remaining ;
    if (normalizedHue > 1.0) 
      normalizedHue -= 1;

    return CHSV(255.0*normalizedHue, 225, 255.0*remaining);

    // some basic examples of how to get started playing with these normalized float parameters
    //
    //return CHSV(primaryHue, 225, (int)(255.0*remaining));
    //return CHSV(255.0*closeness, 225, 255.0*remaining);
    //return CHSV(255.0*position, 225, 255.0*remaining);
    //return CHSV(primaryHue, 225, (int)(255.0*position));
    //return CHSV(primaryHue, 225, (int)(255.0*remaining*position));
    //return CHSV(primaryHue, 225, (int)(255.0*closeness));
    

  }  
  
  
};

#endif //H_BASS
