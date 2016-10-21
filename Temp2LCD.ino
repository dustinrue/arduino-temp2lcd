#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN 2
#define DHTTYPE DHT11

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
DHT dht(DHTPIN, DHTTYPE);

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
  lcd.print("Temperature: ");
  inputString.reserve(200);
}

void loop() {
  if (stringComplete) {
    // Arduino doesn't do strings in switch case statements
    if (inputString == "temperature") {
      getTemperature();
    }
    else if (inputString == "humidity") {
      getHumidity();
    }
    else {
      // don't do anything
    }
    
    clearLine(1);
    lcd.print(inputString);
    // clear the string:
    inputString = "";
    stringComplete = false;

    // the DHT11 needs a couple of seconds to settle
    // after being polled
    delay(2000);
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
  Serial.write(output.c_str());
  digitalWrite(LED_BUILTIN, LOW);
  lcd.setCursor(13,0);
  lcd.print(f);
}

void getHumidity() {
  digitalWrite(LED_BUILTIN,HIGH);
  //float f = dht.readTemperature(true);
  float f = dht.readHumidity(true);
  String output = "";
  output.concat(f);
  output.concat('\n');
  Serial.write(output.c_str());
  digitalWrite(LED_BUILTIN, LOW);
  lcd.setCursor(13,0);
  lcd.print(f);
}

void clearLine(int row) {
  lcd.setCursor(0, row);
  lcd.print("                   ");
  lcd.setCursor(0, row);
}

