# Arduino
Sketches to be used with the Adafruit Trinket, Metro Mini and Arduino microcontroller boards. It is assumed that Arduino/C++ is the language/programming environment. See "Programming Adafruit Trinket" at https://symbolicdomain.wordpress.com for details about installing and configuring the Arduino IDE.

Note that in order to upload these compiled sketches to the board (in linux), you will need to be added to the dialout group:

sudo usermod -a -G dialout <username>

Then log out and log in again.

After that you should be able to upload without an error.

Note also that the Trinket doesn't work with USB 3.0 - you'll need to connect it to a USB 2.0 in order to upload to it.
