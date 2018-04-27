#include <FastLED.h>
#include <math.h>
#define NUM_LEDS 60
#define TOTAL_NUM_LEDS 120
#define DATA_PIN1 7
#define DATA_PIN2 6

#define RCWLPin A3
#define TOUCHPin1 A5
#define TOUCHPin2 A6
#define RCWLTHRESHOLD 700
#define TOUCHTHRESHOLD 700
#define RCWLLEDPin 2
#define TOUCH1LEDPin 3
#define TOUCH2LEDPin 4

CRGB leds_low[NUM_LEDS];
CRGB leds_high[NUM_LEDS];

int brightness=0, brightness_delta=1; //delta為亮度變化量
float easing = 30;  //讓current逐漸變成target

int state;

int state21_loc;
int state13_loc;

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

struct Color pink = {250, 100, 50};

struct Color current = {0, 0, 0}; //current color setting
struct Color target = {0, 0, 0};  //target color setting
struct Color error = {2, 2, 2}; //if current is very close to the target

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2813, DATA_PIN1, RGB>(leds_low, NUM_LEDS);
  FastLED.addLeds<WS2813, DATA_PIN2, RGB>(leds_high, NUM_LEDS);
  state = 1;
  current = blue;
  target = green;
}

void loop() {
  switch(state){
    case 1:
      handle_state1();
      break;
    case 2:
      handle_state2();
      break;
    case 3:
      handle_state3();
      break;
    case 12:
      handle_state12();
      break;
    case 21:
	    handle_state2();
      handle_state21();
      break;
    case 13:
      handle_state1();
	  handle_state13();
      break;
	case 31:
	  handle_state3();
	  handle_state21();
      break;
    case 23:
      handle_state2();
      handle_state13();
      break;
	case 32:
      handle_state12();
      break;
  }
  
  int rcwl = analogRead(RCWLPin);
  int touch1 = analogRead(TOUCHPin1);
  int touch2 = analogRead(TOUCHPin2);
  
  bool rcwl_bool = (rcwl > RCWLTHRESHOLD);
  bool touch1_bool = (touch1 > TOUCHTHRESHOLD);
  bool touch2_bool = (touch2 > TOUCHTHRESHOLD);
  bool touch_bool = (touch1_bool || touch2_bool);
  
  digitalWrite(RCWLLEDPin, (rcwl_bool) ? HIGH : LOW);
  digitalWrite(TOUCH1LEDPin, (touch1_bool) ? HIGH : LOW);
  digitalWrite(TOUCH2LEDPin, (touch2_bool) ? HIGH : LOW);
  
  switch(state){
	  case 1:
	    if (touch_bool){
			state = 13;
			state13_loc = 120;
		}
		else if (rcwl_bool){
			state = 12;
			target = orange;
			easing = 5;
		}
		else state = 1;
		break;
	  case 2:
	    if (touch_bool){
			state = 23;
			state13_loc = 120;
		}
		else if (!rcwl_bool){
			state = 21;
			state21_loc = 0;
		}
		else state = 2;
		break;
	  case 3:
	    if (!touch_bool){
			if (rcwl_bool){
				state = 32;
				target = orange;
				easing = 5;
			}
			else {
				state = 31;
				state21_loc = 0;
			}
		}
		else state = 3;
		break;
	  case 21:
	  case 31:
	    if (state21_loc > TOTAL_NUM_LEDS+15){
			state = 1;
			current = blue;
      target = blue;
			easing = 30;
			brightness = 255;
		}
		break;
	  case 12:
	  case 32:
		if (target-current<error){
			state = 2;
			easing = 30;
		}
		break;
	  case 13:
	  case 23:
	    if (state13_loc < -25){
			state = 3;
			current = pink;
			brightness = 255;
		}
		break;
  }
  /*
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
  */
  Serial.print(current.r);
  Serial.print(" ");
  Serial.print(current.g);
  Serial.print(" ");
  Serial.print(current.b);
  Serial.print(" ");
  Serial.print(state);
  Serial.print(" ");
  Serial.println(rcwl);
  FastLED.show();
  delay(50);
}

