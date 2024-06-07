# Home Automation and Temperature-Controlled Fan

## Overview

This project uses an Arduino to control a fan and a relay based on temperature readings from a DS18B20 sensor. The system also includes an LCD for displaying temperature and fan speed, and Bluetooth functionality for remotely controlling the relay.

## Features

- **Temperature Display**: Displays the current temperature on an LCD.
- **Fan Speed Control**: Adjusts fan speed based on the temperature.
- **Bluetooth Control**: Allows remote control of the relay via Bluetooth.
- **Relay Control**: Controls a relay to turn on/off an external device.

## Components

- Arduino
- DS18B20 Temperature Sensor
- Liquid Crystal Display (LCD)
- Relay Module
- Fan (PWM controlled)
- Bluetooth Module
- Various resistors and capacitors for circuit stabilization

## Pin Configuration

### LCD Display

- `rs` (Pin 7): Register select pin for the LCD.
- `en` (Pin 6): Enable pin for the LCD.
- `d4` (Pin 8): Data pin 4 for the LCD.
- `d5` (Pin 9): Data pin 5 for the LCD.
- `d6` (Pin 10): Data pin 6 for the LCD.
- `d7` (Pin 11): Data pin 7 for the LCD.

### Sensors and Actuators

- `ONE_WIRE_BUS` (Pin 12): Pin connected to the OneWire bus for the DS18B20 temperature sensor. Ensure that you have connected the pull up resistor with the output pin of the temperature sensor. You can take explore more info from this link.
  https://lastminuteengineers.com/ds18b20-arduino-tutorial/
- `RelayPin` (Pin 4): Pin connected to the relay module.
- `FanPin` (Pin 3): Pin connected to the fan for PWM control.

### Constants

- `RELAY_ON`: Value to turn the relay on (1).
- `RELAY_OFF`: Value to turn the relay off (0).

## Libraries Used

- `LiquidCrystal.h`: Library to control the LCD display.
- `OneWire.h`: Library for the OneWire protocol.
- `DallasTemperature.h`: Library to get temperature readings from DS18B20 sensors.

## Code Explanation

### Pin Definitions and Library Inclusions

```cpp
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
```

### Bluetooth Function

```cpp
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
```

The `bluetooth()` function reads data from the serial port (sent via Bluetooth). If '1' is received, the relay is turned on; if '0' is received, the relay is turned off.

### Display Temperature Function

```cpp
void display_temp() {
  sensors.requestTemperatures();  // Request temperature in Celsius readings from DS18B20
  tempC = sensors.getTempCByIndex(0);

  if (tempC != DEVICE_DISCONNECTED_C) {
    Serial.println("Temperature: ");
    Serial.print(tempC);
    Serial.print(" °C ");
  } else {
    Serial.println("Error: Could not read temperature data");
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Temp: "));
  lcd.print(tempC);
  lcd.print(F(" C  "));
  lcd.print(flag = !flag);
}
```

The `display_temp()` function requests the temperature from the DS18B20 sensor and displays it on the LCD. It also prints the temperature to the serial monitor.

### Fan Speed Control Function

```cpp
void Fan_Speed_Control() {
  if (tempC < 32) {
    fanSpeed = 0;
  } else if (tempC >= 32 && tempC < 34) {
    fanSpeed = 60;
  } else if (tempC >= 34 && tempC < 37) {
    fanSpeed = 140;
  } else if (tempC >= 37) {
    fanSpeed = 255;
  }

  analogWrite(FanPin, fanSpeed);
  Serial.print("Fan Speed: ");
  Serial.println(fanSpeed);

  lcd.setCursor(0, 1);
  lcd.print(F("Fan Speed: "));
  lcd.print(fanSpeed);
}
```

The `Fan_Speed_Control()` function adjusts the fan speed based on the current temperature. The speed is set using PWM to control the fan connected to `FanPin`.

### Setup Function

```cpp
void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(RelayPin, OUTPUT);
  pinMode(FanPin, OUTPUT);
  digitalWrite(RelayPin, RELAY_OFF);
  digitalWrite(FanPin, LOW);
}
```

The `setup()` function initializes the serial communication, LCD, and sets the pin modes for the relay and fan.

### Main Loop

```cpp
void loop() {
  display_temp();
  bluetooth();
  Fan_Speed_Control();
  delay(1000);
}
```

The `loop()` function continuously updates the temperature display, checks for Bluetooth commands, adjusts the fan speed, and repeats every second.

## Installation

1. **Hardware Setup**:
    - Connect the components to the Arduino as per the pin configuration.
    - Ensure the power supply to the fan and relay is correctly set up.
    
2. **Software Setup**:
    - Install the necessary libraries in the Arduino IDE.
    - Open the `.ino` file in the Arduino IDE.
    - Upload the code to the Arduino board.

## Usage

1. Power on the system.
2. The LCD will display the current temperature and fan speed.
3. Use a Bluetooth terminal app to send '1' to turn on the relay and '0' to turn it off.
4. The fan speed will adjust automatically based on the temperature.

## Customization

- **Adjusting Fan Speed Ranges**: Modify the `Fan_Speed_Control()` function to set different temperature thresholds and corresponding fan speeds.
- **Changing Temperature Display Format**: Adjust the `display_temp()` function to display temperature in Fahrenheit or add more details.

## Troubleshooting

- **LCD Not Displaying Properly**: Check the connections to the LCD and ensure the correct pins are defined in the code.
- **Temperature Readings Incorrect**: Verify the sensor connections and ensure the DS18B20 sensor is functioning.
- **Bluetooth Not Responding**: Ensure the Bluetooth module is properly connected and paired with the terminal device.

## License

This project is licensed under the MIT License. Feel free to modify and distribute as per the license terms.

## Acknowledgments

Special thanks to the contributors of the libraries used in this project.

---

This README provides a comprehensive guide for users to understand, set up, and troubleshoot the home automation and temperature-controlled fan project with Bluetooth and LCD display, with detailed explanations of every pin and code section.
