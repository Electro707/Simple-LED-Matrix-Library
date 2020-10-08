/**
 * \mainpage
 * \brief A library designed to allow for an easy and quick experience programing an LED matrix with an Arduino, even with little prior knowledge of coding or electronics.
 *
 * \todo Create more examples.
 * \todo Publish pictures.
 * \todo Have scrolling be able to be done from left to right.
 * \todo Add full support for the MAX7221 chipset.
 *
 * <a href="https://github.com/Electro707/Simple-LED-Matrix-Library">Link to the library's Github repository</a>
 *
 * To check out the old documentation page, click <a href="https://electro707.com/documentation/Libraries/simple_led_library_old_docs/index.php">Here!</a>
 * \warning The above old docs page is invalid since v1.4 of this library due to significant function changes.
 *
 * \section Installation
 * To install the library, the prefered method is by using the Arduino's Library Manager. Simply looking for <em>simple led matrix</em> 
 * should bring up this library. THe other way to install this library is to install it as a ZIP library downloaded from the Github repository.
 */

#ifndef SIMPLEMATRIX_H
#define SIMPLEMATRIX_H

#include "Arduino.h"
#include <SPI.h>
#include <avr/pgmspace.h>

#define FONT_CHAR_LENGHT 6

class simpleMatrix{
    public:
        /**
         * \brief The class constructor of this library.
         * \param pin The Arduino pin in which the CS line from the matrix is connected to.
         * \param rotateIndividualDislay Whether to rotate each individual display while sending data. 
                                         Useful as some display are 'inverted' in their LED matrix orientation.
         * \param numb_modules The number of modules. Defaults to 4 8x8 LED matrices
         */
        simpleMatrix(int pin, bool rotateIndividualDislay = false, unsigned int numb_modules=4);

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
        void clearDisplay(int from=0, int to=0x8000);

        /**
         * \brief Fills the display.
         * By default, this function will fill the entire display (all matrices).
         * \param from Which LED matrix to start filling from.
         * \param to Which LED matrix to fill up to.
         */
        void fillDisplay(int from=0, int to=0x8000);
        
        
        /**
         * \brief Scrolls a text left or right
         * \param del The delay between each frame while scrolling.
         * \param *text Any word/sentence you want.
         * \param left_to_right Whether the text will be scrolled left to right or right to left. \n
         *                      false for right to left, true for left to right
         * \param start_from Which column the text will start scrolling from. 
         *                   Defaults to have the display be empty then having the text apear for both directions
         */
        void scrollText(const char *text, int del, bool left_to_right=false, int start_from=0x8000);

        /**
         * \brief Wrapper for scrollText() that simplifies programming. Scrolls text from left to right.
         * \param *text Any word/sentence you want.
         * \param del The delay between each frame while scrolling.
         * \param start_from Where will the test start from. Defaults to having the text's ending be at the start of the matrix array.
         */
        void scrollTextLeftToRight(const char *text, int del, int start_from=0x8000);

        /**
         * \brief Wrapper for scrollText() that simplifies programming. Scrolls text from right to left.
         * \param *text Any word/sentence you want.
         * \param del The delay between each frame while scrolling.
         * \param start_from Where will the test start from. Defaults to having the end of the text at the begining of the matrix.
         */
        void scrollTextRightToLeft(const char *text, int del, int start_from=0x8000);

        /**
         * \brief Same as function scrollText(), but the text is stored in Flash memory instead of RAM.
         *        This allows for arbitrarily long messages to be displayed without any RAM issues/overflow.
         * \param *text A constant char array consisting of the word/sentence to be sent out. \n
         *              \b WARNING: For AVR architectures, the *text array must also have the pretense PROGMEM so the string is stored in the flash memory instead of RAM.
         * \param left_to_right Whether the text will be scrolled left to right or right to left. \n
         *                      false for right to left, true for left to right
         * \param del The delay between each frame while scrolling.
         * \param start_from Which column the text will start scrolling from.
         */
        void scrollTextPROGMEM(const char *text, int del, bool left_to_right=false, int start_from=0x8000);
        
        /**
         * \brief Same as scrollTextLeftToRight(), but the text is stored in FLASH memory instead of RAM (with PROGMEM)
         * \param *text A constant char array consisting of the word/sentence to be sent out. \n
         *              \b WARNING: For AVR architectures, the *text array must also have the pretense PROGMEM so the string is stored in the flash memory instead of RAM.
         * \param del The delay between each frame while scrolling.
         * \param start_from Where will the test start from. Defaults to having the text's ending be at the start of the matrix array.
         */
        void scrollTextPROGMEMLeftToRight(const char *text, int del, int start_from=0x8000);

        /**
         * \brief Same as scrollTextRightToLeft(), but the text is stored in FLASH memory instead of RAM (with PROGMEM)
         * \param *text A constant char array consisting of the word/sentence to be sent out. \n
         *              \b WARNING: For AVR architectures, the *text array must also have the pretense PROGMEM so the string is stored in the flash memory instead of RAM.
         * \param del The delay between each frame while scrolling.
         * \param start_from Where will the test start from. Defaults to having the end of the text at the begining of the matrix.
         */
        void scrollTextPROGMEMRightToLeft(const char *text, int del, int start_from=0x8000);
        
        /**
         * \brief Sends some text to the display, with the option to have it scroll.
         * \param *text The string that you want to send (must be a char array).
         * \param start_from The position where the text will start in the display.
         * \param is_text_progmem Whether the text is stored in Flash or RAM.
         * \param scroll_text Whether the outputed display will be scrolled from right to left.
         * \param del The delay between each frame while scrolling.
         * \param left_to_right false for the text to be scrolled from left to right, true to be scrolled from right to left.
         */
        void print(const char *text, int start_from=0, bool is_text_progmem=false, bool scroll_text=false, int del=0, bool left_to_right=false);
        
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
        void scrollBuffer(uint8_t *mat, int del, int column, int start_from=0x8000);

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
         * \param start_from The offset in the *mat where the function will start accessing.
         */
        void sendMatrixBuffer(uint8_t *mat, int start_from=0);
    private:
        uint8_t *_matrix;                   // The display and row addressed array containing data to be sent to the displays
        uint8_t *_matrix_col;               // A column-addressed array containing the previous data. Used for memory
        bool _ROTATE_INDIV_DISPLAY;         // If the displays are individually rotated by 180
        unsigned int _DL_PIN;               // The CS pin of the LED matrix
        int _NUMB_OF_LED_MATRICES;          // The number of LED matrix modules
        
        //Sends the _matrix buffer to the displays
        void senddisplay(); 
        // Internal function to send a command and data to all matrices.
        void sendCommandtoAll(uint8_t command, uint8_t data);
        // Internal function to send a command and data to a specific display number.
        void sendCommandtoOne(uint8_t command, uint8_t data, uint8_t display);
        // Internal function to scroll some text from left to right
        void scroll_text_left_to_right(const char *text, int del, bool is_text_progmem, int missed_text_cols, int start_letter_on_matrix, uint8_t *display);
        // Internal function to scroll some text from right to left
        void scroll_text_right_to_left(const char *text, int del, bool is_text_progmem, int text_cols_to_send, int end_letter_on_matrix, uint8_t *display, int text_arr_lenght);
};

#endif
