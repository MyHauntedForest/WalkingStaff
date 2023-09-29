#include <Adafruit_NeoPixel.h>
#include <Bounce2.h>
#include "Animations.h"
#include "WalkingStaff.h"

#define PIN_LED D0
#define PIN_BUTTON D1
#define VIRTUAL_GND_PIN D2

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN_LED, NEO_GRB + NEO_KHZ800);

enum State
{
  RAINBOW,
  OLD_EFFECT_TWINKLE,
  OLD_EFFECT_METEOR,
  DISCO,
  SOLID_ORANGE,
  SOLID_RED,
  SOLID_WHITE,
  OFF
};

State currentState = RAINBOW;

int hue = 0;

Bounce button = Bounce();

void setup()
{
  // Serial.begin(9600);

  randomSeed(analogRead(A8));

  pinMode(VIRTUAL_GND_PIN, OUTPUT);
  digitalWrite(VIRTUAL_GND_PIN, LOW); // make 8 a "GND" pin

  button.attach(PIN_BUTTON, INPUT_PULLUP); // Attach the debouncer to a pin with INPUT_PULLUP mode
  button.interval(25);                     // Use a debounce interval of 25 milliseconds

  strip.begin(); //  intialize neopixel strip
  strip.setBrightness(255);
  // strip.setBrightness(128); // half brightness
  strip.show(); // Initialize all pixels to 'off'

  fill(255, 0, 0);
  fancyDelay(500);
  fill(0, 255, 0);
  fancyDelay(500);
  fill(0, 0, 255);
  fancyDelay(500);
}

void loop()
{
  updateButton();
  updateAnimation();
}

void updateButton()
{
  button.update(); // Update the Bounce instance
  if (button.fell())
  {
    switchEffect();
  }
}

void updateAnimation()
{

  if (currentState == RAINBOW)
  {

    if (random(10) == 9)
    {
      hue++;
    }

    if (hue > 255)
    {
      hue = 0;
    }

    uint16_t newHue = mapStupidHack(hue, 0, 255, 0, 65535);

    twinkle(strip.ColorHSV(newHue));
  }
  else if (currentState == OLD_EFFECT_METEOR)
  {
    meteorRain(255, 115, 0, 10, 64, true, 30);
  }
  else if (currentState == OLD_EFFECT_TWINKLE)
  {
    twinkle(255, 115, 0);
  }
  else if (currentState == DISCO)
  {
    uint16_t randomHue = random(65535);
    twinkle(strip.ColorHSV(randomHue));
  }
  else if (currentState == SOLID_ORANGE)
  {
    fill(255, 115, 0);
  }
  else if (currentState == SOLID_WHITE)
  {
    fill(128, 128, 128);
  }
  else if (currentState == SOLID_RED)
  {
    fill(255, 0, 0);
  }
  else if (currentState == OFF)
  {
    fill(0, 0, 0);
  }
}

void switchEffect()
{
  switch (currentState)
  {
  case RAINBOW:
    currentState = OLD_EFFECT_TWINKLE;
    break;
  case OLD_EFFECT_TWINKLE:
    currentState = OLD_EFFECT_METEOR;
    break;
  case OLD_EFFECT_METEOR:
    currentState = DISCO;
    break;
  case DISCO:
    currentState = SOLID_ORANGE;
    break;
  case SOLID_ORANGE:
    currentState = SOLID_WHITE;
    break;
  case SOLID_WHITE:
    currentState = SOLID_RED;
    break;
  case SOLID_RED:
    currentState = OFF;
    break;
  case OFF:
    currentState = RAINBOW;
    break;
  }
}

uint16_t mapStupidHack(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

bool fancyDelay(long ms)
{
  State lastState = currentState;
  unsigned long start = millis();
  unsigned long end = start + ms;
  while (millis() < end)
  {
    updateButton();
  }
  return currentState != lastState;
}