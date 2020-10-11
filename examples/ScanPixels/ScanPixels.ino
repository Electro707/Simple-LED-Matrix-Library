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

If you notice that the display is upside-down per display, change 
simpleMatrix disp(4, false, NUMBER_OF_DISPLAYS); to simpleMatrix disp(4, true, NUMBER_OF_DISPLAYS);
*/
simpleMatrix disp(4, false, NUMBER_OF_DISPLAYS);

//This code will run only once when the Arduino is turned on.
void setup(){
  //Starts the library
  disp.begin();
  //Set the LED's intensity. This value can be anywhere between 0 and 15.
  disp.setIntensity(0x02);
}

//After void setup(), this code will run and loop forever.
void loop(){
  //This sets an LED ON sequentially per row.
  for(int i=0;i<8;i++){ //Repreat per row
    for(int k=0;k<(NUMBER_OF_DISPLAYS*8);k++){ //Repeat per column
      disp.setPixel(i, k);
      delay(DELAY); //Delay by DELAY mS
    }
  }
  //This sets an LED OFF sequentially per row.
  for(int i=0;i<8;i++){ //Repreat per row
    for(int k=0;k<(NUMBER_OF_DISPLAYS*8);k++){ //Repeat per column
      disp.clearPixel(i, k);
      delay(DELAY);  //Delay by DELAY mS
    }
  }
  //This sets an LED ON sequentially per column.
  for(int i=0;i<(NUMBER_OF_DISPLAYS*8);i++){ //Repreat per column
    for(int k=0;k<8;k++){ //Repreat per row
      disp.setPixel(k, i);
      delay(DELAY); //Delay by DELAY mS
    }
  }
  //This sets an LED OFF sequentially per column.
  for(int i=0;i<(NUMBER_OF_DISPLAYS*8);i++){ //Repreat per column
    for(int k=0;k<8;k++){ //Repreat per row
      disp.clearPixel(k, i);
      delay(DELAY); //Delay by DELAY mS
    }
  }
}
