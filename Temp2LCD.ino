#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SimpleTimer.h>

#define DHTPIN 2
#define DHTTYPE DHT11

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
DHT dht(DHTPIN, DHTTYPE);
SimpleTimer timer;

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dht.begin();
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_BUILTIN, HIGH);
  lcd.begin(20,4);
  lcd.setCursor(0,0);
  lcd.print("Office: ");
  lcd.setCursor(0,1);
  lcd.print("Outside: ");
  inputString.reserve(200);

  timer.setInterval(120000, getTemperature);
  timer.setInterval(300000, getOutsideTemperature);
  getTemperature();
  getOutsideTemperature();
}

void loop() {
  timer.run();
  if (stringComplete) {   
    
    lcd.setCursor(9,1);
    lcd.print("           ");
    clearLine(3);
    
    int colon = inputString.indexOf(':');
    String outsideTemp = inputString.substring(0,colon);
    String conditions = inputString.substring(colon + 1, inputString.length());

    lcd.setCursor(9,1);
    lcd.print(outsideTemp);
    lcd.setCursor(0,3);
    lcd.print(conditions);
    // clear the string:
    inputString = "";
    stringComplete = false;
    
  }

}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
      inputString.trim();
    }
  }
}

void getTemperature() {
  digitalWrite(LED_BUILTIN,HIGH);
  float f = dht.readTemperature(true);
  String output = "";
  output.concat(f);
  output.concat('\n');
  digitalWrite(LED_BUILTIN, LOW);
  lcd.setCursor(8,0);
  lcd.print(f);
}

void getOutsideTemperature() {
  Serial.write("update\n");
}

void clearLine(int row) {
  lcd.setCursor(0, row);
  lcd.print("                   ");
  lcd.setCursor(0, row);
}

