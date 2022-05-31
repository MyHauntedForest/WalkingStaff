#include <Adafruit_NeoPixel.h>
#include <BMI160.h>
#include <CurieIMU.h>
#include <Bounce2.h>
#include "Animations.h"
#include "WalkingStaff.h"

#define PIN 6

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

long globaltimer = 0;
int tap = 0;

enum State
{
  RAINBOW,
  OLD_EFFECT_TWINKLE,
  OLD_EFFECT_METEOR,
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
  Serial.begin(9600);

  pinMode(8, OUTPUT);
  digitalWrite(8, LOW); // make 8 a "GND" pin

  button.attach(9, INPUT_PULLUP); // Attach the debouncer to a pin with INPUT_PULLUP mode
  button.interval(25);            // Use a debounce interval of 25 milliseconds

  globaltimer = millis();

  CurieIMU.begin();
  CurieIMU.attachInterrupt(eventCallback);

  /* Enable Shock Detection */
  CurieIMU.setAccelerometerRange(4);                     // https://docs.arduino.cc/retired/archived-libraries/CurieIMU#setaccelerometerrange
  CurieIMU.setDetectionThreshold(CURIE_IMU_SHOCK, 3000); // 1.5g = 1500 mg //https://docs.arduino.cc/retired/archived-libraries/CurieIMU#setdetectionthreshold
  CurieIMU.setDetectionDuration(CURIE_IMU_SHOCK, 50);    // milliseconds of spike required to call interupt
  CurieIMU.interrupts(CURIE_IMU_SHOCK);

  strip.begin();            //  intialize neopixel strip
  strip.setBrightness(128); // half brightness
  strip.show();             // Initialize all pixels to 'off'

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

  // every 1000 ms, lets reset the tap timer
  if (millis() - globaltimer > 1000)
  {
    globaltimer = millis();
    tap = 0;
  }

  updateAnimation();
}

void updateButton()
{
  button.update(); // Update the Bounce instance
  if (button.fell())
  {
    switchStateAll();
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
    // twinkle(255, 0, 0);
    // twinkle(0, 255, 0);
    // twinkle(0, 0, 255);
  }
  else if (currentState == OLD_EFFECT_METEOR)
  {
    meteorRain(255, 115, 0, 10, 64, true, 30);
  }
  else if (currentState == OLD_EFFECT_TWINKLE)
  {
    twinkle(255, 115, 0);
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

void switchStateAll()
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

void switchState2()
{
  switch (currentState)
  {
  case RAINBOW:
    currentState = SOLID_RED;
    break;
  case SOLID_RED:
    currentState = SOLID_WHITE;
    break;
  case SOLID_WHITE:
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

static void eventCallback(void)
{
  if (CurieIMU.getInterruptStatus(CURIE_IMU_SHOCK))
  {
    if (CurieIMU.shockDetected(X_AXIS, POSITIVE))
    {
      tap++;
      globaltimer = millis();

      if (tap >= 2)
      {
        switchState2();
      }
      // switchState2();
    }
  }
}