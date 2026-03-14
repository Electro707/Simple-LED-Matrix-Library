# Simple LED Matrix Library
### By electro707 (Jamal Bouajjaj)

[![Compile Examples](https://github.com/Electro707/Simple-LED-Matrix-Library/actions/workflows/compile-examples.yml/badge.svg)](https://github.com/Electro707/Simple-LED-Matrix-Library/actions/workflows/compile-examples.yml)

This is a library to drive MAX7219 LED matrices. This library has been designed to be both simple to use for beginners, while leaving a lot to play around with for advanced users.
While the library tries to focus on simplicity and ease-of-use, performance was a major priority while developing this library (to allow for fast scrolling text for example).

## Getting Started

To get started, you will need both an Arduino (any kind should work), and a MAX7219 LED Matrix. Both parts can be bought from many places, including Amazon, Ebay, Aliexpress, Adafruit, etc.

### Docs
For documentation, see [the stable documentation page](https://electro707.com/documentation/Libraries/simple_led_library/stable/).

If you are working right off git, see the [git/unstable documentation](https://electro707.com/documentation/Libraries/simple_led_library/git/).

### Wiring LED Matrix to Arduino

To wire the LED Matrix to the Arduino, connect the following pins together.

| LED Matrix Pins | Arduino Pins |
| --- | --- |
| VCC | VCC |
| GND | GND |
| DIN | D11 (Fixed) |
| CLK | D13 (Fixed) |
| CS | D4 (Adjustable by software) |

The restriction for pins D11 and D13 on the Arduino is due to the microcontroller's SPI peripheral, which this library utilizes.

You could still hook up other SPI devices to D11->D13, just ensure only one CS (Chip Select) line is activated at a time.

### Installing Library

Install this library by using Arduino's Library Manager; looking up _simple led matrix_ should be sufficient to find it.

You could also download this library as a ZIP file and install it directly. See [here](https://www.arduino.cc/en/Guide/Libraries#importing-a-zip-library) for a guide from Arduino's website.
You could either download a release candidate (thru Github's release/tag manager) or the development/unstable branch directly from this repo's `master` branch.

## Release

All releases will be tagged and released, and will be available through Arduino's Library Manager.

If you want to experiment with the latest and greatest (and the buggiest), you should download the latest `master` branch.

## License

See the [LICENSE](LICENSE) file for details

## Extra
See [extra/README.md](extra/README.md) for details on how to generate the library documentation, test the examples, and generate the LED Matrix GIFs that are found in the library's docs page
