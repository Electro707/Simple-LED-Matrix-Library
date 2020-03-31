#ifndef SIMPLEMATRIX_H
#define SIMPLEMATRIX_H

#include "Arduino.h"
#include <SPI.h>
#include <avr/pgmspace.h>

#ifndef NUMBER_OF_MODULES
#define NUMBER_OF_MODULES 4
#endif

class simpleMatrix{
	public:
		simpleMatrix(int pin, bool rotateIndividualDislay = false);
		void begin();
		void clearDisplay(int from=0, int to=(NUMBER_OF_MODULES-1));
		void fillDisplay(int from=0, int to=(NUMBER_OF_MODULES-1));
		void scrollText(char *text, int del, int start_from=(NUMBER_OF_MODULES*8)+1);
		void scrollTextPROGMEM(const char *text, int del, int start_from=(NUMBER_OF_MODULES*8)+1);
		void print(char *text, int start_from=0, bool is_text_progmem=false, bool scroll_text=false, int del=0);
		void setIntensity(int intensity);
		void setPixel(int x, int y, int value);
		void scrollBuffer(uint8_t *mat, int del, int column, int start_from=(NUMBER_OF_MODULES*8)+1);
		void sendColumnBuffer(uint8_t *mat, int column, int start_from=0, bool scroll=false, int del=0);
		void sendMatrixBuffer(uint8_t *mat);
	private:
		uint8_t _matrix[NUMBER_OF_MODULES+1][8];
		uint8_t _matrix_col[(NUMBER_OF_MODULES)*8];   // A copy of a column_addressed matrix
		bool _ROTATE_INDIV_DISPLAY;
		int _DL_PIN;
		void senddisplay(); //Sends the _matrix buffer
		void sendCommandtoAll(uint8_t command, uint8_t data);
		void sendCommandtoOne(uint8_t command, uint8_t data, uint8_t display);
};

#endif
