#include <Adafruit_NeoPixel.h>

// Arduino Music Visualizer 0.2

// LED LIGHTING SETUP
#define LED_PIN     7
#define LED_TYPE    NEO_GRB + NEO_KHZ800
#define NUM_LEDS    29


Adafruit_NeoPixel pixels(NUM_LEDS, LED_PIN, LED_TYPE);

#define midway 15
int BRIGHTNESS = 90;
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
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
char inChar=' ';

void setup()
{
  // LED LIGHTING SETUP
  delay( 3000 ); // power-up safety delay
  pixels.begin();
  pixels.setBrightness(BRIGHTNESS);

  // CLEAR LEDS
  for (int i = 0; i < NUM_LEDS; i++)
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    
  pixels.show();


  // SERIAL AND INPUT SETUP
  Serial.begin(115200);
  pinMode(audio, INPUT);
  Serial.println("\nListening...");
}

// FUNCTION TO GENERATE COLOR BASED ON VIRTUAL WHEEL
// https://github.com/NeverPlayLegit/Rainbow-Fader-FastLED/blob/master/rainbow.ino
uint32_t Scroll(int pos) {
  uint8_t r, g, b;
  if(pos < 85) {
    g = 0;
    r = ((float)pos / 85.0f) * 255.0f;
    b = 255 - r;
  } else if(pos < 170) {
    g = ((float)(pos - 85) / 85.0f) * 255.0f;
    r = 255 - g;
    b = 0;
  } else if(pos < 256) {
    b = ((float)(pos - 170) / 85.0f) * 255.0f;
    g = 255 - b;
    r = 1;
  }
  return (uint32_t)r << 16 | (uint32_t)g << 8 | (uint32_t)b;
}

// Uncomment the one you want
void rainbow()
{
  BRIGHTNESS=90;
  for(int i = NUM_LEDS - 1; i >= 0; i--) {
    if (i < react)
      pixels.setPixelColor(i, Scroll((i * 256 / 50 + k) % 256));
    else
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));      
  }
    pixels.setBrightness(BRIGHTNESS);

  pixels.show(); 
}
void showl(int r, int g, int b){
  BRIGHTNESS=90;
    for(int i = NUM_LEDS - 1; i >= 0; i--) {
      if (i<react) pixels.setPixelColor(i, pixels.Color(r, g, b));
      else pixels.setPixelColor(i, pixels.Color(0, 0, 0));
}
  pixels.setBrightness(BRIGHTNESS);

pixels.show();}

void mode8(bool var){

for(int i = NUM_LEDS - 1; i >= 0; i--) {
      if (var) pixels.setPixelColor(i, Scroll((i * 256 / 50 + k) % 256));
      else pixels.setPixelColor(i, pixels.Color(255, 255, 255));
  }
  BRIGHTNESS=react*5;
  pixels.setBrightness(BRIGHTNESS);
  pixels.show(); 

}


void loop(){
  int audio_input = 0;
  if (Serial.available()) {
        String input = Serial.readStringUntil('p');
        if (input.toInt() > 0) {
            int num = input.toInt();
            audio_input = num;
        } else {
            inChar = input[0];
        }
    }

  if (audio_input > 0)
  {
    pre_react = audio_input;

    if (pre_react > react) // ONLY ADJUST LEVEL OF LED IF LEVEL HIGHER THAN CURRENT LEVEL
      react = pre_react;

    
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
