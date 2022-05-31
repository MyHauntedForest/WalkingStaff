#include "Animations.h"
#include "WalkingStaff.h"

// modified from https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/

void twinkle(byte r, byte g, byte b)
{
    twinkle(strip.Color(r, g, b));
}

void twinkle(uint32_t color)
{

    for (int i = 0; i < NUM_LEDS; i++)
    {

        if (random(50) > 48)
        {
            strip.setPixelColor(i, color);
        }

        fadeToBlack(i, 30);
    }
    strip.show();
    fancyDelay(30);
}

void fill(byte r, byte g, byte b)
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        strip.setPixelColor(i, r, g, b);
    }
    strip.show();
}

void fadeToBlack(int ledNo, byte fadeValue)
{
    uint32_t oldColor;
    uint8_t r, g, b;

    oldColor = strip.getPixelColor(ledNo);
    r = (oldColor & 0x00ff0000UL) >> 16;
    g = (oldColor & 0x0000ff00UL) >> 8;
    b = (oldColor & 0x000000ffUL);

    r = (r <= 10) ? 0 : (int)r - (r * fadeValue / 256);
    g = (g <= 10) ? 0 : (int)g - (g * fadeValue / 256);
    b = (b <= 10) ? 0 : (int)b - (b * fadeValue / 256);

    strip.setPixelColor(ledNo, r, g, b);
}

// Rainbow cycle along whole strip. Pass fancyDelay time (in ms) between frames.
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
        if (fancyDelay(wait))
        {
            return;
        }
    }
}

void meteorRain(byte red, byte green, byte blue, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedfancyDelay)
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
        if (fancyDelay(SpeedfancyDelay))
        {
            return;
        }
    }
}