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
simpleMatrix::simpleMatrix(int pin, bool rotateIndividualDislay){
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
    uint8_t d[SIMPLEMATRIX_NUMBER_OF_MODULES*2]; //Array that containts data to be sent
    d[2*display] = command;
    d[(2*display)+1] = data;
    digitalWrite(_DL_PIN,LOW);
    for(int k=0;k<SIMPLEMATRIX_NUMBER_OF_MODULES*2;k++){SPI.transfer(d[k]);} // Send data
    for(int k=0;k<SIMPLEMATRIX_NUMBER_OF_MODULES*8;k++){_matrix_col[k] = 0; _matrix[k/SIMPLEMATRIX_NUMBER_OF_MODULES][k%SIMPLEMATRIX_NUMBER_OF_MODULES] = 0;}
    digitalWrite(_DL_PIN,HIGH);
}

//Sends a command to all MAX7219s
void simpleMatrix::sendCommandtoAll(uint8_t command, uint8_t data){
    digitalWrite(_DL_PIN,LOW);
    for(int i=0;i<SIMPLEMATRIX_NUMBER_OF_MODULES;i++){
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
        for(int i=0;i<SIMPLEMATRIX_NUMBER_OF_MODULES;i++){
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
void simpleMatrix::sendMatrixBuffer(uint8_t *mat, int start_from){
    for(int i=0;i<SIMPLEMATRIX_NUMBER_OF_MODULES;i++){
        for(int k=0;k<8;k++){
            _matrix_col[k+(i*8)] = mat[k+(i*8)+start_from];
            _matrix[i][k] = mat[k+(i*8)+start_from]; //Copy *mat to *_matrix with it split up
        }
    }
    //Function to transpose display (Thanks Spencer Hopwood!)
    for(int d=0;d<SIMPLEMATRIX_NUMBER_OF_MODULES;d++){
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
void simpleMatrix::fillDisplay(int from, int to){
    for(int i=from;i<(to+1);i++){
        for(int k=0;k<8;k++){
            _matrix[i][k] = 0xFF;
        }
    }
    senddisplay();
}

//Clears the displays. Can clear all or some displays
void simpleMatrix::clearDisplay(int from, int to){
    for(int d=from;d<(to+1);d++){
        for(int i=0;i<8;i++){
            _matrix[d][i] = 0x00;
        }
    }
    senddisplay();
}

//Sends and scrolls a text from right to left which is stored in Flash memory
void simpleMatrix::scrollTextPROGMEM(const char *text, int del, bool left_to_right, int start_from){
    // If the default value is unchanged, set the start from the end of the matrix
    if(start_from == (int)0x8000){
        start_from = left_to_right ? -(strlen(text)*FONT_CHAR_LENGHT) : (8*SIMPLEMATRIX_NUMBER_OF_MODULES)+1;
    }
    print(text, start_from, true, true, del, left_to_right);
}
// Wrapper for scrollTextPROGMEM for simple usage
void simpleMatrix::scrollTextPROGMEMRightToLeft(const char *text, int del, int start_from){
    scrollTextPROGMEM(text, del, false, start_from);
}

// Wrapper for scrollTextPROGMEM for simple usage
void simpleMatrix::scrollTextPROGMEMLeftToRight(const char *text, int del, int start_from){
    scrollTextPROGMEM(text, del, true, start_from);
}

// Function that scrolls a text left or right
void simpleMatrix::scrollText(const char *text, int del, bool left_to_right, int start_from){
    // If the default value is unchanged, set the start from the end of the matrix
    if(start_from == (int)0x8000){
        start_from = left_to_right ? -(strlen(text)*FONT_CHAR_LENGHT) : (8*SIMPLEMATRIX_NUMBER_OF_MODULES)+1;
    }
    print(text, start_from, false, true, del, left_to_right);
}

// Wrapper for scrollText for simple usage
void simpleMatrix::scrollTextRightToLeft(const char *text, int del, int start_from){
    scrollText(text, del, false, start_from);
}

// Wrapper for scrollText for simple usage
void simpleMatrix::scrollTextLeftToRight(const char *text, int del, int start_from){
    scrollText(text, del, true, start_from);
}

void simpleMatrix::print(const char *text, int start_from, bool is_text_progmem, bool scroll_text, int del, bool left_to_right){
    uint8_t display[8*SIMPLEMATRIX_NUMBER_OF_MODULES];     // Internal display buffer
    int text_lenght = strlen(text);                        // The lenght of the string
    int text_arr_lenght = text_lenght*FONT_CHAR_LENGHT;          // The column lenght of the text 
    for(int i=0;i<8*SIMPLEMATRIX_NUMBER_OF_MODULES;i++){display[i] = 0x00;} //Sets *display to 0
    
    int start_letter_on_matrix;         // Where does the text start on the display
    int end_letter_on_matrix;           // Where does the text end on the display
    int missed_text_cols = 0;               // The number of columns from the text that was missed (to fill the display when scrolling)
    int text_cols_to_send;              // The number of columns from the text that still needs to be sent
    
    int i=-1;
    
    while(1){
        i++;
        if(start_from+i < 0){
            continue;
        }
        else if(start_from+i==0){
            //TODO: Check other start positions for start_letter_on_matrix
            start_letter_on_matrix = i+start_from;
            missed_text_cols = i;
        }
        
        if(start_from+i >= 8*SIMPLEMATRIX_NUMBER_OF_MODULES || i >= text_arr_lenght){
            end_letter_on_matrix = i+start_from-1;
            text_cols_to_send = i-1;
            break;
        }
        
        if((i%FONT_CHAR_LENGHT)>=5){ //If the remainder of i%FONT_CHAR_LENGHT is more then 5(font size), send 0's
            display[i+start_from] = 0x00;
        }
        else{ //Send out the font
            if(is_text_progmem){
                display[i+start_from] = (font[ pgm_read_byte_near(text + (i/FONT_CHAR_LENGHT))-0x20 ][ i%FONT_CHAR_LENGHT ]);
            }
            else{
                display[i+start_from] = (font[ text[i/FONT_CHAR_LENGHT]-0x20][ i%FONT_CHAR_LENGHT ]);
            }
        }
        
    }
    sendMatrixBuffer(display); //Send out the display
    // If the option for scrolling is there, do so depending if it's left to right or right to left
    if(scroll_text){
        if(left_to_right){
            scroll_text_left_to_right(text, del, is_text_progmem, missed_text_cols, start_letter_on_matrix, display);
        }
        else{
            scroll_text_right_to_left(text, del, is_text_progmem, text_cols_to_send, end_letter_on_matrix, display, text_arr_lenght);
        }
    }
}



// Function to scroll a buffer of any size
void simpleMatrix::scrollBuffer(uint8_t *mat, int del, int column, int start_from){
    sendColumnBuffer(mat, column, start_from, true, del);
}

// General function to send over a column-arrayed buffer. Can be scrolled or not, depending in preference
void simpleMatrix::sendColumnBuffer(uint8_t *mat, int column, int start_from, bool scroll, int del){
    uint8_t display[(SIMPLEMATRIX_NUMBER_OF_MODULES+1)*8]; // Create buffer w/ 1 "immaginary" column + number of columns in matrix
    for(int i=0;i<(SIMPLEMATRIX_NUMBER_OF_MODULES+1)*8;i++){display[i] = 0x00;} // Sets *display to all 0's to start with
        for(int i=0;i<SIMPLEMATRIX_NUMBER_OF_MODULES*8;i++){display[i] = _matrix_col[i];} // Sets *display to copy of current display

    int column_send = 0;
    for(int i=start_from;i<=(SIMPLEMATRIX_NUMBER_OF_MODULES*8)+1;i++){
        display[i] = mat[i-start_from]; 
        column_send++;
        if(column_send == column){break;}
    }
    sendMatrixBuffer(display);  // Send out *display buffer
    if(scroll){
        while(1){
            delay(del);   // Delay by the inputed delay amount
            for(int i=0;i<(SIMPLEMATRIX_NUMBER_OF_MODULES*8)+1;i++){display[i] = display[i+1];} // Scroll display
            sendMatrixBuffer(display); // Send out *display buffer
            // If the number of column sent is less than or equal to the number of columns in the array,
            // send the array data, else send 0
            display[1+(SIMPLEMATRIX_NUMBER_OF_MODULES*8)] = (column_send <= (column-1)) ? mat[column_send] : 0x00;
            // If the number of column sent is greater than the number of column in the array + the column size
            // of the matrix, exit the while loop as it has scrolled thru the array and scrolled it out of frame
            if(column_send > column+(SIMPLEMATRIX_NUMBER_OF_MODULES*8)){
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

void simpleMatrix::scroll_text_left_to_right(const char  *text, int del, bool is_text_progmem, int missed_text_cols, int start_letter_on_matrix, uint8_t *display){
    delay(del);
    while(missed_text_cols > (int)strlen(text)*FONT_CHAR_LENGHT){missed_text_cols--;}
    while(start_letter_on_matrix < 0){start_letter_on_matrix++;}
    while(1){
        for(int i=(SIMPLEMATRIX_NUMBER_OF_MODULES*8)-1;i>0;i--){display[i] = display[i-1];} //Scrolls the display
        if(missed_text_cols > 0){
            missed_text_cols--;
            if((missed_text_cols%FONT_CHAR_LENGHT)>=5){ //If the remainder of i%FONT_CHAR_LENGHT is more then 5(font size), send 0's
                display[start_letter_on_matrix] = 0x00;
            }
            else{ //Send out the font
                if(is_text_progmem){
                    display[start_letter_on_matrix] = (font[ pgm_read_byte_near(text + (missed_text_cols/FONT_CHAR_LENGHT))-0x20 ][ missed_text_cols%FONT_CHAR_LENGHT ]);
                }
                else{
                    display[start_letter_on_matrix] = (font[ text[missed_text_cols/FONT_CHAR_LENGHT]-0x20][ missed_text_cols%FONT_CHAR_LENGHT]);
                }
            }
        }
        else{
            display[start_letter_on_matrix] = 0x00; //Sends 0's if we already send out all of letters
            start_letter_on_matrix++;
        }
        
        if(start_letter_on_matrix > (SIMPLEMATRIX_NUMBER_OF_MODULES*8)){
            break;
        }
        
        sendMatrixBuffer(display); //Send out the display
        delay(del);
    }
}

void simpleMatrix::scroll_text_right_to_left(const char  *text, int del, bool is_text_progmem, int text_cols_to_send, int end_letter_on_matrix, uint8_t *display, int text_arr_lenght){
    delay(del);
    while(1){
        if(end_letter_on_matrix >= SIMPLEMATRIX_NUMBER_OF_MODULES*8){delay(del); end_letter_on_matrix--; continue;}
        for(int i=0;i<(SIMPLEMATRIX_NUMBER_OF_MODULES*8)-1;i++){display[i] = display[i+1];} //Scrolls the display
        if(text_cols_to_send < text_arr_lenght-1){
            text_cols_to_send++;
            if((text_cols_to_send%FONT_CHAR_LENGHT)>=5){ //If the remainder of i%FONT_CHAR_LENGHT is more then 5(font size), send 0's
                display[end_letter_on_matrix] = 0x00;
            }
            else{ //Send out the font
                if(is_text_progmem){
                    display[end_letter_on_matrix] = (font[ pgm_read_byte_near(text + (text_cols_to_send/FONT_CHAR_LENGHT))-0x20 ][ text_cols_to_send%FONT_CHAR_LENGHT ]);
                }
                else{
                    display[end_letter_on_matrix] = (font[ text[text_cols_to_send/FONT_CHAR_LENGHT]-0x20][ text_cols_to_send%FONT_CHAR_LENGHT]);
                }
            }
        }
        else{
            display[end_letter_on_matrix] = 0x00; //Sends 0's if we already send out all of letters
            end_letter_on_matrix--;
        }
        
        if(end_letter_on_matrix < 0){
            break;
        }
            
        sendMatrixBuffer(display); //Send out the display
        delay(del);
    }
}
