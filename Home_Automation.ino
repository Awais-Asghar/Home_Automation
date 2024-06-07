#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 12
#define RelayPin 4
#define RELAY_ON 1
#define RELAY_OFF 0
#define FanPin 3

int fanSpeed = 0;
float tempC;
char data;
bool flag = 0;
const int rs = 7, en = 6, d4 = 8, d5 = 9, d6 = 10, d7 = 11;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void bluetooth() {
  
  if (Serial.available() > 0) {
    data = Serial.read();
    Serial.print(data);
    if (data == '1') {
      digitalWrite(RelayPin, RELAY_ON);
      Serial.println("Bulb is now turned ON.");
    } else if (data == '0') {
      digitalWrite(RelayPin, RELAY_OFF);
      Serial.println("Bulb is now turned OFF.");
    }
  }
}

void display_temp() {
  
  sensors.requestTemperatures();  // Request temperature in Celsius readings from DS18B20
  tempC = sensors.getTempCByIndex(0);

  if (tempC != DEVICE_DISCONNECTED_C) 
  {
    Serial.println("Temperature: ");
    Serial.print(tempC);
    Serial.print(" °C ");
  } else 
  {
    Serial.println("Error: Could not read temperature data");
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Temp: "));
  lcd.print(tempC);
  lcd.print(F(" C  "));
  lcd.print(flag = !flag);

}

void Fan_Speed_Control() {

  if (tempC < 32) 
  {
    fanSpeed = 0;
  } else if (tempC >= 32 && tempC < 34) 
  {
    fanSpeed = 60;
  } else if (tempC >= 34 && tempC < 37) 
  {
    fanSpeed = 140;
  } else if (tempC >=37 ) 
  {
    fanSpeed = 255;
  }

  analogWrite(FanPin, fanSpeed);
  Serial.print("Fan Speed: ");
  Serial.println(fanSpeed);

  lcd.setCursor(0, 1);
  lcd.print(F("Fan Speed: "));
  lcd.print(fanSpeed);

}

void setup() {

  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(RelayPin, OUTPUT);
  pinMode(FanPin, OUTPUT);
  digitalWrite(RelayPin, RELAY_OFF);
  digitalWrite(FanPin, LOW);
}

void loop() {

  display_temp();
  bluetooth();
  Fan_Speed_Control();
  delay(1000);

}
