# Simple LED Matrix Library

This is a library to drive MAX7219 LED matrices. This library has been designed to be both simple to use for beginners, while leaving a lot to play around with for advanced users.

## Getting Started

To get started, you will need both an Arduino (any kind should work), and a MAX7219 LED Matrix. Both parts can be bought from many places, including Amazon, Ebay, Aliexpress, Adafruit, etc.

For more documentation, see [my website!](https://www.electro707.com/documentation/Libraries/simple_led_library/index.php)

## Wiring LED Matrix to Arduino

To wire the LED Matrix to the Arduino, connect the following pins together.

| LED Matrix Pins | Arduino Pins |
| --- | --- |
| VCC | VCC |
| GND | GND |
| DIN | D11 |
| CLK | D13 |
| CS | D4 (Adjustable by software) |

### Installing Library

Install this library by downloading this repository as a ZIP file, then importing it into the Arduino IDE like any other library!

Link to the official guide can be found [here!](https://www.arduino.cc/en/Guide/Libraries).
Follow the "Importing a .zip Library" section of the guide to import this library.

## To-Do
- Add documentation describing how the LED Matrix will be hooked up to the Arduino with drawings.
- Create examples demonstrating how to use the Python Generators and the library
- Create more examples

## Authors

* **Jamal Bouajjaj**

## License

See the [LICENSE](LICENSE) file for details

## Notes

* This project was originally design to be used for the University of New Haven's Makerspace's "Make-A-Sign" event, where people with no previous experience with Arduino or programming made a program which scrolled a text of their choice based on the demo file.
