#include "simple_matrix.h"  //Import the library

#define DELAY 10 //Sets the delay between each frame in mS
#define NUMBER_OF_DISPLAYS 4 //Sets the number of display (4 for this example)
uint8_t matrix_b[NUMBER_OF_DISPLAYS*8]; //Column-adressed display

/*
Initialize the library. The 4 means that the CS pin is connected to pin D4.
You can change this to whatever you want, just make sure to connect the CS
pin to the pin number.
The disp means that any future function calls to the library uses "disp" as the
library's object name. For example, the library has a function called
"setIntensity", you need to write "disp.setIntensity" to call that function.
*/
simpleMatrix disp(4);

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
      matrix_b[k] |= (1<<i); //Sets a 1 to the i-th bit(row number) in the k(column number) element
      disp.sendMatrixBuffer(matrix_b); //Send the matrix buffer
      delay(DELAY); //Delay by DELAY mS
    }
  }
  //This sets an LED OFF sequentially per row.
  for(int i=0;i<8;i++){ //Repreat per row
    for(int k=0;k<(NUMBER_OF_DISPLAYS*8);k++){ //Repeat per column
      matrix_b[k] &= ~(1<<i);  //Sets a 0 to the i-th bit(row number) in the k(column number) element
      disp.sendMatrixBuffer(matrix_b); //Send the matrix buffer
      delay(DELAY);  //Delay by DELAY mS
    }
  }
  //This sets an LED ON sequentially per column.
  for(int i=0;i<(NUMBER_OF_DISPLAYS*8);i++){ //Repreat per column
    for(int k=0;k<8;k++){ //Repreat per row
      matrix_b[i] |= (1<<k);  //Sets a 1 to the k-th bit(row number) in the i(column number) element
      disp.sendMatrixBuffer(matrix_b); //Send the matrix buffer
      delay(DELAY); //Delay by DELAY mS
    }
  }
  //This sets an LED OFF sequentially per column.
  for(int i=0;i<(NUMBER_OF_DISPLAYS*8);i++){ //Repreat per column
    for(int k=0;k<8;k++){ //Repreat per row
      matrix_b[i] &= ~(1<<k); //Sets a 0 to the k-th bit(row number) in the i(column number) element
      disp.sendMatrixBuffer(matrix_b); //Send the matrix buffer
      delay(DELAY); //Delay by DELAY mS
    }
  }
}
