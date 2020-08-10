/**
 * \mainpage
 * \brief A library designed to allow for an easy and quick experience programing an LED matrix with an Arduino, even with little prior knowledge of coding or electronics.
 *
 * \note This library currently only supports a 4 MAX7219 LED matrices display.
 *
 * \todo Create more examples.
 * \todo Publish pictures.
 * \todo Have scrolling be able to be done from left to right.
 * \todo Have the option to have more than 4 MAX7219 LED matrices.
 * \todo Add full support for the MAX7221 chipset.
 *
 * <a href="https://github.com/Electro707/Simple-LED-Matrix-Library">Link to the library's Github repository</a>
 *
 * To check out the old documentation page, click <a href="https://electro707.com/documentation/Libraries/simple_led_library_old_docs/index.php">Here!</a>
 *
 * \section Installation
 * To install the library, the prefered method is by using the Arduino's Library Manager. Simply looking for <em>simple led matrix</em> should bring up this library. THe other way to install this library is to install it as a ZIP library downloaded from the Github repository.
 */

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
        /**
         * \brief The class constructor of this library.
         * \param pin The Arduino pin in which the CS line from the matrix is connected to.
         * \param rotateIndividualDislay Whether to rotate each individual display while sending data. 
                                         Useful as some display are 'inverted' in their LED matrix orientation.
         */
        simpleMatrix(int pin, bool rotateIndividualDislay = false);

        /**
         * \brief Begins the library and initializes the display.
         */
        void begin();

        /**
         * \brief Clears the display.
         * By default, this function will clear the entire display (all matrices).
         * \param from Which LED matrix to start clearing from.
         * \param to Which LED matrix to clear up to.
         */
        void clearDisplay(int from=0, int to=(NUMBER_OF_MODULES-1));

        /**
         * \brief Fills the display.
         * By default, this function will fill the entire display (all matrices).
         * \param from Which LED matrix to start filling from.
         * \param to Which LED matrix to fill up to.
         */
        void fillDisplay(int from=0, int to=(NUMBER_OF_MODULES-1));

        /**
         * \brief Scroll a word/sentence from right to left.
         * \param *text Any word/sentence you want.
         * \param del The delay between each frame while scrolling.
         * \param start_from Which column the text will start scrolling from.
         */
        void scrollText(char *text, int del, int start_from=(NUMBER_OF_MODULES*8)+1);

        /**
         * \brief Same as function scrollText(), but the text is stored in Flash memory instead of RAM.
         *        This allows for arbitrarily long messages to be displayed without any RAM issues/overflow.
         * \param *text A constant char array consisting of the word/sentence to be sent out. \n
         *              \b WARNING: For AVR architectures, the *text array must also have the pretense PROGMEM so the string is stored in the flash memory instead of RAM.
         * \param del The delay between each frame while scrolling.
         * \param start_from Which column the text will start scrolling from.
         */
        void scrollTextPROGMEM(const char *text, int del, int start_from=(NUMBER_OF_MODULES*8)+1);
        
        /**
         * \brief Sends some text to the display, with the option to have it scroll from right to left.
         * \param *text The string that you want to send (must be a char array).
         * \param start_from The position where the text will start in the display.
         * \param is_text_progmem Whether the text is stored in Flash or RAM.
         * \param scroll_text Whether the outputed display will be scrolled from right to left.
         * \param del The delay between each frame while scrolling.
         */
        void print(char *text, int start_from=0, bool is_text_progmem=false, bool scroll_text=false, int del=0);
        
        /**
         * \brief Set the intensity of the LED matrix.
         * \param intensity The intensity of the display from 0-15.
         */
        void setIntensity(int intensity);
        
        /**
         * \brief Sets a single pixel in the 4 8x8 LED matrices to either on or off.
         * \param x,y The coordinates of the pixel to be sent/cleared.
         * \param value true or false depending on whether you want the pixel to be turned off or on.
         */
        void setPixel(int x, int y, int value);
        
        /**
         * \brief A wrapper function for the sendColumnBuffer() function.
         *        Scrolls a custom bitmap to the matrices, with a height of 8 pixels and any number of column
         * \param *mat An uint8_t column-addressed array containing the data to be sent.
         * \param del The delay between each frame while scrolling.
         * \param column The number of columns to be sent out (in most cases this would be the same as the size of the *mat array).
         * \param start_from Where the buffer will start at in the display.
         */
        void scrollBuffer(uint8_t *mat, int del, int column, int start_from=(NUMBER_OF_MODULES*8)+1);

        /**
         * \brief General function to send over a column-arrayed buffer. Can be scrolled or not, depending in preference.
         * \warning This function should not be directly used if you want to scroll a buffer. Instead, the scrollBuffer() function should be used for that.
         * \param *mat An uint8_t column-addressed array containing the data to be sent.
         * \param column The number of columns to be sent out (in most cases this would be the same as the size of the *mat array).
         * \param start_from Where the buffer will start at in the display.
         * \param scroll Whether the outputed display will be scrolled from right to left.
         * \param del The delay between each frame if scroll is set to true.
         */
        void sendColumnBuffer(uint8_t *mat, int column, int start_from=0, bool scroll=false, int del=0);
        
        /**
         * \brief Sends a custom 32x8 bitmap to the matrices. 
         * \warning This function should not be directly used.
         * \param *mat A column-addressed array that contains the bytes (*mat must be an array of uint8_t type) of size that is equal to the number of columns in your buffer (For a 4 matrix display, there will be 32 columns)
         */
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
