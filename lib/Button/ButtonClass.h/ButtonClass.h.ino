#ifndef Button_h
#define Button_h

#include "Arduino.h"


class Button
{
  
  public:

    Button (int pin){
      this -> pin = pin;
      };
          
    bool state = false;

    bool isReleased() {
      
    if (digitalRead(pin) == LOW && flag == false){
    flag = true;
    };
    
    if (digitalRead(pin) == HIGH && flag == true){
    flag = false;
    return true;
    } else {
      return  false;
      };     
      }; 

  private:

  int pin;
  bool flag; 
   };


#endif
