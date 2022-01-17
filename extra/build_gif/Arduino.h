/*
  Mock Arduino.h to be used with test.cpp and generate_img.py with build.sh
*/
#ifndef ARDUINO_H
#define ARDUINO_H

#include "stdint.h"

#define NULL 0

#define OUTPUT 1
#define INPUT 0
#define HIGH 1
#define LOW 0

void pinMode(int, int);
void digitalWrite(int, int);
void delay(int);

char pgm_read_byte_near(const char*);

void send_display_buffer();

#define MSBFIRST 0

class SPI_CLASS{
    public:
      void transfer(uint8_t a);
      void begin(void);
      void setBitOrder(int);
};

extern SPI_CLASS SPI;

#endif
