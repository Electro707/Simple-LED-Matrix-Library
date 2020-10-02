#define SIMPLEMATRIX_NUMBER_OF_MODULES 4
#include "simple_matrix.h" //Import the library

/*
Initialize the library. The 4 means that the CS pin is connected to pin D4.
You can change this to whatever you want, just make sure to connect the CS
pin to the pin number.
The disp means that any future function calls to the library uses "disp" as the
library's object name. For example, the library has a function called
"setIntensity", you need to write "disp.setIntensity" to call that function.
*/
simpleMatrix disp(4);

/*
Store the text to be displayed in memory
The PROGMEM means that the text will be stored in the Arduino's
flash memory instead of RAM. This means that you can a lot more stuff (~27000 characters available)
*/
const char text[] PROGMEM = "Hello World5";
//Defines "textDelay" as 25ms (Delay between when the display scroll by 1 pixel to the left).
#define textDelay 25

//This code will run only once when the Arduino is turned on.
void setup(){
  Serial.begin(19200);
  Serial.println("START");
  //Starts the library
  disp.begin();
  //Set the LED's intensity. This value can be anywhere between 0 and 15.
  disp.setIntensity(0x03);
  disp.scrollTextPROGMEM(text, 500, 16, false);
  Serial.println("DONE!");
}

//After void setup(), this code will run and loop forever.
void loop(){
}
