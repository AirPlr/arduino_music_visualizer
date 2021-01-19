#include <FastLED.h>

// Impostazioni delle luci(Da cambiare a seconda del bisogno)
#define LED_PIN     6 // Interno-esterno
#define LED_PIN2    7 // Monodirezionale
#define NUM_LEDS    46 
#define NUM_LEDZ    300
#define BRIGHTNESS  90
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

//non cambiare
CRGB leds[NUM_LEDS];
CRGB ledz[NUM_LEDZ];
#define UPDATES_PER_SECOND 100



int audio = A0; //pin del jack audio 
int midway=23; // cambiare solo se ce n'è bisogno



int loop_max = 0;
int k = 125; 
int decay = 0; 
int decay_check = 0;
long pre_react = 0; 
long pre_reacter = 0;
long react = 0; 
long react2 = 0;
long post_react = 0; 
long post_reacter = 0;
int wheel_speed = 1;

void setup()
{
  // LED SETUP
  delay( 3000 );
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<LED_TYPE, LED_PIN2, COLOR_ORDER>(ledz, NUM_LEDZ).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );

  // Pulisci i led
  for (int i = 0; i < NUM_LEDS; i++)
    leds[i] = CRGB(0, 0, 0);
    
  FastLED.show();

  for (int f = 0; f < NUM_LEDS; f++)
    ledz[f] = CRGB(0, 0, 0);
    
  FastLED.show();


  // Setup di input e seriale
  Serial.begin(115200);
  pinMode(audio, INPUT);
  Serial.println("\nListening...");
}

// Funzione per generare un colore in base ad una ruota dei colori
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

// I led si accendono dal primo all'ultimo
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
 //I led si accendono dall'interno (Midway) verso l'esterno
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

//Funzionamento effettivo del programma (lettura del voltaggio e applicazione delle funzioni con uscita su monitor seriale dei valori)
void loop()
{
  int audio_input = (analogRead(audio)*8)/6; 
  int audio_inputz = analogRead(audio)*8;  
  if (audio_input > 0)
  {
    pre_react = ((long)NUM_LEDS * (long)audio_input) / 1023L; // Traduci il livello dell'audio nel numero di led

    if (pre_react > react) // Cambia react se è più piccolo del vecchio valore
      react = pre_react;

    Serial.print(audio_input); //Uscita sul monitor seriale
    Serial.print(" -> ");
    Serial.println(pre_react);
  }
if (audio_inputz > 0)
  {
    
    pre_reacter = ((long)NUM_LEDZ * (long)audio_inputz) /1023L;

     if (pre_reacter > react2) // Cambia react2 se è più piccolo del vecchio valore
      react2 = pre_reacter;

    Serial.print(audio_inputz);
    Serial.print(" -> ");
    Serial.println(pre_reacter);
  }
  doubleRainbow(); // Applica colore
  rainbow();

  k = k - wheel_speed; // Velocità della ruota del colore
  if (k < 0) // Resetta la ruota del colore
    k = 255;

  // Rimuovi tutto
  decay_check++;
  if (decay_check > decay)
  {
    decay_check = 0;
    if (react > 0)
      react--;
  }
  //delay(2);
}
