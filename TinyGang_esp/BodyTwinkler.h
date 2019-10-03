#ifndef H_BODYTWINKLE
#define H_BODYTWINKLE

#include "Pattern.h"

class BodyTwinkler : 
public Pattern {

public:
  //inline uint32_t shade (float height, uint32_t color, uint32_t currentColor, float remaining, uint32_t secondaryColor) {
  CRGB paintLed (float position, float remaining, CRGB previous, int primaryHue) {


    float closeness = 1.0 - abs(position - remaining);
    if (closeness > .90) {
      return CHSV(255.0*closeness, 225, 255.0*closeness*remaining);
    } 
    else if (closeness < .30) {
      return CHSV(255.0*closeness, 225, 255.0*remaining);
    }




    if(remaining > 0.95) {
      if(10 > random(100)) {
        return CHSV(primaryHue, 225, 128);
      }
    } 

    if(remaining > 0.85) {
      if(10 > random(100)) {
        return CHSV(primaryHue, 225, 64);
      }
    } 


    if(10 > random(100)) {

      return previous.nscale8(192);
    }

    return previous;

  }  
};

#endif
