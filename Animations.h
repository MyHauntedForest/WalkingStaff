#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

void twinkle(byte r, byte g, byte b);
void twinkle(uint32_t color);
void fill(byte r, byte g, byte b);
void fadeToBlack(int ledNo, byte fadeValue);
void rainbow(int wait);
void meteorRain(byte red, byte green, byte blue, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedfancyDelay);
