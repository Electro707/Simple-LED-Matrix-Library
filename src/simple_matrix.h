/**
 * \mainpage
 * \brief A library designed to allow for an easy and quick experience programing an LED matrix with an Arduino, even with little prior knowledge of coding or electronics.
 *
 * \image html train_choo.gif "Train goes brrrr" width=12%
 *
 * \todo Create more examples.
 * \todo Add full support for the MAX7221 chipset.
 * 
 * \section Installation Installation 
 * To install the library, the prefered method is by using the Arduino's Library Manager. Simply looking for <em>simple led matrix</em> 
 * should bring up this library. THe other way to install this library is to install it as a ZIP library downloaded from the Github repository.
 * 
 * \section Wiring Wiring 
 * On the hardware side, wire the Arduino to the LED matrix in the following fashion
 * <table>
 * <caption>Wiring Table</caption>
 * <tr><th>Arduino Pinout<th>LED Matrix Pinout
    <tr><td>VCC<td>VCC
    <tr><td>GND<td>GND
    <tr><td>D11 (Fixed)<td>DIN
    <tr><td>D13 (Fixed)<td>CLK
    <tr><td>D4 (Adjustable by software)<td>CS
    </table>
 * 
 * \section GitHub GitHub Repo
 * <a href="https://github.com/Electro707/Simple-LED-Matrix-Library">Link to the library's Github repository</a>
 *
 */

#ifndef SIMPLEMATRIX_H
#define SIMPLEMATRIX_H

#ifndef TEST_EXTERNAL
#include "Arduino.h"
#include <SPI.h>
#ifdef ARDUINO_ARCH_AVR
#include <avr/pgmspace.h>
#endif
#else
#include "stdint.h"
#include "string.h"
#include "Arduino.h"
#endif

#define FONT_CHAR_LENGHT 6

class simpleMatrix{
    public:
        /**
         * \brief The class constructor of this library.
         * \param pin The Arduino pin in which the CS line from the matrix is connected to.
         * \param numb_modules The number of modules. Defaults to 4 8x8 LED matrices
         */
        simpleMatrix(int pin, unsigned int numb_modules=4);

        /**
         * \brief Begins the library and initializes the display.
         */
        void begin();
        
        /**
         * \brief Function to invert each individual display 180 degrees.
         * 
         * The motive behind this is that some LED matrices have their LED matrix wired 180 degrees from other
         * (we are talking about hung-low modules, so consistency isn't necessarily something I would nessesary expect),
         * so this function is the fix for that.
         * 
         * \param rotate Whether to rotate the displays or not. By default set to true/
         */
        void invertIndividualDisplays(bool rotate = true);
        
        /**
         * \brief Function to set the display in vertical mode
         * 
         * This will mainly effect where (0,0) starts, as this command will adjust that to (0,0) of a vertical display (so
         * upper left corner if the displays are layed out horizontally)
         * 
         * \param is_vertical Whether the LED matrix is setup vertically. By default set to true.
         */
        void verticalDisplays(bool is_vertical = true);

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
         * 
         * \section Example
         * \code{.cpp}
         *   disp.fillDisplay();
         * \endcode
         * \image html fillDisplay1.gif "fillDisplay1" width=12%
         * \code{.cpp}
         *   disp.fillDisplay(0, 2);
         * \endcode
         * \image html fillDisplay2.gif "fillDisplay2" width=12%
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
         *
         * \section Example
         * \code{.cpp}
         *   disp.scrollText("Hello World!", 50);
         * \endcode
         * \image html scrollText1.gif "scrollText1" width=12%
         * \code{.cpp}
         *   disp.scrollText("Hello World!", 50, true);
         * \endcode
         * \image html scrollText2.gif "scrollText2" width=12%
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
         * \param del The delay between each frame while scrolling.
         * \param left_to_right false for the text to be scrolled from left to right, true to be scrolled from right to left.
         * \param is_text_progmem Whether the text is stored in Flash or RAM.
         *
         * \section Example
         * \code{.cpp}
         *   disp.print("Hello World!");
         * \endcode
         * \image html print1.gif "print1" width=12%
         * \code{.cpp}
         *   disp.print("Hello World!");   // just for a 1 second display
         *   delay(1000);
         *   disp.print("Hello World!", 0, return_delay);
         * \endcode
         * \image html print2.gif "print2" width=12%
         * \code{.cpp}
         *   disp.print("Hello World!", 32, return_delay);
         * \endcode
         * \image html print3.gif "print3" width=12%
         */
        void print(const char *text, int start_from=0, int del=0, bool left_to_right=false, bool is_text_progmem=false);
        
