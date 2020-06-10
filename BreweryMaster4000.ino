#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

const int buttonUpPin = 3;
const int buttonDownPin = 4;
const int buttonMenuPin = 5;

bool buttonUp = false;
bool buttonDown = false;
bool buttonMenu = false;

const int relayOutput = 6;
const int readTemp = 7;

double setTemp = 17;
double currentTemp = 0;
double thermometerCalibration = 0;
double hysteresis = 0.5;
double trueTemp;
double trueSetTemp;

bool checkTemp = false;

unsigned long timerThen = 0;
unsigned long timerNow = 0;
unsigned long checkTime = 5.0;
const long millisToMin = 60000;

const String setTempText = "Temp. zadana:";
const String thermometerCalibrationText = "Kalibracja:";
const String hysteresisText = "Histereza:";
const String checkTimeText = "Czas zwłoki:";

const int setTempPage = 0;
const int thermometerCalibrationPage = 1;
const int hysteresisPage = 2;
const int checkTimePage = 3;
const int lastPage = checkTimePage;


int menuPage = 0;


void setup() {

  lcd.begin(16, 2);
  lcd.clear();

  Serial.begin(115200);
}

void loop() {
  Serial.println();

  

  timerNow = millis();
  trueTemp = currentTemp + thermometerCalibration;
  trueSetTemp = setTemp + hysteresis;

  if ( trueSetTemp > trueTemp && checkTemp == false ) {
    timerThen = millis();
    checkTemp = true;
  };


  if (checkTemp == true && timerNow - timerThen > checkTime * millisToMin) {

    if (trueSetTemp > trueTemp) {
      while (trueSetTemp > trueTemp) {
        digitalWrite(relayOutput, LOW);
      };
      digitalWrite(relayOutput, HIGH);
      checkTemp = true;
    } else {
      checkTemp = true;
    };
  };



  if (digitalRead(buttonMenu) == LOW){
    menuPage++;
    }
  if (menuPage > lastPage) {
    menuPage = 0;
    }

buttonUp = debounce(buttonUpPin);
buttonDown = debounce(buttonDownPin);
buttonMenu = debounce(buttonMenuPin);

displayMenu(lcd, setTempText, setTemp, menuPage, setTempPage);
displayMenu(lcd, thermometerCalibrationText, thermometerCalibration, menuPage, thermometerCalibrationPage);
displayMenu(lcd, hysteresisText, hysteresis, menuPage, hysteresisPage);
displayMenu(lcd, checkTimeText, checkTime, menuPage, checkTimePage);

setValue (buttonUp, buttonDown, setTemp, menuPage, setTempPage);
setValue (buttonUp, buttonDown, thermometerCalibration, menuPage, thermometerCalibrationPage);
setValue (buttonUp, buttonDown, hysteresis, menuPage, hysteresisPage);
setValue (buttonUp, buttonDown, checkTime, menuPage, checkTimePage);

  
}


bool debounce (int buttonPin){

unsigned long timerNow = millis();
unsigned long timerThen;
bool debounce = false;

  if (digitalRead(buttonPin) == LOW && debounce == false) {
    debounce = true;
    timerThen = millis();
    };

  if (digitalRead(buttonPin) == HIGH && debounce == true && timerNow - timerThen > 500) {
    debounce = false;
    return true; 
     };
  return false;
  }


void displayMenu ( LiquidCrystal_I2C lcd, String text, int value, int currentPage, int setPage ){
  if  (currentPage == setPage) {

      lcd.setCursor(0,0);
      lcd.print(text);
      lcd.setCursor(0,1);
      lcd.print(value);
        } 
  };


void setValue (int buttonUp, int buttonDown, int value, int currentPage, int setPage) {
  
  if (digitalRead(buttonUp) == LOW) {
    value=value + 0.5;
    };

  if (digitalRead(buttonDown) == LOW){
    value = value - 0.5;
    };
    
  };
