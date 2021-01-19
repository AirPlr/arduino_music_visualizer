#include <FastLED.h>

// Arduino Music Visualizer 0.2

// LED LIGHTING SETUP
#define LED_PIN     6
#define LED_PIN2    7
#define NUM_LEDS    46
#define NUM_LEDZ    300
#define BRIGHTNESS  90
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
CRGB ledz[NUM_LEDZ];
#define UPDATES_PER_SECOND 100

// AUDIO INPUT SETUP
int audio = A0; 

// STANDARD VISUALIZER VARIABLES
int loop_max = 0;
int k = 125; // COLOR WHEEL POSITION
int decay = 0; // HOW MANY MS BEFORE ONE LIGHT DECAY
int decay_check = 0;
long pre_react = 0; // NEW SPIKE CONVERSION
long pre_reacter = 0;
long react = 0; // NUMBER OF LEDs BEING LIT
long react2 = 0;
long post_react = 0; // OLD SPIKE CONVERSION
long post_reacter = 0;
int midway=23;
// RAINBOW WAVE SETTINGS
int wheel_speed = 1;

void setup()
{
  // LED LIGHTING SETUP
  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<LED_TYPE, LED_PIN2, COLOR_ORDER>(ledz, NUM_LEDZ).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );

  // CLEAR LEDS
  for (int i = 0; i < NUM_LEDS; i++)
    leds[i] = CRGB(0, 0, 0);
    
  FastLED.show();

  for (int f = 0; f < NUM_LEDS; f++)
    ledz[f] = CRGB(0, 0, 0);
    
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

// FUNCTION TO GET AND SET COLOR
// THE ORIGINAL FUNCTION WENT BACKWARDS
// THE MODIFIED FUNCTION SENDS WAVES OUT FROM FIRST LED
// https://github.com/NeverPlayLegit/Rainbow-Fader-FastLED/blob/master/rainbow.ino
void rainbow()
{
  for(int i = NUM_LEDZ - 1; i >= 0; i--) {
    if (i < react)
      ledz[i] = Scroll((i * 256 / 50 + k) % 256);
    else
      ledz[i] = CRGB(0, 0, 0);      
  }
  FastLED.show(); 
}

void doubleRainbow()
{
  for(int i = NUM_LEDS - 1; i >= midway; i--) {
    if (i < react + midway) {
      leds[i] = Scroll((i * 256 / 50 + k) % 256);
      leds[(midway - i) + midway] = Scroll((i * 256 / 50 + k) % 256);
    }
    else
      leds[i] = CRGB(0, 0, 0);
      leds[midway - react] = CRGB(0, 0, 0);
  }
  FastLED.show();
}

void loop()
{
  int audio_input = (analogRead(audio)*8)/6; // ADD x2 HERE FOR MORE SENSITIVITY  
  int audio_inputz = analogRead(audio)*8; // ADD x2 HERE FOR MORE SENSITIVITY
  if (audio_input > 0)
  {
    pre_react = ((long)NUM_LEDS * (long)audio_input) / 1023L; // TRANSLATE AUDIO LEVEL TO NUMBER OF LEDs

    if (pre_react > react) // ONLY ADJUST LEVEL OF LED IF LEVEL HIGHER THAN CURRENT LEVEL
      react = pre_react;

    Serial.print(audio_input);
    Serial.print(" -> ");
    Serial.println(pre_react);
  }
if (audio_inputz > 0)
  {
    
    pre_reacter = ((long)NUM_LEDZ * (long)audio_inputz) /1023L;

     if (pre_reacter > react2) // ONLY ADJUST LEVEL OF LED IF LEVEL HIGHER THAN CURRENT LEVEL
      react2 = pre_reacter;

    Serial.print(audio_inputz);
    Serial.print(" -> ");
    Serial.println(pre_reacter);
  }
  doubleRainbow(); // APPLY COLOR
  rainbow();

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
