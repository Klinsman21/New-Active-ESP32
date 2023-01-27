#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define LedStripPin 32
#define potTime 33
#define LedNumber 30
#define maxDelay 30000    // set stopwatch time
#define lightOnDelay 2000 // time all LEDs are on

#define vSensorPin 34
#define chargingPin 36
#define fullChargePin 39

int timeDelay;
unsigned long timePrevious = 0;
unsigned long timePreviousON = 0;
unsigned long timePreviousInfinite = 0;
int previousColor = -1;
int newColor;
bool stopwatchTrigger = true;
bool ledsOFF = true;
bool showLedStatusOnStart = true;
unsigned long timeNow;

Adafruit_NeoPixel strip(LedNumber, LedStripPin, NEO_GRB + NEO_KHZ800);

void enableLED(int color);

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
float getBatteryVoltage()
{
  float voltage = 0;
  for (size_t i = 0; i < 10; i++)
  {
    voltage += ((float)analogRead(vSensorPin) / 4095) * 3.3 * 2 * 1.05;
    delay(5);
  }
  voltage = voltage / 10;

  return voltage;
}

int getChargerStatus() // 0 = carregando, 1 = carregado, 2 = no USB
{                      // false = carregando true = carregado
  if (digitalRead(chargingPin) == 0 and digitalRead(fullChargePin) == 1)
  {
    return 0;
  }
  else if (digitalRead(chargingPin) == 1 and digitalRead(fullChargePin) == 0)
  {
    return 1;
  }
  return 2;
}

void blinkLedStatus(bool opt, bool percent, float value) // opt == 0 charging, opt == true fullcharge
{
  if (percent)
  {
    if (value > 0 and value <= 3.5)
    {
      strip.clear();
      Serial.println("25%");
      strip.setPixelColor(0, strip.Color(0, 150, 0));
      strip.show();
    }
    else if (value > 3.5 and value <= 3.7)
    {
      strip.clear();
      Serial.println("50%");
      strip.setPixelColor(0, strip.Color(0, 150, 0));
      strip.show();
      strip.setPixelColor(4, strip.Color(0, 150, 0));
      strip.show();
    }
    else if (value > 3.7 and value <= 3.9)
    {
      strip.clear();
      Serial.println("75%");
      strip.setPixelColor(0, strip.Color(0, 150, 0));
      strip.show();
      strip.setPixelColor(4, strip.Color(0, 150, 0));
      strip.show();
      strip.setPixelColor(25, strip.Color(0, 150, 0));
      strip.show();
    }
    else if (value > 3.9)
    {
      strip.clear();
      Serial.println("100%");
      strip.setPixelColor(0, strip.Color(0, 150, 0));
      strip.show();
      strip.setPixelColor(4, strip.Color(0, 150, 0));
      strip.show();
      strip.setPixelColor(25, strip.Color(0, 150, 0));
      strip.show();
      strip.setPixelColor(29, strip.Color(0, 150, 0));
      strip.show();
    }
  }

  else
  {
    if (opt)
    {
      // enableLED(2);
      strip.clear();
      strip.setPixelColor(0, strip.Color(0, 100, 0));
      strip.show();
      strip.setPixelColor(4, strip.Color(0, 100, 0));
      strip.show();
      strip.setPixelColor(25, strip.Color(0, 100, 0));
      strip.show();
      strip.setPixelColor(29, strip.Color(0, 100, 0));
      strip.show();

      // strip.clear();
      // strip.setPixelColor(0, strip.Color(0, 100, 0));
      // strip.show();
      // strip.setPixelColor(1, strip.Color(0, 100, 0));
      // strip.show();
      // strip.setPixelColor(5, strip.Color(0, 100, 0));
      // strip.show();
      // strip.setPixelColor(3, strip.Color(0, 100, 0));
      // strip.show();
      // strip.setPixelColor(4, strip.Color(0, 100, 0));
      // strip.show();
      // strip.setPixelColor(9, strip.Color(0, 100, 0));
      // strip.show();
      // strip.setPixelColor(24, strip.Color(0, 100, 0));
      // strip.show();
      // strip.setPixelColor(29, strip.Color(0, 100, 0));
      // strip.show();
      // strip.setPixelColor(24, strip.Color(0, 100, 0));
      // strip.show();
      // strip.setPixelColor(28, strip.Color(0, 100, 0));
      // strip.show();
      // strip.setPixelColor(26, strip.Color(0, 100, 0));
      // strip.show();
      // strip.setPixelColor(25, strip.Color(0, 100, 0));
      // strip.show();
      // strip.setPixelColor(20, strip.Color(0, 100, 0));
      // strip.show();
    }
    else
    {
      // enableLED(1);
      strip.clear();
      // strip.setPixelColor(0, strip.Color(255, 0, 0));
      // strip.show();
      // strip.setPixelColor(4, strip.Color(255, 0, 0));
      // strip.show();
      // strip.setPixelColor(25, strip.Color(255, 0, 0));
      // strip.show();
      strip.setPixelColor(29, strip.Color(100, 0, 0));
      strip.show();

      // strip.clear();
      // strip.setPixelColor(0, strip.Color(100, 0, 0));
      // strip.show();
      // strip.setPixelColor(1, strip.Color(100, 0, 0));
      // strip.show();
      // strip.setPixelColor(5, strip.Color(100, 0, 0));
      // strip.show();
      // strip.setPixelColor(3, strip.Color(100, 0, 0));
      // strip.show();
      // strip.setPixelColor(4, strip.Color(100, 0, 0));
      // strip.show();
      // strip.setPixelColor(9, strip.Color(100, 0, 0));
      // strip.show();
      // strip.setPixelColor(24, strip.Color(100, 0, 0));
      // strip.show();
      // strip.setPixelColor(29, strip.Color(100, 0, 0));
      // strip.show();
      // strip.setPixelColor(24, strip.Color(100, 0, 0));
      // strip.show();
      // strip.setPixelColor(28, strip.Color(100, 0, 0));
      // strip.show();
      // strip.setPixelColor(26, strip.Color(100, 0, 0));
      // strip.show();
      // strip.setPixelColor(25, strip.Color(100, 0, 0));
      // strip.show();
      // strip.setPixelColor(20, strip.Color(100, 0, 0));
      // strip.show();
    }
  }
}

