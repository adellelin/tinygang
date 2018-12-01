
#ifndef H_PATTERN
#define H_PATTERN

class Pattern {
  public: virtual CRGB paintLed (float position, float remaining, CRGB previous, int primaryHue);
};


#endif

