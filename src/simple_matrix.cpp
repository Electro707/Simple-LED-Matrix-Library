#include "Arduino.h"
#include "simple_matrix.h"
#include "font.h"

/********************************************************************************
	simpleMatrix Library
	This library is designed to handle all the low and high level functions for
	the MAX7219 LED Matrixes.
  Originally designed for the University of New Haven's 
  Makerspace's Make-A-Sign Event.

	Author: Jamal Bouajjaj
********************************************************************************/

/********************************************************************************
  Low Level Function
********************************************************************************/
//Sends a command to a single MS7219
void simpleMatrix::sendCommandtoOne(uint8_t command, uint8_t data, uint8_t display){
  uint8_t d[number_of_module*2]; //Array that containts data to be sent
  d[2*display] = command;
  d[(2*display)+1] = data;
  digitalWrite(_DL_PIN,LOW);
  for(int k=0;k<number_of_module*2;k++){SPI.transfer(d[k]);} // Send data
  digitalWrite(_DL_PIN,HIGH);
}

//Sends a command to all MAX7219s
void simpleMatrix::sendCommandtoAll(uint8_t command, uint8_t data){
  digitalWrite(_DL_PIN,LOW);
  for(int i=0;i<number_of_module;i++){
    SPI.transfer(command);
    SPI.transfer(data);
  }
  digitalWrite(_DL_PIN,HIGH);
}

//Updates all displays. The buffer that is displayed in the
//	internal _matrix array
void simpleMatrix::senddisplay(){
  for(int r=0;r<8;r++){ //8 rows
    digitalWrite(_DL_PIN,LOW);
    for(int i=0;i<number_of_module;i++){
      SPI.transfer(r+1);
      SPI.transfer(_matrix[i][r]);
    }
    digitalWrite(_DL_PIN,HIGH);
  }
}

//The library's constructor. Sets the internal _DL_PIN value to pin and
//sends the pin as an OUTPUT and turns it HIGH
simpleMatrix::simpleMatrix(int pin){
  _DL_PIN = pin;
  pinMode(_DL_PIN,OUTPUT);
  digitalWrite(_DL_PIN,HIGH);
}

//Initalizes the library. Starts the SPI periferal and sends nessesary commands
void simpleMatrix::begin(){
	SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  sendCommandtoAll(0x0C,0x00); //Enter shutdown mode
  sendCommandtoAll(0x0B,0x07); //MAX7219 scan all 8 digits(in this case 8 rows)
  sendCommandtoAll(0x0A,0x0F); //Set to max intensity
  sendCommandtoAll(0x0F,0x00); //Exit test-display Mode
  sendCommandtoAll(0x09,0x00); //No decoding
  clearDisplay(); //Clear display
  sendCommandtoAll(0x0C,0x01); //Exit shutdown mode
}
/********************************************************************************
  High-Level functions
********************************************************************************/

