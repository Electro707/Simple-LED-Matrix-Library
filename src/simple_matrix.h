#ifndef SIMPLEMATRIX_H
#define SIMPLEMATRIX_H

#include "Arduino.h"
#include <SPI.h>
#include <avr/pgmspace.h>

#ifndef number_of_module
#define number_of_module 4
#endif

class simpleMatrix{
  public:
    simpleMatrix(int pin);
    void begin();
    void clearDisplay(int from=0, int to=(number_of_module-1));
    void fillDisplay(int from=0, int to=(number_of_module-1));
    void scrollText(char *text, int del);
	  void scrollTextPROGMEM(const char *text, int del);
    void sendMatrixBuffer(uint8_t *mat);
    void setIntensity(int intensity);
    void setPixel(int x, int y, int value);
    void sendCustomSymbol(int d, uint8_t *mat, bool scroll = false, int del = 0);
    void scrollBuffer(uint8_t *mat, int del, int column);
  private:
    uint8_t _matrix[(number_of_module)+2][8];
    int _DL_PIN;
    void senddisplay(); //Sends the _matrix buffer
    void sendCommandtoAll(uint8_t command, uint8_t data);
    void sendCommandtoOne(uint8_t command, uint8_t data, uint8_t display);
};

#endif
