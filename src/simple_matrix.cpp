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
Constructor
********************************************************************************/
//The library's constructor. Sets the internal _DL_PIN value to pin and
//sends the pin as an OUTPUT and turns it HIGH
simpleMatrix::simpleMatrix(int pin, bool rotateIndividualDislay = false){
	_DL_PIN = pin;
	_ROTATE_INDIV_DISPLAY = rotateIndividualDislay;
	pinMode(_DL_PIN,OUTPUT);
	digitalWrite(_DL_PIN,HIGH);
}
/********************************************************************************
  Low Level Function
********************************************************************************/
//Sends a command to a single MS7219
void simpleMatrix::sendCommandtoOne(uint8_t command, uint8_t data, uint8_t display){
	uint8_t d[NUMBER_OF_MODULES*2]; //Array that containts data to be sent
	d[2*display] = command;
	d[(2*display)+1] = data;
	digitalWrite(_DL_PIN,LOW);
	for(int k=0;k<NUMBER_OF_MODULES*2;k++){SPI.transfer(d[k]);} // Send data
	for(int k=0;k<NUMBER_OF_MODULES*8;k++){_matrix_col[k] = 0; _matrix[k/NUMBER_OF_MODULES][k%NUMBER_OF_MODULES] = 0;}
	digitalWrite(_DL_PIN,HIGH);
}

