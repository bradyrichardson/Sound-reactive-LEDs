#include <FastLED.h>

#define LED_PIN     3
#define NUM_LEDS    297 // adjust this to match the number of LEDs in strip
#define BRIGHTNESS  50
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
#define SENSOR_PIN A2
#define MAX_PULSE_LENGTH 17 // adjust this value as needed
#define MAX_PULSES 80
#define THRESHOLD 700

// const CRGB COLORS[3] = {CRGB::Aqua, CRGB::WhiteSmoke, CRGB::Blue}; // example of how to use multiple colors

struct Pulse {
  int position;
  int length;
  CRGB color;
  bool active;
};

Pulse pulses[MAX_PULSES];

void startNewPulse() {
  for (int i = 0; i < MAX_PULSES; i++) {
    if (!pulses[i].active) {
      pulses[i].position = 0;
      pulses[i].length = 1;  // start with a length of 1
      // pulses[i].color = COLORS[i % 4]; // example of how to use multiple colors
      pulses[i].color = CRGB::Aqua;
      pulses[i].active = true;
      break;
    }
  }
}

void updatePulses() {
  FastLED.clear();

  for (int i = 0; i < MAX_PULSES; i++) {
    if (pulses[i].active) {
      // draw the pulse
      for (int j = 0; j < pulses[i].length; j++) {
        int pos = pulses[i].position - j;
        if (pos >= 0 && pos < NUM_LEDS) {
          leds[pos] = pulses[i].color;
          leds[pos].fadeToBlackBy(64 * j / pulses[i].length);  // fade based on position in pulse
        }
      }
      
      // move the pulse
      pulses[i].position++;
      
      // increase pulse length if below max
      if (pulses[i].length < MAX_PULSE_LENGTH) {
        pulses[i].length++;
      }
      
      // deactivate the pulse if it reaches the end
      if (pulses[i].position - pulses[i].length >= NUM_LEDS) {
        Serial.println("TEST");
        pulses[i].active = false;
      }
    }
  }
}

void setup() {
  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(  BRIGHTNESS );
  pinMode(SENSOR_PIN, INPUT);
  Serial.begin(9600);

  for (int i = 0; i < MAX_PULSES; i++) {
    pulses[i].active = false;
  }
}

void loop() {
  int val = analogRead(SENSOR_PIN);
  
  if (val > THRESHOLD) {
    startNewPulse();
  }

  updatePulses();
  FastLED.show();
  // delay(10);  // adjust this delay to control the overall animation speed
}
