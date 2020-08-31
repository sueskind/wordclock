#include <FastLED.h>
#include "RTClib.h"

// RTC
RTC_DS3231 rtc;
DateTime now;

// LEDs
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define LED_PIN 5
#define ROWS 11
#define COLS 11
#define NUM_LEDS 121
#define BRIGHTNESS_MAX 150 // up to 255 (but high numbers draw a lot of power with little effect)
#define BRIGHTNESS_MIN 3   // down to 0 (though very small values omit some colors)
CRGB leds[NUM_LEDS];

// Buttons, nano has interrupt pins D2 and D3
#define BUTTON1 2
#define BUTTON2 3
unsigned long lastInterrupt = 0;

// colors, cycled by BUTTON1
#define NUM_COLORS 8
byte hues[NUM_COLORS] = {0, 32, 64, 96, 128, 160, 192, 224};
volatile byte hueIndex;
byte saturation = 255;
byte value = 255;

// Photoresistor light sensor
#define SENSOR_PIN A3
#define SENSOR_BRIGHT 900
#define SENSOR_DARK 100
#define FADING_SPEED 0.05    // 0 - 1
float previousBrightness = 0.0;
float nextBrightness;

// night off mode (since nobody sees it during the night)
// **do things exist while nobody sees it?**
#define OFF_FROM 1  // 1:00
#define OFF_TO 6    // 6:00

// word definitions
#define ES_IST      setLEDs(0,0,1);setLEDs(0,3,5);
#define FUENF       setLEDs(0,7,10);
#define ZWANZIG     setLEDs(1,0,6);
#define ZEHN        setLEDs(1,7,10);
#define VIERTEL     setLEDs(2,2,8);
#define NACH        setLEDs(3,1,4);
#define VOR         setLEDs(3,6,8);
#define HALB        setLEDs(4,0,3);
#define ZWOELF      setLEDs(4,5,9);
#define ZWEI        setLEDs(5,0,3);
#define EIN         setLEDs(5,2,4);
#define S           setLED(5,5);
#define SIEBEN      setLEDs(5,5,10);
#define DREI        setLEDs(6,1,4);
#define FUENF_h     setLEDs(6,7,10);
#define NEUN        setLEDs(7,2,5);
#define VIER        setLEDs(7,7,10);
#define ELF         setLEDs(8,1,3);
#define ZEHN_h      setLEDs(8,5,8);
#define MITTERN     setLEDs(9,0,6);
#define ACHT        setLEDs(9,7,10);
#define SECHS       setLEDs(10,0,4);
#define UHR         setLEDs(10,8,10);

void setup() {
  FastLED.addLeds<LED_TYPE,LED_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  randomSeed(analogRead(0));
  hueIndex = byte(random(NUM_COLORS));
  pinMode(BUTTON1, INPUT);
  attachInterrupt(0, button1Interrupted, RISING);
  
  // set the compiling machine time
  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

// int step = 0; // mock
void loop() {
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i].setRGB(0, 0, 0);
  }
  
  // mock
  // step+=5;
  // now = DateTime(2000,1,1,step / 60 % 24,step % 60);

  now = rtc.now();

  if(now.hour() >= OFF_FROM && now.hour() < OFF_TO){
    FastLED.show();
    delay(1000 * 60);
  }else{

    // shift time for 
    unsigned int shifted = (now.hour() * 60 + now.minute() + 35) % (12 * 60);
  
    ES_IST
    
    // midnight
    if(now.hour() == 0 && now.minute() < 5){
      MITTERN ACHT
  
    }else{
      switch(shifted / 60 % 12){
        case 0: ZWOELF break;
        case 1: EIN if(now.minute() % 60 >= 5) S break; // s not at full hour
        case 2: ZWEI break;
        case 3: DREI break;
        case 4: VIER break;
        case 5: FUENF_h break;
        case 6: SECHS break;
        case 7: SIEBEN break;
        case 8: ACHT break;
        case 9: NEUN break;
        case 10: ZEHN_h break;
        case 11: ELF break;
      }

      // so it says 5 after X from X:03 to X:07 etc.
      byte shiftedMinute = now.minute() + 2;
      
      if(shiftedMinute % 60 < 5){
        UHR
      }else if(shiftedMinute < 10){
        FUENF NACH
      }else if(shiftedMinute < 15){
        ZEHN NACH
      }else if(shiftedMinute < 20){
        VIERTEL NACH
      }else if(shiftedMinute < 25){
        ZWANZIG NACH
      }else if(shiftedMinute < 30){
        FUENF VOR HALB
      }else if(shiftedMinute < 35){
        HALB
      }else if(shiftedMinute < 40){
        FUENF NACH HALB
      }else if(shiftedMinute < 45){
        ZWANZIG VOR
      }else if(shiftedMinute < 50){
        VIERTEL VOR
      }else if(shiftedMinute < 55){
        ZEHN VOR
      }else{
        FUENF VOR
      }
    }
  
    updateBrightness();
    FastLED.show();
    delay(200);
  }
}

void button1Interrupted(){
  // software solution to avoid double triggering interrupt
  if(millis() - lastInterrupt > 400){
    hueIndex = (hueIndex + 1) % NUM_COLORS;
    lastInterrupt = millis();
  }
}

void updateBrightness(){
  // Assuming linear relationship between all observables
  int env = analogRead(SENSOR_PIN);
  if(env > SENSOR_BRIGHT){
    nextBrightness = BRIGHTNESS_MAX;
  }else if(env < SENSOR_DARK){
    nextBrightness = BRIGHTNESS_MIN;
  }else{
    // straight line equation
    nextBrightness = float(BRIGHTNESS_MAX - BRIGHTNESS_MIN)
                     / float(SENSOR_BRIGHT - SENSOR_DARK)
                     * float(env - SENSOR_DARK)
                     + float(BRIGHTNESS_MIN);
  }
  
  // calculating weighted average ensures proper fading
  nextBrightness = nextBrightness * FADING_SPEED
                   + previousBrightness * (1 - FADING_SPEED);
  FastLED.setBrightness(nextBrightness);
  previousBrightness = nextBrightness;
}

void setLEDs(byte row, byte from, byte to){
  for(int j = from; j <= to; j++){
    setLED(row, j);
  }
}

void setLED(byte row, byte col){
  // TODO something fancier with colors
  leds[idx(row, col)].setHSV(hues[hueIndex], saturation, value);
}

byte idx(byte row, byte col){
  // Row-wise snake starting from bottom right
  if(row % 2 == 0){
    return (COLS * (ROWS - 1 - row)) + (COLS - 1 - col);
  }else{
    return (COLS * (ROWS - 1 - row)) + col;
  }
}
