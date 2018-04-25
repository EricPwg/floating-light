#include <FastLED.h>
#include <math.h>
#define NUM_LEDS 60
#define DATA_PIN 6
#define DATA_PIN2 5

CRGB leds[NUM_LEDS];
CRGB leds2[NUM_LEDS];

int brightness=0, brightness_delta=1; //delta為亮度變化量
float easing = 30;  //讓current逐漸變成target

int state = 0;

int state2_loc;
int state6_loc;

struct Color{
  float r;
  float g;
  float b;
  
  inline Color operator+(Color a) {
        return {r+a.r, g+a.g, b+a.b};
  }
  inline Color operator-(Color a) {
        return {r-a.r, g-a.g, b-a.b};
  }
  inline Color operator/(float easing) {
        return {r/easing, g/easing, b/easing};
  }
  bool operator<(Color a) {
    if(abs(r)<a.r && abs(g)<a.g && abs(b)<a.b)
        return true;
    return false;
  }
  bool operator==(Color a) {
    if(r==a.r && g==a.g && b==a.b)
        return true;
    return false;
  }
};

struct Color blue = {0, 0, 139};
struct Color green = {20, 139, 40};

struct Color orange = {220, 70, 0};
struct Color white = {255, 255, 255};

struct Color pink = {250, 128, 120};

struct Color current = {0, 0, 0}; //current color setting
struct Color target = {0, 0, 0};  //target color setting
struct Color error = {2, 2, 2}; //if current is very close to the target

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2813, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.addLeds<WS2813, DATA_PIN2, RGB>(leds2, NUM_LEDS);
  current = blue;
  target = green;
}

void loop() {
  for (int i=0;i<NUM_LEDS;i++){
    leds[i] = CRGB(255, 0, 0);
    leds2[i] = CRGB(0, 0, 255);
  }
  FastLED.show();
  delay(500);
  for (int i=0;i<NUM_LEDS;i++){
    leds[i] = CRGB(255, 255, 0);
    leds2[i] = CRGB(0, 255, 0);
  }
  FastLED.show();
  delay(500);
}