void handle_state1(){
  if(target==green && target-current<error)
    target = blue;
  if(target==blue && target-current<error)
    target = green;
   
   current = current + ((target-current)/easing);  //gradually change the color

  if(brightness >= 255) brightness_delta = -5;
  if(brightness <= 0) brightness_delta = 5;
  brightness += brightness_delta;
  
  for (int i=0; i<NUM_LEDS; i++){
    leds_low[i] = CRGB(current.g*brightness/255, current.r*brightness/255, current.b*brightness/255);
	leds_high[i] = CRGB(current.g*brightness/255, current.r*brightness/255, current.b*brightness/255);
  }
}

void handle_state12(){
  current = current + ((target-current)/easing);  //gradually change the color

  if(brightness >= 255) brightness_delta = -5;
  if(brightness <= 0) brightness_delta = 5;
  brightness += brightness_delta;
  
  for (int i=0; i<NUM_LEDS; i++){
    leds_low[i] = CRGB(current.g*brightness/255, current.r*brightness/255, current.b*brightness/255);
	leds_high[i] = CRGB(current.g*brightness/255, current.r*brightness/255, current.b*brightness/255);
  }
}

void handle_state2(){
  if(target==orange && target-current<error)
    target = white;
  if(target==white && target-current<error)
    target = orange;
   
   current = current + ((target-current)/easing);  //gradually change the color

  if(brightness >= 255) brightness_delta = -5;
  if(brightness <= 0) brightness_delta = 5;
  brightness += brightness_delta;
  
  for (int i=0; i<NUM_LEDS; i++){
    leds_low[i] = CRGB(current.g*brightness/255, current.r*brightness/255, current.b*brightness/255);  
	  leds_high[i] = CRGB(current.g*brightness/255, current.r*brightness/255, current.b*brightness/255);
  }
}


void handle_state21(){
  for (int i=0; i<NUM_LEDS; i++){
    if (i > state21_loc) break;
	int diffval = state21_loc-i;
	if (diffval < 10) leds_low[i] = CRGB(blue.g*(diffval)/10, blue.r*(diffval)/10, blue.b*(diffval)/10);
    else leds_low[i] = CRGB(blue.g, blue.r, blue.b);
  }
  for (int i=0; i<NUM_LEDS; i++){
    if (i+NUM_LEDS > state21_loc) break;
	int diffval = state21_loc-(i+NUM_LEDS);
	if (diffval < 10) leds_high[i] = CRGB(blue.g*(diffval)/10, blue.r*(diffval)/10, blue.b*(diffval)/10);
    else leds_high[i] = CRGB(blue.g, blue.r, blue.b);
  }
  state21_loc+=3;
}

void handle_state3(){
  if(brightness >= 255) brightness_delta = -3;
  if(brightness <= 100) brightness_delta = 3;
  brightness += brightness_delta;
  
  for (int i=0; i<NUM_LEDS; i++){
    leds_low[i] = CRGB(current.g*brightness/255, current.r*brightness/255, current.b*brightness/255);
	leds_high[i] = CRGB(current.g*brightness/255, current.r*brightness/255, current.b*brightness/255);
  }
}

void handle_state13(){
  for (int i=NUM_LEDS-1; i>=0; i--){
    if (i < state13_loc) break;
    leds_low[i] = CRGB(pink.g, pink.r, pink.b);
    /*
	  int diffval = state13_loc-i;
	  if (diffval > 10) leds_low[i] = CRGB(pink.g*(diffval)/10, pink.r*(diffval)/10, pink.b*(diffval)/10);
    else leds_low[i] = CRGB(pink.g, pink.r, pink.b);
    */
  }
  for (int i=NUM_LEDS-1; i>=0; i--){
    if (i+NUM_LEDS < state13_loc) break;
    leds_high[i] = CRGB(pink.g, pink.r, pink.b);
    /*
	  int diffval = state13_loc-i;
	  if (diffval > 10) leds_high[i] = CRGB(pink.g*(diffval)/10, pink.r*(diffval)/10, pink.b*(diffval)/10);
    else leds_high[i] = CRGB(pink.g, pink.r, pink.b);
    */
  }
  state13_loc-=2;
}
