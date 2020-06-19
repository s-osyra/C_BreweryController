
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Button.h>

#define ONE_WIRE_BUS 8
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

const int relay1Output = 6;
const int relay2Output = 7;
const int readTemp = 8;

float setTemp = 18.0;
float currentTemp = 0.0;
float thermometerCalibration = 0.0;
float hysteresis = 0.5;
float trueTemp;

bool checkTemp = false;
bool heater = false;
bool cooler = false;


unsigned long timerThen = 0;
unsigned long timerSensor = 0;
float checkTime = 5;
unsigned long tempCheckTime = 5000;
const long millisToMin = 60000;

const String setTempText = "T.zadana: ";
const String thermometerCalibrationText = "Kalib.:";
const String hysteresisText = "Hist.:";
const String checkTimeText = "C.zwloki:";
const String currentTempText = "Ob.T.: ";

const int setTempPage = 0;
const int thermometerCalibrationPage = 1;
const int hysteresisPage = 2;
const int checkTimePage = 3;
const int lastPage = checkTimePage;


int menuPage = 0;

Button menu (2);
Button down (3);
Button up (4);


void setup() {

  lcd.begin(16, 2);
  lcd.clear();

  pinMode (relay1Output, OUTPUT);
  pinMode (relay2Output, OUTPUT);
  digitalWrite(relay1Output, HIGH);
  digitalWrite(relay2Output, HIGH);

  //Serial.begin(115200);



  sensors.begin();


 
  timerThen  = 0;
  timerSensor = 0;
  
  sensors.requestTemperatures();
}

void loop() {

  trueTemp = (currentTemp + thermometerCalibration) ;

  if (abs(millis() - timerSensor) > tempCheckTime) {
    timerSensor = millis();
    sensors.requestTemperatures();
    
  };

//  Serial.print("////////////////////////");
//  Serial.print("\n");
//  Serial.print("Timer: ");
//  Serial.print(millis() - timerThen);
//  Serial.print("\n");
//  Serial.print("checkTemp: ");
//  Serial.print(checkTemp);
//  Serial.print("\n");
//  Serial.print("////////////////////////");
  
  currentTemp = sensors.getTempCByIndex(0);

  
  if (  (abs(millis() - timerThen) > checkTime * millisToMin) && checkTemp == false ) {
    checkTemp = true;

      if (trueTemp  < setTemp - hysteresis){
      heater = true;
      cooler = false;
     }

    else if ( (setTemp + hysteresis) < trueTemp ){
      heater = false;
      cooler = true;
     }

     else {
          checkTemp = false;
          timerThen = millis();
      };
     
  };
     

  if (heater == true && checkTemp == true) {
    
    digitalWrite(relay1Output, LOW);
        if (trueTemp > setTemp ) {
          checkTemp = false;
          timerThen = millis();
          digitalWrite(relay1Output, HIGH);
          };
        };

  if (cooler == true && checkTemp == true) {
    
    digitalWrite(relay2Output, LOW);
        if (trueTemp < setTemp ) {
          checkTemp = false;
          timerThen = millis();
          digitalWrite(relay2Output, HIGH);
          };
        };




  if (menu.isReleased() == true) {
    menuPage++;
    lcd.clear();
  };

  if (menuPage > lastPage) {
    menuPage = 0;
  };


  lcd.setCursor(0, 0);
  lcd.print(setTempText);
  lcd.print(setTemp, 1);
  lcd.print((char)223);
  lcd.print("C");


switch (menuPage) {
  
  case 0:
  setTemp = setValue (up.isReleased(), down.isReleased(), setTemp);
  displayLCD(lcd, currentTempText, trueTemp );
  displayLCD_C();
  break;

  case 1:
  displayLCD(lcd, thermometerCalibrationText, thermometerCalibration );
  displayLCD_C();
  thermometerCalibration = setValue (up.isReleased(), down.isReleased(), thermometerCalibration);
  break;

  case 2:
  displayLCD(lcd, hysteresisText, hysteresis );
  displayLCD_C();
  hysteresis = setValue (up.isReleased(), down.isReleased(), hysteresis);
  break;

  case 3: 
  displayLCD(lcd, checkTimeText, checkTime );
  displayLCD_M();
  checkTime = setValue (up.isReleased(), down.isReleased(), checkTime);

  if (checkTime <=0 ) {
    checkTime = 0.1;
    };
  
  break;
  
    
  };

}


void displayLCD(LiquidCrystal_I2C lcd, String text, float value){
    lcd.setCursor(0, 1);
    lcd.print(text);
    lcd.print(value, 1); 
  };
void displayLCD_C(){
    lcd.print((char)223);
    lcd.print("C");
    lcd.print("     ");
   };
void displayLCD_M(){
    lcd.print(" min");
    lcd.print("     ");
  };


float setValue (bool buttonUp, bool buttonDown, float value) {


    if (buttonUp == true) {
      return value = value + 0.1;
    };

    if (buttonDown == true) {
      return value = value - 0.1;
    };
  

  return value;

};
