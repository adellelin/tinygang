/*
   Tiny Gang is an open source project to include multiple jacket wearers
   the base code is developed by @mpinner and @adellelin for two jacket
   interaction and is to be expanded for multiple players
*/

#include "FastLED.h"

#include <elapsedMillis.h>

#include "RainbowSparkle.h"
#include "WhiteTrace.h"
#include "BookendTrace.h"
#include "Twinkler.h"
#include "BookendFlip.h"

// How many leds in your strip?
#define NUM_LEDS 42

#define JACKET1_LEFT  1
#define JACKET1_RIGHT 2
#define JACKET2_LEFT   11
#define JACKET2_RIGHT  12

#define JACKET1 1 // use this when other jacket is HR, otherwise 0
#define JACKET2 0 // we used JACKET2 0 for white jacket

#define WHOSE_HARDWARE JACKET1 // little black jacet with one long strip split to two connectors

#define ADC_TEENSY_3_1 // change this depending on which board

#ifdef ADC_TEENSY_3_1 // sets up pin conditions for the 3.2 boards
#define DATA_PIN_RIGHT 23
#define CLOCK_PIN_RIGHT 22
#define DATA_PIN_LEFT 18
#define CLOCK_PIN_LEFT 19
#else
#define DATA_PIN_RIGHT 8
#define CLOCK_PIN_RIGHT 7
#define DATA_PIN_LEFT 4
#define CLOCK_PIN_LEFT 3
#endif

#define MY_PATTERN_ID 4

Pattern* patterns[] = {
  new BookendFlip(), // pattern individually triggered
  new WhiteTrace(), // pattern upon receive
  new RainbowSparkle(),
  new BookendTrace(),
  new WhiteTrace(),
  new Twinkler()
};

int inboundHue = 229; // incoming char sets color

int ledHue[] = { 0, 20, 255, 229, 200, 120};
// 120 was cyan
// 229 pink
// 22 orange
// 200 lilac
// 'a' green

elapsedMillis ellapseTimeMs[(sizeof(patterns)) / 4];

float crazyDuration = 2000;
float regularDuration = 3000;
float fastDuration = 2000;
float durationMs = crazyDuration;

float repeatDurationMs = 17000;
float repeatCrazyMs = 3000;
boolean sentAlready = false; // send message boolean

elapsedMillis sendEllapseMs;
float sendMs = 2000;

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

CRGB led[(sizeof(patterns)) / 4][NUM_LEDS]; //

// Define the array of leds
CRGB leds_right[NUM_LEDS];
CRGB leds_left[NUM_LEDS];

char patternCommand[] = {
  'q', 'a', 'z',
  'w', 's', 'x',
  'e', 'd', 'c',
  'r', 'f', 'v'
};

void setup() {

  // setup serial DEBUG and XBEE
  Serial.begin(9600);
  Serial1.begin(9600);

  // setup pins for JACKET1 vs JACKET2
  pinMode(10, INPUT_PULLUP);
  pinMode(11, OUTPUT);
  digitalWrite(11, LOW);
  pinMode(12, INPUT_PULLUP);

  Serial.println("resetting");
  LEDS.addLeds<WS2801, DATA_PIN_LEFT, CLOCK_PIN_LEFT, RGB>(leds_left, NUM_LEDS);
  LEDS.addLeds<WS2801, DATA_PIN_RIGHT, CLOCK_PIN_RIGHT, RGB>(leds_right, NUM_LEDS);
  LEDS.setBrightness(255);
  LEDS.setDither( 0 );
  FastLED.setDither( 0 );

  Serial.print("uploaded.. ");
  Serial.println(__FILE__);

  Serial.print("reseting jacket.. ");
  Serial.println(__DATE__);

  Serial.print("jacket switch is");
  // For adding a switch to have a pattern that is always on vs regular
  if (isCrazy()) {
    Serial.println(" CRAZY");
  }
  if (isRegular()) {
    Serial.println(" REGULAR");
  }

  for (int i = 0; i < (sizeof(patterns)) / 4; i++) {
    for (int j = 0; j < NUM_LEDS; j ++) {
      //led[i][j] = CHSV(100,100,100);
      led[i][j] = CHSV(255 / NUM_LEDS * j, 255 / (sizeof(patterns)) / 4 * i, 255);
    }
  }

  inputString.reserve(200);
}

