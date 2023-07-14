/********************************************************************************
 * Template for generating C "libraries" that are used to generate GIF of the
 * matrix library's display output
********************************************************************************/
#include "Arduino.h"
#include "simple_matrix.h"
#include "stdio.h"

/********************************************************************************
 * Mock function definitions for Arduino.h
********************************************************************************/
void pinMode(int, int){
}
void digitalWrite(int, int){
}
void SPI_CLASS::transfer(uint8_t a){
//     printf("%02x", a);
}
void SPI_CLASS::begin(void){
}
void SPI_CLASS::setBitOrder(int){
}
void SPI_CLASS::beginTransaction(SPISettings settings){
}
char pgm_read_byte_near(const char*){
  return 0xAA;
}
void delay(int){
}

/********************************************************************************
 * END Mock function definitions for Arduino.h
********************************************************************************/

#define NUMB_OF_LED_MATRICES 4
int return_delay = 0;

SPI_CLASS SPI;
simpleMatrix disp(NUMB_OF_LED_MATRICES);
void (*callback_f)(uint8_t *, int);

/**
 * This function returns the _matrix pointer in simple_matrix.c, which gets sent right
 * up to Python
*/
uint8_t *return_m(){
  return disp.return_external_matrix();
}

/**
 * This function gets called by simple_matrix.c when the TEST_EXTERNAL flag is raised
 * every time senddisplay() is called, allowing us to get the LED matrix display.
*/
void send_display_buffer(){
  callback_f(return_m(), return_delay);
}

/**
 * Not sure if `extern "C"` is needed. Probably not. IIRC, it just made Python happy so I am keeping it
*/
extern "C" {
  
  /**
   * This is where you will make your own function that interfaces with the virtual display
  */
  int TEST_FUNCTION(void (*f)(uint8_t *, int)){
    callback_f = f;   // Callback function to give the array to the Python application for processing
    return_delay = 1000;
    
    // insert your own function here :)
  }

}
