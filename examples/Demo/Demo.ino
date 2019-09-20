#include "makerspace_sign.h" //Import the library

/*
Initialize the library. The 4 means that the CS pin is connected to pin D4.
You can change this to whatever you want, just make sure to connect the CS
pin to the pin number.
The mk means that any future function calls to the library uses "mk" as the
library's object name. For example, the library has a function called
"setIntensity", you need to write "mk.setIntensity" to call that function.
*/
MS_MAX7219 mk(4);

/*
Store the text to be displayed in memory
The PROGMEM means that the text will be stored in the Arduino's
flash memory instead of RAM
*/
const char text[] PROGMEM = "Hello World!";
//Defines "textDelay" as 100.
#define textDelay 100

//This code will run only once when the Arduino is turned on.
void setup(){
  //Starts the library
  mk.begin();
  //Set the LED's intensity. This value can be anywhere between 0 and 15.
  mk.setIntensity(0x05);
}

//After void setup(), this code will run and loop forever.
void loop(){
  //Scroll the text "text". The "textDelay" determines how long each
  //frame is in mS.
  mk.scrollText(text,textDelay);
  //After scolling, delay by 1 second.
  delay(1000);
}