//Converts a column-addressed buffer to a display-row-addressed buffer and sends it
void simpleMatrix::sendMatrixBuffer(uint8_t *mat){
  for(int i=0;i<number_of_module;i++){
    for(int k=0;k<8;k++){
        _matrix[i][k] = mat[k+(i*8)]; //Copy *mat to *_matrix with it split up
    }
  }
	//Function to transpose display (Thanks Spencer !)
  for(int d=0;d<number_of_module;d++){
    uint8_t temp[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    for(int i=0;i<8;i++){
      uint8_t bitmask_v = 1<<(i);
      for(int k=0;k<8;k++){
        temp[i] |= (_matrix[d][k] & bitmask_v) >> (i) << k;
      }
    }
    for(int i=0;i<8;i++){_matrix[d][i] = temp[i];}
  }
  senddisplay();
}

//Sends a custom 8x8 bitmap that is column-addressed. Can scroll it from left to right
void simpleMatrix::sendCustomSymbol(int d, uint8_t *mat, bool scroll = false, int del = 0){
  uint8_t display[(number_of_module+1)*8];
  for(int i=0;i<(number_of_module+1)*8;i++){display[i] = 0;} // Set *display to 0
  for(int k=0;k<8;k++){
      display[k+(d*8)] = mat[k]; //Copy 8x8 bitmap to internal *display array
  }
  sendMatrixBuffer(display);
  if(scroll==true){
		delay(del);
    int location = (d*8)+8; //Sets the right-most location of the bitmap
    while(1){
      for(int i=0;i<number_of_module*8;i++){display[i] = display[i+1];}  //Scrolls display
      sendMatrixBuffer(display);
      delay(del);
      if(location==0){ //If the right-most location = 0, it means that the displays
				break;				 //Has fully scrolled thru the bitmap, thus exit the while loop
			}
      location--;
    }
  }
}

//Fills the display. Can fill all or some displays
void simpleMatrix::fillDisplay(int from=0, int to=(number_of_module-1)){
  for(int i=from;i<(to+1);i++){
    for(int k=0;k<8;k++){
      _matrix[i][k] = 0xFF;
    }
  }
  senddisplay();
}

//Clears the displays. Can clear all or some displays
void simpleMatrix::clearDisplay(int from=0, int to=(number_of_module-1)){
  for(int d=from;d<(to+1);d++){
    for(int i=0;i<8;i++){
      _matrix[d][i] = 0x00;
    }
  }
  senddisplay();
}

//Sends and scrolls a text from left to right which is stored in Flash memory
void simpleMatrix::scrollTextPROGMEM(const char *text, int del){
	//Buffer will be 2 "immaginary" matrices right of the real one.
	//This is to have the smooth scolling effect
  const uint8_t charlenght = 6; //The lenght of each char including the lenght of the font.
  uint8_t display[8*(number_of_module+2)]; //Internal display buffer
  int buffer_location = 8*(number_of_module); //Location of the buffer in columns(right-most location)
  int lettercount = 0; //How many letters we sent
  int text_lenght = strlen(text);
  for(int i=0;i<8*(number_of_module+2);i++){display[i] = 0x00;} //Sets *display to 0
	//Sends out letters in the "immaginary" matrices, which is 2 of them.
	//The i-max is divided the multipled by char becuase when dividing, the MCU will round
	//down, thus only sending out the number of characters that will fit the 2 "immaginary" matrices
  for(int i=0;i<((2*8)/charlenght)*charlenght;i++){ //Send letter over the invisible displays
    if((i%charlenght)>=5){ //If the remainder of i%charlenght is more then 5(font size), send 0's
      display[i+(number_of_module*8)] = 0x00;
    }
    else{ //Send out the font
      display[i+(number_of_module*8)] = (font[ pgm_read_byte_near(text + (i/charlenght))-0x20 ][ i%charlenght ]);
    }
    buffer_location++;
  }
  lettercount = (2*8)/charlenght; //Record number of letters sent
  sendMatrixBuffer(display); //Send out the display
  while(1){
    delay(del);
    for(int i=0;i<(number_of_module+2)*8;i++){display[i] = display[i+1];} //Scrolls the display
    sendMatrixBuffer(display); //Send out the display
    buffer_location--;
    if(buffer_location <= (8*number_of_module)){ //When the buffer location reaches the real matrices
      if(lettercount < text_lenght){ //If the number of letters sent is less than the text lenght
        for(int i=0;i<charlenght;i++){ //Send out one character in the "immaginary" matrices
          if((i%charlenght)>=5){  //If the remainder of i%charlenght is more then 5(font size), send 0's
            display[i+(8*number_of_module)] = 0x00;
          }
          else{ //Send out the font
            display[i+(8*number_of_module)] = (font[ pgm_read_byte_near(text + lettercount)-0x20 ][ i ]);
          }
        }
        buffer_location += charlenght; //Increment buffer_location after character is sent
        lettercount++; //Increment the nubmer of letters sent
      }
      else{
        display[buffer_location] = 0x00; //Sends 0's if we already send out all of letters
      }
    }
    if(buffer_location == 0){ //When all the letters are scrolled and out of the matrix
      break;									//Break out of the while loop
    }
  }
}

//Sends and scrolls a text from left to right
void simpleMatrix::scrollText(char *text, int del){
	//Buffer will be 2 "immaginary" matrices right of the real one.
	//This is to have the smooth scolling effect
  const uint8_t charlenght = 6; //The lenght of each char including the lenght of the font.
  uint8_t display[8*(number_of_module+2)]; //Internal display buffer
  int buffer_location = 8*(number_of_module); //Location of the buffer in columns(right-most location)
  int lettercount = 0; //How many letters we sent
  int text_lenght = strlen(text);
  for(int i=0;i<8*(number_of_module+2);i++){display[i] = 0x00;} //Sets *display to 0
	//Sends out letters in the "immaginary" matrices, which is 2 of them.
	//The i-max is divided the multipled by char becuase when dividing, the MCU will round
	//down, thus only sending out the number of characters that will fit the 2 "immaginary" matrices
  for(int i=0;i<((2*8)/charlenght)*charlenght;i++){ //Send letter over the invisible displays
    if((i%charlenght)>=5){ //If the remainder of i%charlenght is more then 5(font size), send 0's
      display[i+(number_of_module*8)] = 0x00;
    }
    else{ //Send out the font
      display[i+(number_of_module*8)] = (font[ text[i/charlenght]-0x20][ i%charlenght ]);
    }
    buffer_location++;
  }
  lettercount = (2*8)/charlenght; //Record number of letters sent
  sendMatrixBuffer(display); //Send out the display
  while(1){
    delay(del);
    for(int i=0;i<(number_of_module+2)*8;i++){display[i] = display[i+1];} //Scrolls the display
    sendMatrixBuffer(display); //Send out the display
    buffer_location--;
    if(buffer_location <= (8*number_of_module)){ //When the buffer location reaches the real matrices
      if(lettercount < text_lenght){ //If the number of letters sent is less than the text lenght
        for(int i=0;i<charlenght;i++){ //Send out one character in the "immaginary" matrices
          if((i%charlenght)>=5){  //If the remainder of i%charlenght is more then 5(font size), send 0's
            display[i+(8*number_of_module)] = 0x00;
          }
          else{ //Send out the font
            display[i+(8*number_of_module)] = (font[ text[lettercount]-0x20][ i ]); 
          }
        }
        buffer_location += charlenght; //Increment buffer_location after character is sent
        lettercount++; //Increment the nubmer of letters sent
      }
      else{
        display[buffer_location] = 0x00; //Sends 0's if we already send out all of letters
      }
    }
    if(buffer_location == 0){ //When all the letters are scrolled and out of the matrix
      break;									//Break out of the while loop
    }
  }
}

//Scolls a column-adressed buffer from right to left. This buffer is to be the size of the 
//      whole matrix.
void simpleMatrix::scrollBuffer(uint8_t *mat, int del){
  int buffer_location = (number_of_module+1)*8;  //Location of buffer (right-most)
  int eight_col_sent = 1; //The number of 8 columns sent
  uint8_t display[(number_of_module+2)*8]; //Create buffer w/ 1 "immaginary" matrix
  for(int i=0;i<8*(number_of_module+2);i++){display[i] = 0x00;} //Sets *display to 0
  for(int i=0;i<8;i++){display[i+(number_of_module*8)] = mat[i];} //Sends out 8 columns from *mat
  sendMatrixBuffer(display);  //Send out *display buffer
  while(1){
    delay(del);
    for(int i=0;i<(number_of_module+2)*8;i++){display[i] = display[i+1];} //Scroll display
    sendMatrixBuffer(display); //Send out *display buffer
    buffer_location--;
    if(buffer_location <= (number_of_module*8)){ //If the buffer location hit the real matrices
			 //If not all of *mat is sent out(The *mat is the size of the matrix, so comparing
		 			//eight_col_sent to number_of_module works)
      if(eight_col_sent != number_of_module){
				//Send out 8 columns to the 1 "immaginary" buffer
        for(int i=0;i<8;i++){display[i+(number_of_module*8)] = mat[i+(eight_col_sent*8)];}
        eight_col_sent++;
        buffer_location += 8; //Increment buffer location by 8
      }
			else{ //Otherwise send a 0
				display[buffer_location] = 0x00;
			}
    }
    if(buffer_location == 0){ //When the whole *mat buffer are scrolled and out of the matrix
      break;									//Break out of the while loop
    }
  }
}
// Scolls a column-adressed buffer from right to left of x column (Better if it's a multiple of 8)
void simpleMatrix::scrollBuffer2(uint8_t *mat, int del, int column){
  int buffer_location = (number_of_module+1)*8;  //Location of buffer (right-most)
  int eight_col_sent = 1; //The number of 8 columns sent
  uint8_t display[(column)+2]; //Create buffer w/ 2 "immaginary" matrix
  for(int i=0;i<(column)+2;i++){display[i] = 0x00;} //Sets *display to 0
  for(int i=0;i<8;i++){display[i+(number_of_module*8)] = mat[i];} //Sends out 8 columns from *mat
  sendMatrixBuffer(display);  //Send out *display buffer
  while(1){
    delay(del);
    for(int i=0;i<(number_of_module+2)*8;i++){display[i] = display[i+1];} //Scroll display
    sendMatrixBuffer(display); //Send out *display buffer
    buffer_location--;
    if(buffer_location <= (number_of_module*8)){ //If the buffer location hit the real matrices
			 //If not all of *mat is sent out(The *mat is the size of the matrix, so comparing
		 			//eight_col_sent to number_of_module works)
      if(eight_col_sent < (column/8)){
				//Send out 8 columns to the 1 "immaginary" buffer
        for(int i=0;i<8;i++){display[i+(number_of_module*8)] = mat[i+(eight_col_sent*8)];}
        eight_col_sent++;
        buffer_location += 8; //Increment buffer location by 8
      }
		else{ //Otherwise send a 0
			display[buffer_location] = 0x00;
		}
    }
    if(buffer_location == 0){ //When the whole *mat buffer are scrolled and out of the matrix
      break;									//Break out of the while loop
    }
  }
}

//  Sets the intensity of all displays
void simpleMatrix::setIntensity(int intensity){
  sendCommandtoAll(0x0A,intensity); //Set to max intensity
}
