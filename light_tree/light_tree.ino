#include <FastLED.h>
#include <math.h>
#define NUM_LEDS 60
#define DATA_PIN 6

CRGB leds[NUM_LEDS];

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
  current = blue;
  target = green;
}

void loop() {
  switch(state){
    case 0:
      handle_state0();
      break;
    case 1:
      handle_state1();
      break;
    case 2:
      handle_state2();
      break;
    case 3:
      handle_state2();
      handle_state3();
      break;
    case 4:
      handle_state4();
      break;
     case 5:
      handle_state5();
      break;
     case 6:
      handle_state5();
      handle_state6();
      break;
  }
  
  if(state == 0 && analogRead(A7) > 600){
    //state = 1;
    //target = orange;
    state = 4;
    target = pink;
    easing = 5;
  }
  else if (state == 1 && target-current<error){
    state = 2;
    state = 4;
    easing = 10;
  }
  else if (state == 2 && analogRead(A7) < 600){
    target = blue;
    easing = 60;
    state = 3;
    state2_loc = 0;
  }
  else if (state == 3 && state2_loc >= 60){
    easing = 30;
   state = 0;
   current = blue;
  }
  else if(state == 4 && target-current<error){
    state = 5;
    easing = 5;
  }
  else if (state == 5 && analogRead(A7) < 600){
    target = blue;
    easing = 60;
    state = 6;
    state6_loc = 0;
  }
  else if (state == 6 && state6_loc >= 60){
    easing = 30;
   state = 0;
   current = blue;
  }

  Serial.print(current.r);
  Serial.print(" ");
  Serial.print(current.g);
  Serial.print(" ");
  Serial.print(current.b);
  Serial.print(" ");
  Serial.println(state);
  FastLED.show();
  delay(50);
}

void handle_state0(){
  if(target==green && target-current<error)
    target = blue;
  if(target==blue && target-current<error)
    target = green;
   
   current = current + ((target-current)/easing);  //gradually change the color

  if(brightness >= 255) brightness_delta = -3;
  if(brightness <= 0) brightness_delta = 3;
  brightness += brightness_delta;
  
  for (int i=0; i<NUM_LEDS; i++){
    leds[i] = CRGB(current.g*brightness/255, current.r*brightness/255, current.b*brightness/255);  
  }
}

void handle_state1(){
  
   current = current + ((target-current)/easing);  //gradually change the color

  if(brightness >= 255) brightness_delta = -3;
  if(brightness <= 0) brightness_delta = 3;
  brightness += brightness_delta;
  
  for (int i=0; i<NUM_LEDS; i++){
    leds[i] = CRGB(current.g*brightness/255, current.r*brightness/255, current.b*brightness/255);  
  }
}

void handle_state2(){
  if(target==orange && target-current<error)
    target = white;
  if(target==white && target-current<error)
    target = orange;
   
   current = current + ((target-current)/easing);  //gradually change the color

  if(brightness >= 255) brightness_delta = -3;
  if(brightness <= 0) brightness_delta = 3;
  brightness += brightness_delta;
  
  for (int i=0; i<NUM_LEDS; i++){
    leds[i] = CRGB(current.g*brightness/255, current.r*brightness/255, current.b*brightness/255);  
  }
}


void handle_state3(){
  
  for (int i=0; i<NUM_LEDS; i++){
    if (i > state2_loc) break;
    leds[i] = CRGB(blue.g*brightness/255, blue.r*brightness/255, blue.b*brightness/255);  
  }
  state2_loc+=2;
}

void handle_state4(){
  
   current = current + ((target-current)/easing);  //gradually change the color

  if(brightness >= 255) brightness_delta = -3;
  if(brightness <= 0) brightness_delta = 3;
  brightness += brightness_delta;
  
  for (int i=0; i<NUM_LEDS; i++){
    leds[i] = CRGB(current.g*brightness/255, current.r*brightness/255, current.b*brightness/255);  
  }
}

void handle_state5(){
  
   current = current + ((target-current)/easing);  //gradually change the color

  if(brightness >= 255) brightness_delta = -3;
  if(brightness <= 0) brightness_delta = 3;
  brightness += brightness_delta;
  
  for (int i=0; i<NUM_LEDS; i++){
    leds[i] = CRGB(current.g*brightness/255, current.r*brightness/255, current.b*brightness/255);  
  }
}

void handle_state6(){
  
  for (int i=0; i<NUM_LEDS; i++){
    if (i > state6_loc) break;
    leds[i] = CRGB(blue.g*brightness/255, blue.r*brightness/255, blue.b*brightness/255);  
  }
  state6_loc+=2;
}
