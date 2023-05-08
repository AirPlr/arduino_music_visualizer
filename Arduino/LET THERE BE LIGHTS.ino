#include <FastLED.h>

// Arduino Music Visualizer 0.2

// LED LIGHTING SETUP
#define LED_PIN     7
#define NUM_LEDS    29
#define midway 15
int BRIGHTNESS = 90;
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
#define UPDATES_PER_SECOND 100

// AUDIO INPUT SETUP
int audio = A0; 
int mode=9;
// STANDARD VISUALIZER VARIABLES
int loop_max = 0;
int k = 125; // COLOR WHEEL POSITION
int decay = 0; // HOW MANY MS BEFORE ONE LIGHT DECAY
int decay_check = 0;
long pre_react = 0; // NEW SPIKE CONVERSION
long react = 0; // NUMBER OF LEDs BEING LIT
long post_react = 0; // OLD SPIKE CONVERSION
// RAINBOW WAVE SETTINGS
int wheel_speed = 1;

void setup()
{
  // LED LIGHTING SETUP
  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );

  // CLEAR LEDS
  for (int i = 0; i < NUM_LEDS; i++)
    leds[i] = CRGB(0, 0, 0);
    
  FastLED.show();


  // SERIAL AND INPUT SETUP
  Serial.begin(115200);
  pinMode(audio, INPUT);
  Serial.println("\nListening...");
}

// FUNCTION TO GENERATE COLOR BASED ON VIRTUAL WHEEL
// https://github.com/NeverPlayLegit/Rainbow-Fader-FastLED/blob/master/rainbow.ino
CRGB Scroll(int pos) {
  CRGB color (0,0,0);
  if(pos < 85) {
    color.g = 0;
    color.r = ((float)pos / 85.0f) * 255.0f;
    color.b = 255 - color.r;
  } else if(pos < 170) {
    color.g = ((float)(pos - 85) / 85.0f) * 255.0f;
    color.r = 255 - color.g;
    color.b = 0;
  } else if(pos < 256) {
    color.b = ((float)(pos - 170) / 85.0f) * 255.0f;
    color.g = 255 - color.b;
    color.r = 1;
  }
  return color;
}

// Uncomment the one you want
void rainbow()
{
  BRIGHTNESS=90;
  for(int i = NUM_LEDS - 1; i >= 0; i--) {
    if (i < react)
      leds[i] = Scroll((i * 256 / 50 + k) % 256);
    else
      leds[i] = CRGB(0, 0, 0);      
  }
    FastLED.setBrightness(BRIGHTNESS);

  FastLED.show(); 
}
void showl(int r, int g, int b){
  BRIGHTNESS=90;
    for(int i = NUM_LEDS - 1; i >= 0; i--) {
      if (i<react) leds[i]=CRGB(r,g,b);
      else leds[i]=CRGB(0,0,0);
}
  FastLED.setBrightness(BRIGHTNESS);

FastLED.show();}

void mode8(bool var){

for(int i = NUM_LEDS - 1; i >= 0; i--) {
      if (var) leds[i] = Scroll((i * 256 / 50 + k) % 256);
      else leds[i]=CRGB(255,255,255);
  }
  BRIGHTNESS=react*5;
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.show(); 

}


void loop(){
  int audio_input = (analogRead(audio)*8)/2; // ADD x2 HERE FOR MORE SENSITIVITY  
  if (audio_input > 0)
  {
    pre_react = ((long)NUM_LEDS * (long)audio_input) / 1023L; // TRANSLATE AUDIO LEVEL TO NUMBER OF LEDs

    if (pre_react > react) // ONLY ADJUST LEVEL OF LED IF LEVEL HIGHER THAN CURRENT LEVEL
      react = pre_react;

    if (Serial.available() > 0) // SERIAL INPUT FOR ADJUSTING SETTINGS
    {
      char inChar = (char)Serial.read();
      if (inChar == 'r')
        mode = 0;
      if (inChar == 'g')
      mode = 1;
      if (inChar == 'b')
        mode = 2;
      if (inChar == 'v')
        mode = 3;
      if (inChar == 'y')
        mode = 4;
      if (inChar == 'w')
        mode = 5;
      if (inChar == 'c')
        mode = 6;
      if (inChar == 'm')
        mode=7;
      if (inChar == 'f')
        mode=8;
      if (inChar == 'l')
        mode=9;
      if (inChar == 'a')
        mode=10;
    }
  }
// Uncomment the one you want
  // doubleRainbow(); // APPLY COLOR
  
  switch (mode)
  {
  case 0:
    showl(255,0,0);
    break;
  case 1:
    showl(0,255,0);
    break;
  case 2:
    showl(0,0,255);
    break;
  case 3:
    showl(255,0,255);
    break;
  case 4:
    showl(255,255,0);
    break;
  case 5:
    showl(255,255,255);
    break;
  case 6:
    showl(0,255,255);
    break;
  case 7:
    rainbow();
      break;
  case 8:
      mode8(false);
      break;
  case 9:
      mode8(true);
      break;
  } 

     
    

  

  k = k - wheel_speed; // SPEED OF COLOR WHEEL
  if (k < 0) // RESET COLOR WHEEL
    k = 255;

  // REMOVE LEDs
  decay_check++;
  if (decay_check > decay)
  {
    decay_check = 0;
    if (react > 0)
      react--;
  }
  //delay(2);
}