void blink()
{
  timeNow = millis();
  timeDelay = map(analogRead(potTime), 5, 4095, 1000, maxDelay); // set stopwatch timer

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
  stopwatchTrigger = true;
}

void enableLED(int color)
{ // is 1 = red, is 2 = blue, is 3 = green, is 4> = off led strip
  // strip.clear();
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

void setup()
{
  strip.begin();
  pinMode(potTime, INPUT);
  pinMode(vSensorPin, INPUT);
  pinMode(chargingPin, INPUT);
  pinMode(fullChargePin, INPUT);
  strip.clear();
  enableLED(previousColor);
  Serial.begin(115200);
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_39, 1);
}

void loop()
{
  if (getChargerStatus() == 1) // vefirica se a bateria está carregada
  {
    blinkLedStatus(true, false, 0);
  }
  else if (getChargerStatus() == 0) // verifica se a bateria está carregando
  {
    blinkLedStatus(false, false, 0);
  }
  else
  {
    if (showLedStatusOnStart)
    {
      blinkLedStatus(false, true, getBatteryVoltage());
      delay(2500);
      showLedStatusOnStart = false;
      strip.clear();
    }
    if (getBatteryVoltage() >= 3.5)
    {
      blink(); // pisca o led de forma normal, baseado no timer
    }
    else
    {
      Serial.println("Pediu carga");
      strip.clear();
      strip.setPixelColor(0, strip.Color(70, 70, 0));
      strip.show();
      strip.setPixelColor(4, strip.Color(70, 70, 0));
      strip.show();
      strip.setPixelColor(25, strip.Color(70, 70, 0));
      strip.show();
      strip.setPixelColor(29, strip.Color(70, 70, 0));
      strip.show();
      delay(58000);
      strip.setPixelColor(0, strip.Color(0, 0, 0));
      strip.show();
      strip.setPixelColor(4, strip.Color(0, 0, 0));
      strip.show();
      strip.setPixelColor(25, strip.Color(0, 0, 0));
      strip.show();
      strip.setPixelColor(29, strip.Color(0, 0, 0));
      strip.show();
      delay(2000);
      esp_deep_sleep_start();
    }
  }
}
// battv = ((float)analogRead(34) / 4095) * 3.3 * 2 * 1.05; Calcula tensão da bateria