# Wordclock
Wordclock with German layout Arduino Nano project. Design inspired by [grahamvinyl on Imgur](https://imgur.com/a/dtLSy).

![Wordclock](doc/wordclock.jpg)

Libraries used:
 - [FastLED](https://github.com/FastLED/FastLED)
 - [RTClib](https://github.com/adafruit/RTClib)

## Specs
 - 11x11 WS2812b LEDs
 - Two physical buttons
 - DS3231 real time clock
 - Photoresistor for brightness measurement

## Functionality
wordclock-basic:
 - Show current time to 5 min accuracy
 - Memorize time even when unplugged
 - Turn off light during the night
 
wordclock-advanced (additionally to basic):
 - Adjust brightness to room's lighting
 - Change color on button press
 - *Random shine effect (slow flickering/blinkin) (WIP)*
 - *Color effects (WIP)*
 - *Random animations when display changes (WIP)*
 - *Play Tetris (WIP, ESP32 needed)*
 - *Play Pong (WIP, ESP32 needed)*