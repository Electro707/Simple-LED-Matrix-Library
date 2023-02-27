#include "Arduino.h"
#include "simple_matrix.h"
#include "stdio.h"

/********************************************************************************
 * Mock functions for Arduino.h
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

#define NUMB_OF_LED_MATRICES 4
int return_delay = 1000;

SPI_CLASS SPI;
simpleMatrix disp(NUMB_OF_LED_MATRICES);
void (*callback_f)(uint8_t *, int);

uint8_t *return_m(){
  return disp.return_external_matrix();
}

void send_display_buffer(){
  callback_f(return_m(), return_delay);
}

extern "C" {
    int test_f(void (*f)(uint8_t *, int)){
    callback_f = f;   // Callback function to give the array to the Python application for processing
    
    // Test application. Change this to what you want
    disp.begin();

    disp.fillDisplay();
    
    return 0;
  }
}
