#include <FastLED.h>
#include "RTClib.h"

RTC_DS3231 rtc;
DateTime now;

#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define DATA_PIN 2
#define BRIGHTNESS 50

#define ROWS 11
#define COLS 11
#define NUM_LEDS 121
CRGB leds[NUM_LEDS];

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
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);

  // set the compiling machine time
  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // Serial.begin(9600);
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
  unsigned int shifted = (now.hour() * 60 + now.minute() + 35) % (12 * 60);

  // Serial.print(now.hour());
  // Serial.print(" ");
  // Serial.println(now.minute());

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

    if(now.minute() < 5){
      UHR
    }else if(now.minute() < 10){
      FUENF NACH
    }else if(now.minute() < 15){
      ZEHN NACH
    }else if(now.minute() < 20){
      VIERTEL NACH
    }else if(now.minute() < 25){
      ZWANZIG NACH
    }else if(now.minute() < 30){
      FUENF VOR HALB
    }else if(now.minute() < 35){
      HALB
    }else if(now.minute() < 40){
      FUENF NACH HALB
    }else if(now.minute() < 45){
      ZWANZIG VOR
    }else if(now.minute() < 50){
      VIERTEL VOR
    }else if(now.minute() < 55){
      ZEHN VOR
    }else{
      FUENF VOR
    }
  }
  
  FastLED.show();
  delay(5000);
}

void setLEDs(byte row, byte from, byte to){
  for(int j = from; j <= to; j++){
    setLED(row, j);
  }
}

void setLED(byte row, byte col){
  // TODO something fancier with colors
  leds[idx(row, col)].setRGB(170, 0, 255);
}

byte idx(byte row, byte col){
  // Row-wise snake starting from bottom right
  if(row % 2 == 0){
    return (COLS * (ROWS - 1 - row)) + (COLS - 1 - col);
  }else{
    return (COLS * (ROWS - 1 - row)) + col;
  }
}
