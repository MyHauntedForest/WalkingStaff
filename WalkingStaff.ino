#include <Adafruit_NeoPixel.h>
#include <BMI160.h>
#include <CurieIMU.h>

#define PIN 6
#define NUM_LEDS 60
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

bool tap = false;

void setup()
{

  /* Initialise the IMU */
  CurieIMU.begin();
  CurieIMU.attachInterrupt(eventCallback);

  /* Enable Shock Detection */
  CurieIMU.setDetectionThreshold(CURIE_IMU_SHOCK, 1500); // 1.5g = 1500 mg
  CurieIMU.setDetectionDuration(CURIE_IMU_SHOCK, 50);    // milliseconds of spike required to call interupt
  CurieIMU.interrupts(CURIE_IMU_SHOCK);

  // Go green for 3 seconds to let us know we have turned on the staff
  strip.begin();
  fill(0, 255, 0);
  strip.show();
  delay(3000);
  fill(0, 0, 0);
  strip.show();
}

void loop()
{
  doSpells();

  // rainbow(10);
}

void doSpells()
{
  if (tap)
  {
    tap = false;
    meteorRain(255, 115, 0, 10, 64, true, 30);
    delay(1000);
  }
}

static void eventCallback(void)
{
  if (CurieIMU.getInterruptStatus(CURIE_IMU_SHOCK))
  {
    if (CurieIMU.shockDetected(X_AXIS, POSITIVE))
    {
      tap = true;
    }

    // if (CurieIMU.shockDetected(X_AXIS, NEGATIVE))
    // {
    //   tap = true;
    // }
  }
}

void meteorRain(byte red, byte green, byte blue, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay)
{
  fill(0, 0, 0);

  for (int i = 0; i < NUM_LEDS + NUM_LEDS; i++)
  {

    // fade brightness all LEDs one step
    for (int j = 0; j < NUM_LEDS; j++)
    {
      if ((!meteorRandomDecay) || (random(10) > 5))
      {
        fadeToBlack(j, meteorTrailDecay);
      }
    }

    // draw meteor
    for (int j = 0; j < meteorSize; j++)
    {
      if ((i - j < NUM_LEDS) && (i - j >= 0))
      {
        strip.setPixelColor(i - j, red, green, blue);
      }
    }

    strip.show();
    delay(SpeedDelay);
  }
}

void fill(byte r, byte g, byte b)
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    strip.setPixelColor(i, strip.Color(r, g, b));
  }
}

void fadeToBlack(int ledNo, byte fadeValue)
{
  uint32_t oldColor;
  uint8_t r, g, b;
  int value;

  oldColor = strip.getPixelColor(ledNo);
  r = (oldColor & 0x00ff0000UL) >> 16;
  g = (oldColor & 0x0000ff00UL) >> 8;
  b = (oldColor & 0x000000ffUL);

  r = (r <= 10) ? 0 : (int)r - (r * fadeValue / 256);
  g = (g <= 10) ? 0 : (int)g - (g * fadeValue / 256);
  b = (b <= 10) ? 0 : (int)b - (b * fadeValue / 256);

  strip.setPixelColor(ledNo, r, g, b);
}

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(int wait)
{
  for (long firstPixelHue = 0; firstPixelHue < 5 * 65536; firstPixelHue += 256)
  {
    for (int i = 0; i < strip.numPixels(); i++)
    {
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show();
    delay(wait);
  }
}