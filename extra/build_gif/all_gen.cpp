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

uint8_t *return_m(){
  return disp.return_external_matrix();
}

void send_display_buffer(){
  callback_f(return_m(), return_delay);
}

extern "C" {
  int fillDisplay1(void (*f)(uint8_t *, int)){
    callback_f = f;   // Callback function to give the array to the Python application for processing
    return_delay = 1000;
    
    // Test application. Change this to what you want
    disp.begin();

    disp.fillDisplay();
    
    return 0;
  }

  int fillDisplay2(void (*f)(uint8_t *, int)){
    callback_f = f;   // Callback function to give the array to the Python application for processing
    return_delay = 1000;
    
    // Test application. Change this to what you want
    disp.begin();

    disp.fillDisplay(0, 2);
    
    return 0;
  }

  int scrollText1(void (*f)(uint8_t *, int)){
    callback_f = f;   // Callback function to give the array to the Python application for processing
    return_delay = 50;
    
    // Test application. Change this to what you want
    disp.begin();

    disp.scrollText("Hello World!", 50);
    
    return 0;
  }

  int scrollText2(void (*f)(uint8_t *, int)){
    callback_f = f;   // Callback function to give the array to the Python application for processing
    return_delay = 50;
    
    // Test application. Change this to what you want
    disp.begin();

    disp.scrollText("Hello World!", 50, true);
    
    return 0;
  }

  int setColumnPixels1(void (*f)(uint8_t *, int)){
    callback_f = f;   // Callback function to give the array to the Python application for processing
    return_delay = 1000;
    
    // Test application. Change this to what you want
    disp.begin();

    disp.setColumnPixels(0, 0, 2);
    disp.setColumnPixels(3, 5, 7);
    disp.setColumnPixels(8, 0, 7);
    disp.setColumnPixels(12, 3, 5);
    
    return 0;
  }

  int setRowPixels1(void (*f)(uint8_t *, int)){
    callback_f = f;   // Callback function to give the array to the Python application for processing
    return_delay = 1000;
    
    // Test application. Change this to what you want
    disp.begin();

    disp.setRowPixels(0, 2, 0);
    disp.setRowPixels(10, 12, 0);
    disp.setRowPixels(0, 15, 5);
    disp.setRowPixels(20, 31, 7);
    
    return 0;
  }

  int trainn(void (*f)(uint8_t *, int)){
    callback_f = f;   // Callback function to give the array to the Python application for processing

    uint8_t TRAIIIINNNNNNN[] = { 
      0b11100,
      0b101100,
      0b1001110,
      0b10001110,
      0b10001111,
      0b10001111,
      0b11111111,
      0b11111110,
      0b11111110,
      0b11111110,
      0b11111110,
      0b11111110,
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111110,
      0b11111110,
      0b1111100,
      0b11111110,
      0b10001111,
      0b10001111,
      0b11111111,
      0b10001110,
      0b10001110,
      0b11111110,
      0b10001110,
      0b10001110,
      0b11111111,
      0b10001111,
      0b10001111,
      0b11111110,
      0b1111100,
      0b11111110,
      0b10001111,
      0b10001111,
      0b11111111,
      0b10001110,
      0b10001110,
      0b11111110,
      0b10001110,
      0b10001110,
      0b11111111,
      0b10001111,
      0b10001111,
      0b11111110,
      0b1111100,
      0b11111110,
      0b10001111,
      0b10001111,
      0b11111111,
      0b10001110,
      0b10001110,
      0b11111110,
      0b10001110,
      0b10001110,
      0b11111111,
      0b10001111,
      0b10001111,
      0b11111110,
      0b1111100,
      0b11111110,
      0b10001111,
      0b10001111,
      0b11111111,
      0b10001110,
      0b10001110,
      0b11111110,
      0b10001110,
      0b10001110,
      0b11111111,
      0b10001111,
      0b10001111,
      0b11111110,
      0b1111100,
      0b11111110,
      0b10001111,
      0b10001111,
      0b11111111,
      0b10001110,
      0b10001110,
      0b11111110,
      0b10001110,
      0b10001110,
      0b11111111,
      0b10001111,
      0b10001111,
      0b11111110,
      0b1111100,
      0b11111110,
      0b10001111,
      0b10001111,
      0b11111111,
      0b10001110,
      0b10001110,
      0b11111110,
      0b10001110,
      0b10001110,
      0b11111111,
      0b10001111,
      0b10001111,
      0b11111110,
      0b1111100,
      0b11111110,
      0b10001111,
      0b10001111,
      0b11111111,
      0b10001110,
      0b10001110,
      0b11111110,
      0b10001110,
      0b10001110,
      0b11111111,
      0b10001111,
      0b10001111,
      0b11111110,
      0b1111100,
      0b11111110,
      0b10001111,
      0b10001111,
      0b11111111,
      0b10001110,
      0b10001110,
      0b11111110,
      0b10001110,
      0b10001110,
      0b11111111,
      0b10001111,
      0b10001111,
      0b11111110,
      0b1111100,
      0b11111110,
      0b10001111,
      0b10001111,
      0b11111111,
      0b10001110,
      0b10001110,
      0b11111110,
      0b10001110,
      0b10001110,
      0b11111111,
      0b10001111,
      0b10001111,
      0b11111110,
      0b1111100,
      0b11111110,
      0b10001111,
      0b10001111,
      0b11111111,
      0b10001110,
      0b10001110,
      0b11111110,
      0b10001110,
      0b10001110,
      0b11111111,
      0b10001111,
      0b10001111,
      0b11111110,
      0b1111100,
      0b11111110,
      0b10001111,
      0b10001111,
      0b11111111,
      0b10001110,
      0b10001110,
      0b11111110,
      0b10001110,
      0b10001110,
      0b11111111,
      0b10001111,
      0b10001111,
      0b11111110,
      0b1111100,
      0b11111110,
      0b10001111,
      0b10001111,
      0b11111111,
      0b10001110,
      0b10001110,
      0b11111110,
      0b10001110,
      0b10001110,
      0b11111111,
      0b10001111,
      0b10001111,
      0b11111110,
      0b1111100,
      0b11111110,
      0b10001111,
      0b10001111,
      0b11111111,
      0b10001110,
      0b10001110,
      0b11111110,
      0b10001110,
      0b10001110,
      0b11111111,
      0b10001111,
      0b10001111,
      0b11111110,
      0b1111100,
      0b11111110,
      0b10001111,
      0b10001111,
      0b11111111,
      0b10001110,
      0b10001110,
      0b11111110,
      0b10001110,
      0b10001110,
      0b11111111,
      0b10001111,
      0b10001111,
      0b11111110,
      0b1111100,
      0b11111110,
      0b11111110,
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111110,
      0b11111110,
      0b11111110,
      0b11111110,
      0b11111110,
      0b11111111,
      0b10001111,
      0b10001111,
      0b10001110,
      0b1001110,
      0b101100,
      0b11100,
      0b0
      };
      uint8_t column_lenght = 232;

      // Test application. Change this to what you want
    disp.begin();
    return_delay = 45;
    disp.scrollText("CHOOO CHOOO !!!", 10);
    return_delay = 20;
    disp.scrollBuffer(TRAIIIINNNNNNN, 10, column_lenght);  
    
    return 0;
  }


}
