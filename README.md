# C_BreweryController

Program for Arduino UNO.

## How it works:

It check what is current temperature and matches it with set temperature. When difference occur, it turn on heater or cooler.

## Hardware:
* DS18B20 – temperature sensor,
* Arduino Uno,
* LCD1602 HD44780 2x16,
* Relay module (x2),

## Libraries used:
* NewLiquidCrystal_lib – for LCD display,
* DallasTemperature, OneWire – for sensor,

## Variables:
* setTemp – temperature that we aim for,
* hysteresis – diffrence from setTemp, after cooler or heater will turn on,
* thermometerCalibration – correction for sensor,
* checkTime – time after which temperature will be regulated,

## Classes:
* Button – for button definition, action on realise,