void loop() {
  // available() -- Get the number of bytes (characters) available
  // for reading from the serial port.
  // This is data that's already arrived and stored in the
  // serial receive buffer (which holds 64 bytes).

  while (Serial1.available()) {

    char inChar = (char)Serial1.read();
    Serial.println(inChar);
    //Serial.println(sizeof(patternCommand));
    Serial.println(sizeof(patterns) / 4);
    // read the incoming character and check it's index,
    // start the corresponding pattern ID by setting its timer to 0
    for  (int i = 0; i < sizeof(patternCommand); i++) {
      if (inChar == patternCommand[i]) {
        ellapseTimeMs[i] = 0;
        //durationMs = regularDuration;
      } else {
        // if incoming characters do not correspond to pattern ID
        // it sets the jacket color
        inboundHue = inChar;
      }
    }
  }

  /*
    // autosend every 2s
    if (ellapseTimeMs[0] > sendMs) {
    Serial1.print('a');
    Serial.print('a');
    sendEllapseMs = 0;
    }
  */

  if (isRegular()) {
    choosePattern();
  }  else {
    choosePattern();
  }


  // send message to other jacket after pattern duration
  if (false == sentAlready) {
    //if (ellapseTimeMs[MY_PATTERN_ID] > durationMs / 4) { // sending halfway through
    if (ellapseTimeMs[MY_PATTERN_ID] > 0) { // sending halfway through
      Serial.print("sending::");
      Serial.print(" my-pattern-id:");
      Serial.print(MY_PATTERN_ID);
      Serial1.print((char)ledHue[MY_PATTERN_ID]); // sending color to IMU jackets
      Serial.print(" cmd:");
      Serial.println(patternCommand[MY_PATTERN_ID]);
      Serial1.print(patternCommand[MY_PATTERN_ID]);
      sentAlready = true;

    }
  }

  for (int i = 0; i < (sizeof(patterns)) / 4; i++) {
    for (int j = 0; j < NUM_LEDS; j ++) {

      if (ellapseTimeMs[i] > durationMs ) {
        //led[i][j] = CHSV(100,100,100);
        led[i][j] = 0; // turn all LED to black once time hits
      } else {
        float position = j / (float)NUM_LEDS;
        float remaining = 1.0 - ellapseTimeMs[i] / durationMs;
        // set color to be the incoming message color
        int color = inboundHue;
        // if own pattern, then lookup color array
        if (i == MY_PATTERN_ID) {
          color = ledHue[i];
        }
        led[0][j] = patterns[i]->paintLed (position, remaining, led[0][j], color);
        //Serial.println(ledHue[i]);
      }

    }
  }

  show();
  delay(10);

}


void show() {
  int half_leds = (NUM_LEDS / 2);

  switch (WHOSE_HARDWARE) {
    case JACKET1 :
      for (int i = 0; i < half_leds; i ++) {
        leds_left[i] = led[0][i];
        leds_right[i] = led[0][i];
      }
      for (int i = 0; i < half_leds; i ++) {
        leds_left[i + half_leds] = led[0][half_leds - i];
        leds_right[i + half_leds] = led[0][half_leds - i];
      }
      break;
    case JACKET2:
      for (int i = 0; i < NUM_LEDS; i ++) {
        leds_right[i] = led[0][i];
        leds_left[i] = led[0][i];
      }
      break;

  }


  FastLED.show();

  return;
}

void choosePattern() {

  float repeatMs = repeatDurationMs;

  if (isCrazy() ) {
    repeatMs = repeatCrazyMs;
  }

  // autofire over repeat duration
  if (ellapseTimeMs[MY_PATTERN_ID] > repeatMs) {
    ellapseTimeMs[MY_PATTERN_ID] = 0;
    durationMs = regularDuration;
    sentAlready = false;
  }

  return;
}


void fadeall() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds_right[i].nscale8(200);
    leds_left[i].nscale8(200);
  }
}

boolean isRegular() {
  return !isCrazy();
  //digitalRead(10);
}

boolean isCrazy() {
  return false;
  //digitalRead(12);
}




