/*
   An ARDUINO NANO aftermarket automotive temperature and voltage monitor
  Pedro Cabral 2019

  V3-16OUT2019

  CIRCUIT:
  5VDC regulated supply (L7805)
  LCD RS pin to digital pin 12
  LCD Enable pin to digital pin 11
  LCD D4 pin to digital pin 5
  LCD D5 pin to digital pin 4
  LCD D6 pin to digital pin 3
  LCD D7 pin to digital pin 2
  LCD R/W pin to ground
  LCD VSS pin to ground
  LCD VCC pin to 5V
  LCD LED anode through 250R to 7V
  LCD LED cathode to Q2 collector
  ARDUINO D5 through 390R to Q2 base
  Q2 emitter to GND
  LCD V0 (contrast) to GND via 3k3 resistor
*/

// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to

const int StandardDelay = 400;
const int MeasureDelay = 125;
const int SplashScreenDelay = 3000;
const int Overtemp1 = 90;
const int Overtemp2 = 80;
const int MinBrightness = 5;
const int MaxBrightness = 254;
const int Lum = 0;

const int rs = 12;
const int en = 11;
const int d4 = 5;
const int d5 = 4;
const int d6 = 3;
const int d7 = 2;
const int WarnLedPin = 13;

const int BrightnessPin = 9; //LCD brightness set pin (PWM)

int VoltPin = A7; //Input from the voltage measurement voltage divider
int LdrPin = A6; //Input from the luminosity measurement LDR voltage divider
int Th1Pin = A5; //Input from the temperature sensor nr.1
int Th2Pin = A4; //Input from the temperature sensor nr.2

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {

  Serial.begin(9600);

  pinMode(VoltPin, INPUT);
  pinMode(LdrPin, INPUT);
  pinMode(Th1Pin, INPUT);
  pinMode(Th2Pin, INPUT);

  pinMode(BrightnessPin, OUTPUT);

  lcd.begin(16, 2); // set up the LCD's number of columns and rows:
}

