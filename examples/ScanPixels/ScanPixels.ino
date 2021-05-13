#include "simple_matrix.h"  //Import the library

#define DELAY 10 //Sets the delay between each frame in mS
#define NUMBER_OF_DISPLAYS 4 //Sets the number of display (4 for this example)

/*
Initialize the library. The 4 means that the CS pin is connected to pin D4.
You can change this to whatever you want, just make sure to connect the CS
pin to the pin number.

The disp means that any future function calls to the library uses "disp" as the
library's object name. For example, the library has a function called
"setIntensity", you need to write "disp.setIntensity" to call that function.
*/
simpleMatrix disp(4, NUMBER_OF_DISPLAYS);

//This code will run only once when the Arduino is turned on.
void setup(){
  //Starts the library
  disp.begin();
  /*
   * If you notice that the display is upside-down per display, uncomment the following line
   */
  //disp.invertIndividualDisplays();
  
  //Set the LED's intensity. This value can be anywhere between 0 and 15.
  disp.setIntensity(0x02);
}

//After void setup(), this code will run and loop forever.
void loop(){
  //This sets an LED ON sequentially per row.
  for(int y=0;y<8;y++){ //Repreat per row
    for(int x=0;x<(NUMBER_OF_DISPLAYS*8);x++){ //Repeat per column
      disp.setPixel(x, y);
      delay(DELAY); //Delay by DELAY mS
    }
  }
  //This sets an LED OFF sequentially per row.
  for(int y=0;y<8;y++){ //Repreat per row
    for(int x=0;x<(NUMBER_OF_DISPLAYS*8);x++){ //Repeat per column
      disp.clearPixel(x, y);
      delay(DELAY);  //Delay by DELAY mS
    }
  }
  //This sets an LED ON sequentially per column.
  for(int x=0;x<(NUMBER_OF_DISPLAYS*8);x++){ //Repreat per column
    for(int y=0;y<8;y++){ //Repreat per row
      disp.setPixel(x, y);
      delay(DELAY); //Delay by DELAY mS
    }
  }
  //This sets an LED OFF sequentially per column.
  for(int x=0;x<(NUMBER_OF_DISPLAYS*8);x++){ //Repreat per column
    for(int y=0;y<8;y++){ //Repreat per row
      disp.clearPixel(x, y);
      delay(DELAY); //Delay by DELAY mS
    }
  }
}
