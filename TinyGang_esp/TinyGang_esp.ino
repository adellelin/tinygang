/*
   Tiny Gang is an open source project to include multiple jacket wearers
   the base code is developed by @mpinner and @adellelin for two jacket
   interaction and is to be expanded for multiple players
*/

#include <FastLED.h>

#define LED_PIN     12
#define COLOR_ORDER GRB
#define CHIPSET     WS2811
#define NUM_LEDS    240

#include <elapsedMillis.h>

#include "RainbowSparkle.h"
#include "WhiteTrace.h"
#include "BookendTrace.h"
#include "Twinkler.h"
#include "BookendFlip.h"
#include "BassShader.h"
#include "BodyTwinkler.h"


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

int myPatternId = 0;

Pattern* patterns[] = {
  new BodyTwinkler(), // red yellow sparkle short
  new BassShader(), // pattern individually triggered
  new RainbowSparkle(), // more pale pink and blue
  new WhiteTrace(),
  new BookendTrace(),
  new Twinkler()
};

int inboundHue = 229; // incoming char sets color

int ledHue[] = { 0, 20, 255, 120, 229, 200};
// 120 was cyan
// 229 pink
// 22 orange
// 200 lilac
// 'a' green

elapsedMillis ellapseTimeMs[(sizeof(patterns)) / 4];

float crazyDuration = 5000;
float regularDuration = 2000;
float fastDuration = 2000;
float durationMs = regularDuration;

float repeatDurationMs = 5000;
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

//****************
// DIP SWITCHES

int pins[] = {2, 15};

//************************************************************
// this is a simple example that uses the easyMesh library
//
// 1. blinks led once for every node on the mesh
// 2. blink cycle repeats every BLINK_PERIOD
// 3. sends a silly message to every node on the mesh at a random time between 1 and 5 seconds
// 4. prints anything it receives to Serial.print
//
//
//************************************************************
#include <painlessMesh.h>

             // some gpio pin that is connected to an LED...
             // on my rig, this is 5, change to the right number of your LED.
#define   LED             5       // GPIO number of connected LED, ON ESP-12 IS GPIO2

#define   BLINK_PERIOD    3000 // milliseconds until cycle repeat
#define   BLINK_DURATION  100  // milliseconds LED is on for

#define   MESH_SSID       "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555

             // Prototypes
             void sendMessage();
void receivedCallback(uint32_t from, String & msg);
void newConnectionCallback(uint32_t nodeId);
void changedConnectionCallback();
void nodeTimeAdjustedCallback(int32_t offset);
void delayReceivedCallback(uint32_t from, int32_t delay);

Scheduler     userScheduler; // to control your personal task
painlessMesh  mesh;

bool calc_delay = false;
SimpleList<uint32_t> nodes;

void sendMessage() ; // Prototype
Task taskSendMessage( TASK_SECOND * 1, TASK_FOREVER, &sendMessage ); // start with a one second interval

// Task to blink the number of nodes
Task blinkNoNodes;
bool onFlag = false;

void setup() {

  // setup serial DEBUG and XBEE
  Serial.begin(115200);
  Serial1.begin(9600);

  // setup dipswitch
     for (int i = 0 ; i < 2; i++) {
      pinMode(pins[i], INPUT_PULLUP);
    }

  Serial.println("resetting");
  // LEDS.addLeds<WS2801, DATA_PIN_LEFT, CLOCK_PIN_LEFT, RGB>(leds_left, NUM_LEDS);
  // LEDS.addLeds<WS2801, DATA_PIN_RIGHT, CLOCK_PIN_RIGHT, RGB>(leds_right, NUM_LEDS);
  LEDS.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds_right, NUM_LEDS).setCorrection( TypicalLEDStrip );
  //LEDS.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds_left, NUM_LEDS).setCorrection( TypicalLEDStrip );
  LEDS.setBrightness(128);
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

  setupPainlessMesh();
}

void loop() {
  // available() -- Get the number of bytes (characters) available
  // for reading from the serial port.
  // This is data that's already arrived and stored in the
  // serial receive buffer (which holds 64 bytes).

  while (Serial.available()) {
    char inChar = (char)Serial.read();
    receiveMeshMsg(inChar);
  }

  /*
    // autosend every 2s
    if (ellapseTimeMs[0] > sendMs) {
    Serial1.print('a');
    Serial.print('a');
    sendEllapseMs = 0;
    }
  */
  
  checkPatternTimer();
  
  // send message to other jacket after pattern duration
  if (false == sentAlready) {
    //if (ellapseTimeMs[MY_PATTERN_ID] > durationMs / 4) { // sending halfway through
    if (ellapseTimeMs[myPatternId] > 0) { // sending halfway through
      Serial.print("sending::");
      Serial.print(" my-pattern-id:");
      Serial.print(myPatternId);
      Serial1.print((char)ledHue[myPatternId]); // sending color to IMU jackets
      Serial.print(" cmd:");
      Serial.println(patternCommand[myPatternId]);
      Serial1.print(patternCommand[myPatternId]);
      sentAlready = true;
      String msg = (String)patternCommand[myPatternId];
      mesh.sendBroadcast(msg);
    }
  }

  
  for (int i = 0; i < (sizeof(patterns)) / 4; i++) {
    for (int j = 0; j < NUM_LEDS; j ++) {
    // check timers for all patterns
      if (ellapseTimeMs[i] > durationMs ) {
        //led[i][j] = CHSV(100,100,100);
        led[i][j] = 0; // turn all LED to black once time hits
      } else {
        float position = (float)j / (float)NUM_LEDS;
        float remaining = 1.0 - ellapseTimeMs[i] / durationMs;
        // set color to be the incoming message color
        int color = inboundHue;
        // if own pattern, then lookup color array
        if (i == myPatternId) {
          color = ledHue[i];
        }
        led[0][j] = patterns[i]->paintLed (position, remaining, led[0][j], color);
        //Serial.println(ledHue[i]);
      }
    }
  }

  show();
  delay(10);

  // painless mesh
  mesh.update();
  digitalWrite(LED, !onFlag);
}