void loop() {

  //SPLASH SCREEN
  while ( millis() <= SplashScreenDelay) {
    digitalWrite(WarnLedPin, HIGH);
    analogWrite (BrightnessPin, MaxBrightness);  //Sets the brightness of the LCD for the duration of the splash screen
    while ( millis() <= 1000) {
      lcd.setCursor(0, 0);
      lcd.print("CARCOMP V3"); //Print the version number to the LCD
      lcd.setCursor(0, 1);
      lcd.print("P.Cabral OUT2019");
    }
    lcd.setCursor(0, 0);
    lcd.print("Ola Catarina!!!"); // Print a message to the LCD.
    lcd.setCursor(0, 1);
    lcd.print("Boa viagem......"); // Print a message to the LCD.
  }

  digitalWrite(WarnLedPin, LOW);

  //LCD HEADER
  lcd.setCursor(0, 0);
  lcd.print("Bater. Agua Oleo "); // Print the headers to the LCD.

  //SENSOR DATA ACQUISITION
  float VoltValue_1 = (analogRead(VoltPin));  //Reads the voltage divider value, sample 1
  int LumValue_1 = (analogRead(LdrPin)); //Reads the LDR value, sample 1
  int Temp1Value_1 = (analogRead(Th1Pin));//Reads and calculates the temperature 1 value, sample 1
  int Temp2Value_1 = (analogRead(Th2Pin));//Reads and calculates the temperature 2 value, sample 1
  delay (MeasureDelay);//Waits before next measurement run

  float VoltValue_2 = (analogRead(VoltPin));  //Reads the voltage divider value, sample 2
  int LumValue_2 = (analogRead(LdrPin)); //Reads the LDR value, sample 2
  int Temp1Value_2 = (analogRead(Th1Pin));//Reads and calculates the temperature 1 value, sample 2
  int Temp2Value_2 = (analogRead(Th2Pin));//Reads and calculates the temperature 2 value, sample 2
  delay (MeasureDelay);//Waits before next measurement run

  float VoltValue_3 = (analogRead(VoltPin));  //Reads the voltage divider value, sample 3
  int LumValue_3 = (analogRead(LdrPin)); //Reads the LDR value, sample 3
  int Temp1Value_3 = (analogRead(Th1Pin));//Reads and calculates the temperature 1 value, sample 3
  int Temp2Value_3 = (analogRead(Th2Pin));//Reads and calculates the temperature 2 value, sample 3
  delay (MeasureDelay);//Waits before next measurement run

  float VoltValue_4 = (analogRead(VoltPin));  //Reads the voltage divider value, sample 4
  int LumValue_4 = (analogRead(LdrPin)); //Reads the LDR value, sample 4
  int Temp1Value_4 = (analogRead(Th1Pin));//Reads and calculates the temperature 1 value, sample 4
  int Temp2Value_4 = (analogRead(Th2Pin));//Reads and calculates the temperature 2 value, sample 4
  delay (MeasureDelay);//Waits before next measurement run

  float VoltValue_5 = (analogRead(VoltPin));  //Reads the voltage divider value, sample 5
  int LumValue_5 = (analogRead(LdrPin)); //Reads the LDR value, sample 5
  int Temp1Value_5 = (analogRead(Th1Pin));//Reads and calculates the temperature 1 value, sample 5
  int Temp2Value_5 = (analogRead(Th2Pin));//Reads and calculates the temperature 2 value, sample 5

  //SENSOR DATA DATA AVERAGE CALCULATION
  float VoltValueRaw = (VoltValue_1 + VoltValue_2 + VoltValue_3 + VoltValue_4 + VoltValue_5) / 5;
  int LumValueRaw = (LumValue_1 + LumValue_2 + LumValue_3 + LumValue_4 + LumValue_5) / 5;
  int Temp1ValueRaw = (Temp1Value_1 + Temp1Value_2 + Temp1Value_3 + Temp1Value_4 + Temp1Value_5) / 5;
  int Temp2ValueRaw = (Temp2Value_1 + Temp2Value_2 + Temp2Value_3 + Temp2Value_4 + Temp2Value_5) / 5;

  //SENSOR DATA CORRECTIONS
  float VoltValue = (VoltValueRaw * (5.0 / 1023.0) * 3.03) + 3;
  int BrightnessValue = (LumValueRaw * (5.0 / 1023.0)) * 200; // Calculates the LCD LED brightness value as a function of luminosity


  int Temp1Value = Temp1ValueRaw * 0.48828125 - 273.15 - 3;
  int Temp2Value = Temp2ValueRaw * 0.48828125 - 273.15 - 3;


  //SERIAL MONITOR PRINTOUTS
  Serial.print ("Volts:");
  Serial.print(VoltValue);
  Serial.print ("V");
  Serial.print (" ;  ");

  Serial.print ("Bright:");
  Serial.print(BrightnessValue);
  Serial.print (" ;  ");
  Serial.print ("Temp1:");
  Serial.print(Temp1Value);
  Serial.print ("C");
  Serial.print (" ;  ");
  Serial.print ("Temp2:");
  Serial.print(Temp2Value);
  Serial.println ("C");


  //LCD BRIGHTNESS PWM CONTROL
  if (BrightnessValue >= MinBrightness && BrightnessValue <= MaxBrightness) {
    analogWrite (BrightnessPin, BrightnessValue); //Sets the brightness of the LCD LED
  }
  else if (BrightnessValue < MinBrightness) {
    analogWrite (BrightnessPin, MinBrightness); //Sets the brightness of the LCD LED to min. value to avoid offscale low
  }
  else if (BrightnessValue > MaxBrightness) {
    analogWrite (BrightnessPin, MaxBrightness);
  }

  //Prints the readings if all is good

  if (Temp1Value <= Overtemp1 && Temp2Value <= Overtemp2) {

    //TURN OFF WARN LED
    digitalWrite(WarnLedPin, LOW);

    //LCD PRINTOUTS
    lcd.setCursor(0, 1);  // set the cursor to column 0, line 1
    lcd.print(VoltValue);  // print the voltage reading to the LCD
    lcd.print("V  "); // print "V" symbol
    lcd.setCursor(7, 1);  // set the cursor to column 0, line 1
    lcd.print(Temp1Value);  // print the temperature reading of sensor nr.1 to the LCD
    lcd.print((char)223);   // print "ºC" symbol
    lcd.print("C  ");
    lcd.setCursor(12, 1);  // set the cursor to column 0, line 1
    lcd.print(Temp2Value);  // print the temperature reading of sensor nr.2 to the LCD
    lcd.print((char)223); // print "ºC" symbol
    lcd.print("C  ");
    delay(StandardDelay);
  }

  //Prints the warnings sign "!" if T1 above max

  else if (Temp1Value > Overtemp1 && Temp2Value <= Overtemp2) {

    //TURN ON WARN LED
    digitalWrite(WarnLedPin, HIGH);

    //LCD PRINTOUTS
    lcd.setCursor(0, 1);  // set the cursor to column 0, line 1
    lcd.print(VoltValue);  // print the voltage reading to the LCD
    lcd.print("V  "); // print "V" symbol

    lcd.setCursor(7, 1);  // set the cursor to column 0, line 1
    lcd.print(Temp1Value);  // print the temperature reading of sensor nr.1 to the LCD
    lcd.print((char)223); // print "ºC" symbol
    lcd.print("!");// print "!" symbol

    lcd.setCursor(12, 1);  // set the cursor to column 0, line 1
    lcd.print(Temp2Value);  // print the temperature reading of sensor nr.2 to the LCD
    lcd.print((char)223); // print "ºC" symbol
    lcd.print("C  ");
    delay(StandardDelay);
  }

  //Prints the warnings sign "!" if T1 and T2 above max

  else if (Temp1Value > Overtemp1 && Temp2Value > Overtemp2) {

    //TURN ON WARN LED
    digitalWrite(WarnLedPin, HIGH);

    //LCD PRINTOUTS
    lcd.setCursor(0, 1);  // set the cursor to column 0, line 1
    lcd.print(VoltValue);  // print the voltage reading to the LCD
    lcd.print("V  "); // print "V" symbol

    lcd.setCursor(7, 1);  // set the cursor to column 0, line 1
    lcd.print(Temp1Value);  // print the temperature reading of sensor nr.1 to the LCD
    lcd.print((char)223); // print "ºC" symbol
    lcd.print("!");// print "!" symbol

    lcd.setCursor(12, 1);  // set the cursor to column 0, line 1
    lcd.print(Temp2Value);  // print the temperature reading of sensor nr.2 to the LCD
    lcd.print((char)223); // print "ºC" symbol
    lcd.print("!");// print "!" symbol

    delay(StandardDelay);
  }

  //Prints the warnings sign "!" if T2 above max

  else if (Temp1Value <= Overtemp1 && Temp2Value > Overtemp2) {

    //TURN ON WARN LED
    digitalWrite(WarnLedPin, HIGH);

    //LCD PRINTOUTS
    lcd.setCursor(0, 1);  // set the cursor to column 0, line 1
    lcd.print(VoltValue);  // print the voltage reading to the LCD
    lcd.print("V  "); // print "V" symbol
    lcd.setCursor(7, 1);  // set the cursor to column 0, line 1
    lcd.print(Temp1Value);  // print the temperature reading of sensor nr.1 to the LCD
    lcd.print((char)223);   // print "ºC" symbol
    lcd.print("C  ");

    lcd.setCursor(12, 1);  // set the cursor to column 0, line 1
    lcd.print(Temp2Value);  // print the temperature reading of sensor nr.2 to the LCD
    lcd.print((char)223); // print "ºC" symbol
    lcd.print("!");// print "!" symbol

    delay(StandardDelay);
  }

}