        /**
         * \brief Set the intensity of the LED matrix.
         * \param intensity The intensity of the display from 0-15.
         */
        void setIntensity(int intensity);
        
        /**
         * \brief Sets a single pixel in the LED matrices.
         * \param x,y The coordinates of the pixel to be set.
         *
         * \section Example
         * \code{.cpp}
         *   disp.setPixel(0, 0);
         *   disp.setPixel(7, 7);
         *   disp.setPixel(12, 5);
         *   disp.setPixel(20, 3);
         * \endcode
         * \image html setPixel1.gif "setPixel1" width=12%
         * \code{.cpp}
         *   for(int y=0;y<8;y++){
         *       for(int x=0;x<32;x++){
         *           disp.setPixel(x, y);
         *       }
         *   }
         * \endcode
         * \image html setPixel2.gif "setPixel2" width=12%
         */
        void setPixel(int x, int y);
        
        /**
         * \brief Clears a single pixel in the LED matrices.
         * \param x,y The coordinates of the pixel to be cleared.
         *
         * \section Example
         * \code{.cpp}
         *   disp.fillDisplay();
         *   disp.clearPixel(0, 0);
         *   disp.clearPixel(7, 7);
         *   disp.clearPixel(12, 5);
         *   disp.clearPixel(20, 3);
         * \endcode
         * \image html clearPixel1.gif "clearPixel1" width=12%
         * \code{.cpp}
         *   disp.fillDisplay();
         *   for(int y=0;y<8;y++){
         *       for(int x=0;x<32;x++){
         *           disp.clearPixel(x, y);
         *       }
         *   }
         * \endcode
         * \image html clearPixel2.gif "clearPixel2" width=12%
         */
        void clearPixel(int x, int y);
        
        /**
         * \brief Sets a row of pixels
         * \param x0 The start x position for the pixels to be set
         * \param x1 The end x position for the pixels to be set
         * \param y The y position of the pixels to be set
         * 
         * \section Example
         * \code{.cpp}
         *   disp.setColumnPixels(0, 0, 2);
         *   disp.setColumnPixels(3, 5, 7);
         *   disp.setColumnPixels(8, 0, 7);
         *   disp.setColumnPixels(12, 3, 5);
         * \endcode
         * \image html setRowPixels1.gif "setRowPixels1" width=12%
         */
        void setRowPixels(int x0, int x1, int y);
        
        /**
         * \brief Clears a row of pixels
         * \param x0 The start x position for the pixels to be cleared
         * \param x1 The end x position for the pixels to be cleared
         * \param y The y position of the pixels to be cleared
         */
        void clearRowPixels(int x0, int x1, int y);
        
        /**
         * \brief Set a column of pixels
         * \param x The x position of the pixels to be set
         * \param y0 The start y position for the pixels to be set
         * \param y1 The end y position for the pixels to be set
         * 
         * \section Example
         * \code{.cpp}
         *   disp.setColumnPixels(0, 0, 2);
         *   disp.setColumnPixels(3, 5, 7);
         *   disp.setColumnPixels(8, 0, 7);
         *   disp.setColumnPixels(12, 3, 5);
         * \endcode
         * \image html setColumnPixels1.gif "setColumnPixels1" width=12%
         */
        void setColumnPixels(int x, int y0, int y1);
        
        /**
         * \brief Clears a column of pixels
         * \param x The x position of the pixels to be cleared
         * \param y0 The start y position for the pixels to be cleared
         * \param y1 The end y position for the pixels to be cleared
         */
        void clearColumnPixels(int x, int y0, int y1);
        
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
        void sendMatrixBuffer(uint8_t *mat=NULL, int start_from=0);
        
#ifdef TEST_EXTERNAL
        uint8_t *return_external_matrix(void);
#endif

    private:
        uint8_t *_matrix;                   // The display and row addressed array containing data to be sent to the displays
        uint8_t *_matrix_col;               // A column-addressed array containing the previous data. Used for memory
        bool _ROTATE_INDIV_DISPLAY;         // If the displays are individually rotated by 180
        unsigned int _DL_PIN;               // The CS pin of the LED matrix
        int _NUMB_OF_LED_MATRICES;          // The number of LED matrix modules
        bool _IS_DISPLAY_VERTICAL;          // Whether the display is vertical or not
        
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