void show() {

  // jacket 1 and 2 have different mappings for their led strips
  // jacket 1 is mirrored, jacket 2 is mirrored
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

void checkPatternTimer() {
  // sets the timers for my pattern,  when timer goes to 0
  // set the ellapseTimeMs to 0, which triggers 
  // all actions
  
  myPatternId = choosePattern();
  float repeatMs = repeatDurationMs;

  // autofire over repeat duration
  if (ellapseTimeMs[myPatternId] > repeatMs) {
    // pattern fires when ellapseTimeMs of the pattern is 0
    ellapseTimeMs[myPatternId] = 0;
    durationMs = regularDuration;
    sentAlready = false;
  }
  return;
}

int choosePattern(){
  int swA = digitalRead(pins[0]);
  int swB = digitalRead(pins[1]);
  if (swA == 0 && swB == 0) return 0;
  if (swA == 0 && swB == 1) return 1;
  if (swA == 1 && swB == 0) return 2;
  if (swA == 1 && swB == 1) return 3;
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

void receiveMeshMsg(char inChar) {

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
// painless mesh

void setupPainlessMesh() {
  pinMode(LED, OUTPUT);

  mesh.setDebugMsgTypes(ERROR | DEBUG);  // set before init() so that you can see error messages

  mesh.init(MESH_SSID, MESH_PASSWORD, &userScheduler, MESH_PORT);
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
  mesh.onNodeDelayReceived(&delayReceivedCallback);

  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();

  blinkNoNodes.set(BLINK_PERIOD, (mesh.getNodeList().size() + 1) * 2, []() {
    // If on, switch off, else switch on
    if (onFlag)
      onFlag = false;
    else
      onFlag = true;
    blinkNoNodes.delay(BLINK_DURATION);

    if (blinkNoNodes.isLastIteration()) {
      // Finished blinking. Reset task for next run
      // blink number of nodes (including this node) times
      blinkNoNodes.setIterations((mesh.getNodeList().size() + 1) * 2);
      // Calculate delay based on current mesh time and BLINK_PERIOD
      // This results in blinks between nodes being synced
      blinkNoNodes.enableDelayed(BLINK_PERIOD -
                                 (mesh.getNodeTime() % (BLINK_PERIOD * 1000)) / 1000);
    }
  });
  userScheduler.addTask(blinkNoNodes);
  blinkNoNodes.enable();

  randomSeed(analogRead(A0));
}

void sendMessage() {
  String msg = "Hello from node ";
  msg += mesh.getNodeId();
  msg += " myFreeMemory: " + String(ESP.getFreeHeap());
  //mesh.sendBroadcast(msg);

  if (calc_delay) {
    SimpleList<uint32_t>::iterator node = nodes.begin();
    while (node != nodes.end()) {
      mesh.startDelayMeas(*node);
      node++;
    }
    calc_delay = false;
  }

  //Serial.printf("Sending message: %s\n", msg.c_str());

  taskSendMessage.setInterval( random(TASK_SECOND * 1, TASK_SECOND * 5));  // between 1 and 5 seconds
}

void receivedCallback(uint32_t from, String & msg) {
  Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
  receiveMeshMsg(msg[0]);
}

void newConnectionCallback(uint32_t nodeId) {
  // Reset blink task
  onFlag = false;
  blinkNoNodes.setIterations((mesh.getNodeList().size() + 1) * 2);
  blinkNoNodes.enableDelayed(BLINK_PERIOD - (mesh.getNodeTime() % (BLINK_PERIOD * 1000)) / 1000);

  Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
  Serial.printf("--> startHere: New Connection, %s\n", mesh.subConnectionJson(true).c_str());
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
  // Reset blink task
  onFlag = false;
  blinkNoNodes.setIterations((mesh.getNodeList().size() + 1) * 2);
  blinkNoNodes.enableDelayed(BLINK_PERIOD - (mesh.getNodeTime() % (BLINK_PERIOD * 1000)) / 1000);

  nodes = mesh.getNodeList();

  Serial.printf("Num nodes: %d\n", nodes.size());
  Serial.printf("Connection list:");

  SimpleList<uint32_t>::iterator node = nodes.begin();
  while (node != nodes.end()) {
    Serial.printf(" %u", *node);
    node++;
  }
  Serial.println();
  calc_delay = true;
}

void nodeTimeAdjustedCallback(int32_t offset) {
  Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset);
}

void delayReceivedCallback(uint32_t from, int32_t delay) {
  Serial.printf("Delay to node %u is %d us\n", from, delay);
}
