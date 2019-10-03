#ifndef H_BODYTWINKLE
#define H_BODYTWINKLE

#include "Pattern.h"

class BodyTwinkler : 
public Pattern {

public:
  //inline uint32_t shade (float height, uint32_t color, uint32_t currentColor, float remaining, uint32_t secondaryColor) {
  CRGB paintLed (float position, float remaining, CRGB previous, int primaryHue) {
    
  int normalizedHue = primaryHue + 255.0*position;
  if (normalizedHue > 255) { 
    normalizedHue -= 255;
  }

    if(remaining > 0.95) {
      if(50 > random(100)) {
        return CHSV(normalizedHue, 225, 128);
      }
    } 

    if(remaining > 0.85) {
      if(10 > random(100)) {
        return CHSV(normalizedHue, 225, 64);
      }
    } 


    if(10 > random(100)) {

      return previous.nscale8(32);
    }

    return previous;

  }  
};

#endif
