#include <math.h>
#include "Wire.h"
#include <SerLCD.h>

// Thermistor stuff
const int thermistorPin = A0;
const float nominalResistance = 10000.0;
const float nominalTemperature = 25.0;
const float bCoefficient = 3950.0;
const float seriesResistor = 10000.0;

// 2 Buttons
const int colorButtonPin = 2;   // Button 1: cycles backlight color
const int unitButtonPin  = 3;   // Button 2: toggles Celsius / Fahrenheit
-
bool lastColorButtonState = HIGH;
bool lastUnitButtonState  = HIGH;

//  Colors for background
int colorIndex = 0;
const int colors[][3] = {
  {200, 200, 200},  // 0: White
  {255,   0,   0},  // 1: Red
  {  0, 255,   0},  // 2: Green
  {  0,   0, 255},  // 3: Blue
  {255, 165,   0},  // 4: Orange
  {128,   0, 128},  // 5: Purple
  {  0, 255, 255},  // 6: Cyan
  {255, 255,   0},  // 7: Yellow
};
const int numColors = 8;

bool showCelsius = true;  // true = Celsius, false = Fahrenheit

int loops = 0;

//object lcd
SerLCD lcd;

void setup() {
  Serial.begin(9600);

  pinMode(colorButtonPin, INPUT_PULLUP);
  pinMode(unitButtonPin,  INPUT_PULLUP);

  Wire.begin();
  lcd.begin(Wire);
  lcd.setContrast(5);
  lcd.setFastBacklight(colors[0][0], colors[0][1], colors[0][2]);

  // initial settup screen
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp Monitor");
  lcd.setCursor(0, 1);
  lcd.print("Initializing..."); //yeah.. thats right
  delay(2000);
  lcd.clear();
}

void loop() {
  //  Button 1 for Color Cycle 
  bool currentColorState = digitalRead(colorButtonPin);
  if (currentColorState == LOW && lastColorButtonState == HIGH) {
    colorIndex = (colorIndex + 1);
    if (colorIndex > 7){
      colorIndex = 0;
    }
     //% numColors;
    lcd.setFastBacklight(colors[colorIndex][0], colors[colorIndex][1], colors[colorIndex][2]);
    delay(50); // Debounce
    Serial.println("Color has changed!!!!!!");  
  }
  lastColorButtonState = currentColorState;

  // Button 2 for Units
  bool currentUnitState = digitalRead(unitButtonPin);
  if (currentUnitState == LOW && lastUnitButtonState == HIGH) {
    showCelsius = !showCelsius;  // Flip between C and F
    delay(50); // Debounce
  }
  lastUnitButtonState = currentUnitState;



  delay(1500);
  loops++;
  if(loops >= 100){
    thermistor();
    loops = 0;
  }
}

void thermistor(){
  // Thermistor Reading
  float adcReading = analogRead(thermistorPin);

  float resistance = (1023.0 / adcReading) - 1.0;
  resistance = seriesResistor / resistance;

  float temperatureC = resistance / nominalResistance;
  temperatureC = log(temperatureC);
  temperatureC /= bCoefficient;
  temperatureC += 1.0 / (nominalTemperature + 273.15);
  temperatureC = 1.0 / temperatureC;
  temperatureC -= 273.15;

  float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;

  // Serial Output 
  Serial.print("Temperature: ");
  Serial.print(temperatureC, 1);
  Serial.print(" *C  /  ");
  Serial.print(temperatureF, 1);
  Serial.println(" *F");

  // LCD OUTPUT
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temperature:");

  lcd.setCursor(0, 1);
  if (showCelsius) {
    lcd.print(temperatureC, 1);
    lcd.print(" C");
  } else {
    lcd.print(temperatureF, 1);
    lcd.print(" F");
  }
}
