#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define LedStripPin 32
#define potTime 33
#define LedNumber 32
#define maxDelay 30000    // set stopwatch time
#define lightOnDelay 2000 // time all LEDs are on

int timeDelay;
unsigned long timePrevious = 0;
unsigned long timePreviousON = 0;
unsigned long timePreviousInfinite = 0;
int previousColor = -1;
int newColor;
bool stopwatchTrigger = true;
bool ledsOFF = true;

Adafruit_NeoPixel strip(LedNumber, LedStripPin, NEO_GRB + NEO_KHZ800);

void enableLED(int color);

void setup()
{
  strip.begin();
  pinMode(potTime, INPUT);
  strip.clear();
  enableLED(previousColor);
  Serial.begin(115200);
}

void loop()
{
  timeDelay = map(analogRead(potTime), 5, 4095, 1000, maxDelay); // set stopwatch timer
  unsigned long timeNow = millis();

  if (timeNow - timePrevious >= timeDelay and stopwatchTrigger and timeDelay >= 3000)
  { // sorting random color with pot
    newColor = random(1, 4);

    while (previousColor == newColor)
    {
      newColor = random(1, 4);
    }
    previousColor = newColor;
    enableLED(newColor);
    ledsOFF = true;
    timePreviousON = timeNow;
    timePrevious = timeNow;
  }

  if (timeNow - timePreviousInfinite >= 2000 and stopwatchTrigger and timeDelay < 3000)
  { // sorting random color 2 seconds
    newColor = random(1, 4);

    while (previousColor == newColor)
    {
      newColor = random(1, 4);
    }
    previousColor = newColor;
    enableLED(newColor);
    timePreviousInfinite = timeNow;
  }

  if (timeNow - timePreviousON >= lightOnDelay and ledsOFF)
  { // turn off all leds after delay
    enableLED(4);
    ledsOFF = false;
  }

  // if (digitalRead(remoteKey) == 1)
  // {
  //   stopwatchTrigger = false;
  //   newColor = random(1, 4);
  //   Serial.println(newColor);
  //   if (previousColor == -1)
  //   {
  //     previousColor = newColor;
  //   }
  //   while (previousColor == newColor)
  //   {
  //     newColor = random(1, 4);
  //   }
  //   previousColor = newColor;
  //   enableLED(newColor);
  //   ledsOFF = true;
  //   timePreviousON = timeNow;
  //   timePrevious = timeNow;
  // }
  stopwatchTrigger = true;
  // Serial.println(timeDelay);
}

void enableLED(int color)
{ // is 1 = red, is 2 = blue, is 3 = green, is 4> = off led strip
  // strip.clear();
  Serial.println("Piscou LED");
  switch (color)
  {
  case 1: // is red
    for (int i = 0; i < LedNumber; i++)
    {
      strip.setPixelColor(i, strip.Color(255, 0, 0));
      strip.show();
    }
    break;

  case 2: // is green
    for (int i = 0; i < LedNumber; i++)
    {
      strip.setPixelColor(i, strip.Color(0, 255, 0));
      strip.show();
    }
    break;

  case 3: // is blue
    for (int i = 0; i < LedNumber; i++)
    {
      strip.setPixelColor(i, strip.Color(0, 0, 255));
      strip.show();
    }
    break;

  default:
    for (int i = 0; i < LedNumber; i++)
    {
      strip.setPixelColor(i, strip.Color(0, 0, 0));
      strip.show();
    }
    break;
  }
}