//Sends a command to all MAX7219s
void simpleMatrix::sendCommandtoAll(uint8_t command, uint8_t data){
	digitalWrite(_DL_PIN,LOW);
	for(int i=0;i<NUMBER_OF_MODULES;i++){
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
		for(int i=0;i<NUMBER_OF_MODULES;i++){
			SPI.transfer(r+1);
			SPI.transfer(_matrix[i][r]);
		}
	digitalWrite(_DL_PIN,HIGH);
	}
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
	for(int i=0;i<NUMBER_OF_MODULES;i++){
		for(int k=0;k<8;k++){
			_matrix_col[k+(i*8)] = mat[k+(i*8)];
			_matrix[i][k] = mat[k+(i*8)]; //Copy *mat to *_matrix with it split up
		}
	}
	//Function to transpose display (Thanks Spencer Hopwood!)
	for(int d=0;d<NUMBER_OF_MODULES;d++){
		uint8_t temp[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
		for(int i=0;i<8;i++){
			uint8_t bitmask_v = 1<<(i);
			for(int k=0;k<8;k++){
				// From expericence, there are displays which have each matrix flipped 180degrees
				// To combat this, there is a flag which will decide whether or not to rotate
				// each matrix's buffer by 180degrees
				if(_ROTATE_INDIV_DISPLAY){
					temp[7-i] |= ((_matrix[d][k] & bitmask_v) >> (i)) << (7-k); 
				}else{
					temp[i] |= ((_matrix[d][k] & bitmask_v) >> (i)) << (k);
				}
			}
		}
		for(int i=0;i<8;i++){_matrix[d][i] = temp[i];}
	}
	senddisplay();
}

//Fills the display. Can fill all or some displays
void simpleMatrix::fillDisplay(int from=0, int to=(NUMBER_OF_MODULES-1)){
	for(int i=from;i<(to+1);i++){
		for(int k=0;k<8;k++){
			_matrix[i][k] = 0xFF;
		}
	}
	senddisplay();
}

//Clears the displays. Can clear all or some displays
void simpleMatrix::clearDisplay(int from=0, int to=(NUMBER_OF_MODULES-1)){
	for(int d=from;d<(to+1);d++){
		for(int i=0;i<8;i++){
			_matrix[d][i] = 0x00;
		}
	}
	senddisplay();
}

//Sends and scrolls a text from left to right which is stored in Flash memory
void simpleMatrix::scrollTextPROGMEM(const char *text, int del, int start_from=(NUMBER_OF_MODULES*8)+1){
	print(text, start_from, true, true, del);
}

//Sends and scrolls a text from left to right
void simpleMatrix::scrollText(char *text, int del, int start_from=(NUMBER_OF_MODULES*8)+1){
	print(text, start_from, false, true, del);
}

//Sends text with an option to scoll it
void simpleMatrix::print(char *text, int start_from=0, bool is_text_progmem=false, bool scroll_text=false, int del=0){
	//Buffer will be 2 "immaginary" matrices right of the real one.
	//This is to have the smooth scolling effect
	uint8_t display[8*(NUMBER_OF_MODULES+2)]; //Internal display buffer
	const uint8_t charlenght = 6; //The lenght of each char including the lenght of the font.
	int text_lenght = strlen(text);
	for(int i=0;i<8*(NUMBER_OF_MODULES+2);i++){display[i] = 0x00;} //Sets *display to 0
	int lettercount = 0;
	//Sends out letters in the "immaginary" matrices, which is 2 of them.
	//The i-max is divided the multipled by char becuase when dividing, the MCU will round
	//down, thus only sending out the number of characters that will fit the 2 "immaginary" matrices
	int i=0;
	while(1){
		if((i%charlenght)>=5){ //If the remainder of i%charlenght is more then 5(font size), send 0's
			display[i+start_from] = 0x00;
		}
		else{ //Send out the font
			if(is_text_progmem){
				display[i+start_from] = (font[ pgm_read_byte_near(text + (i/charlenght))-0x20 ][ i%charlenght ]);
			}
			else{
				display[i+start_from] = (font[ text[i/charlenght]-0x20][ i%charlenght ]);
			}
		}
		
		i++;
		if((i%charlenght)==0){ //If the remainder of i%charlenght is more then 5(font size), send 0's
			lettercount++;
		}
		if(((lettercount+1)*charlenght)+start_from >= 8*(NUMBER_OF_MODULES+2)){
			break;
		}
		
	}
	//int lettercount = i/charlenght;
	int buffer_location = i + start_from;
	sendMatrixBuffer(display); //Send out the display

	if(scroll_text){
		while(1){
			delay(del);
			for(int i=0;i<((NUMBER_OF_MODULES+2)*8)-1;i++){display[i] = display[i+1];} //Scrolls the display
			sendMatrixBuffer(display); //Send out the display
			buffer_location--;
			if(buffer_location <= (8*NUMBER_OF_MODULES)){ //When the buffer location reaches the real matrices
				if(lettercount < text_lenght){ //If the number of letters sent is less than the text lenght
					for(int i=0;i<charlenght;i++){ //Send out one character in the "immaginary" matrices
						if((i%charlenght)>=5){  //If the remainder of i%charlenght is more then 5(font size), send 0's
							display[i+(8*NUMBER_OF_MODULES)] = 0x00;
						}
						else{ //Send out the font
							if(is_text_progmem){
								display[i+(8*NUMBER_OF_MODULES)] = (font[ pgm_read_byte_near(text + lettercount)-0x20 ][ i ]);
							}
							else{
								display[i+(8*NUMBER_OF_MODULES)] = (font[ text[lettercount]-0x20][ i ]); 
							}
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
				break;  //Break out of the while loop
			}
		}
	}
}

// Function to scroll a buffer of any size
void simpleMatrix::scrollBuffer(uint8_t *mat, int del, int column, int start_from=(NUMBER_OF_MODULES*8)+1){
	sendColumnBuffer(mat, column, start_from, true, del);
}

// General function to send over a column-arrayed buffer. Can be scrolled or not, depending in preference
void simpleMatrix::sendColumnBuffer(uint8_t *mat, int column, int start_from=0, bool scroll=false, int del=0){
	uint8_t display[(NUMBER_OF_MODULES+1)*8]; // Create buffer w/ 1 "immaginary" column + number of columns in matrix
	for(int i=0;i<(NUMBER_OF_MODULES+1)*8;i++){display[i] = _matrix_col[i];} // Sets *display to copy of current display
	int column_send = 0;
	for(int i=start_from;i<=(NUMBER_OF_MODULES*8)+1;i++){
		display[i] = mat[i-start_from]; 
		column_send++;
		if(column_send == column){break;}
	}
	sendMatrixBuffer(display);  // Send out *display buffer
	if(scroll){
		while(1){
			delay(del);   // Delay by the inputed delay amount
			for(int i=0;i<(NUMBER_OF_MODULES*8)+1;i++){display[i] = display[i+1];} // Scroll display
			sendMatrixBuffer(display); // Send out *display buffer
			// If the number of column sent is less than or equal to the number of columns in the array,
			// send the array data, else send 0
			display[1+(NUMBER_OF_MODULES*8)] = (column_send <= (column-1)) ? mat[column_send] : 0x00;
			// If the number of column sent is greater than the number of column in the array + the column size
			// of the matrix, exit the while loop as it has scrolled thru the array and scrolled it out of frame
			if(column_send > column+(NUMBER_OF_MODULES*8)){
				break;  // Break out of the while loop
			}
			column_send++;  // Increment the number of columns sent
		}
	}
}

//  Sets the intensity of all displays
void simpleMatrix::setIntensity(int intensity){
	sendCommandtoAll(0x0A,intensity); //Set to max intensity
}
