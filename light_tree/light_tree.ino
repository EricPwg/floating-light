#include <FastLED.h>
#define NUM_LEDS 60
#define DATA_PIN 6

CRGB leds[NUM_LEDS];

int brightness=0, brightness_delta=1; //delta為亮度變化量
float easing = 30;  //讓current逐漸變成target

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
    if(r<a.r && g<a.g && b<a.b)
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
struct Color green = {46, 139, 87};

struct Color current = {0, 0, 0}; //current color setting
struct Color target = {0, 0, 0};  //target color setting
struct Color error = {2, 2, 2}; //if current is very close to the target

void setup() {
  FastLED.addLeds<WS2813, DATA_PIN, RGB>(leds, NUM_LEDS);  
  current = blue;
  target = green;
}

void loop() {
  if(target==green && target-current<error)
    target = blue;
  if(target==blue && target-current<error)
    target = green;
  

  if(analogRead(A7) > 600)
    target = green;

  current = current + ((target-current)/easing);  //gradually change the color

  if(brightness >= 255) brightness_delta = -3;
  if(brightness <= 0) brightness_delta = 3;
  brightness += brightness_delta;
  
  for (int i=0; i<NUM_LEDS; i++){
    leds[i] = CRGB(current.g*brightness/255, current.r*brightness/255, current.b*brightness/255);  
  }
  FastLED.show();
  delay(50);
}


