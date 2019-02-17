/*****************************************************************************
  KX126_I2C.ino

 Copyright (c) 2018 ROHM Co.,Ltd.

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
******************************************************************************/
#include <Wire.h>
#include <math.h>
#include <Adafruit_NeoPixel.h>
#include "KX126.h"

#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6
#define NUM_PIX 300

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIX, PIN);

KX126 KX126(KX126_DEVICE_ADDRESS_1F);

void setup() {
  byte rc;

  Serial.begin(115200);
  while (!Serial);

  Wire.begin();

  rc = KX126.init();
  if (rc != 0) {
    Serial.println("KX126 initialization failed");
    Serial.flush();
  }
  strip.begin();
  strip.setBrightness(50);
  strip.show();
  delay(300);
}


int count = 0;
int toggle = 0;

void loop() {
  byte rc;
  float acc[3];
  float magnitude;
  float x,y,z;
  float cmp;
  rc = KX126.get_val(acc);
  if (rc == 0) {
    x = acc[0];
    y = acc[1];
    z = acc[2]; 
    // magnitude =( round( 100 * (sqrt(pow(x,2) + pow(y,2) + pow(z,2)) ) ) / 100.0 ) - 1 ;
    magnitude = abs( sqrt( pow(x,2) + pow(y,2) + pow(z,2) ) - 1 );
    cmp = ( 2.0 / magnitude );
    Serial.write("KX126 (Magnitude) = ");
    Serial.print(magnitude);
    Serial.print("\n");
    Serial.write("KX126 (cmp) = ");
    Serial.print(cmp);
    Serial.print("\n");
    if ( count > cmp && !toggle ) {
      light_on();
      count = 0;
      toggle++;
    }
    else if ( count > cmp && toggle ) {
      light_off();
      count = 0;
      toggle = 0;  
    }
    count++;
  }
}
void light_on () {
  // color back of hand RED
  strip.setPixelColor( 2 , 255, 0, 0 );
  strip.setPixelColor( 3 , 255, 0, 0 );
  strip.setPixelColor( 4 , 255, 0, 0 );
  strip.setPixelColor( 50 , 255, 0, 0 );
  strip.setPixelColor( 51 , 255, 0, 0 );
  strip.setPixelColor( 52 , 255, 0, 0 );
  strip.setPixelColor( 108 , 255, 0, 0 );
  strip.setPixelColor( 109 , 255, 0, 0 );
  strip.setPixelColor( 110 , 255, 0, 0 );
  // color front of hand YELLOW
  strip.setPixelColor( 81, 255, 255, 0 );
  strip.setPixelColor( 82, 255, 255, 0 );
  strip.setPixelColor( 83, 255, 255, 0 );
  strip.setPixelColor( 138 , 255, 255, 0 );
  strip.setPixelColor( 139, 255, 255, 0 );
  strip.setPixelColor( 140, 255, 255, 0 );
  // color index finger GREEN
  for ( int i = 116; i < 131; i = i + 2 ) {
    strip.setPixelColor( i , 0, 255, 0 );
  }
  // color middle finger BLUE
  for ( int i = 58; i < 77; i = i + 2 ) {
    strip.setPixelColor( i, 0, 0, 255 );
  }
  // color ring finger PURPLE
  for ( int i = 9; i < 28; i = i + 2) {
    strip.setPixelColor( i , 255, 0, 255 );
  }
  // display colors
  strip.show();
}

void light_off () {
 // set pixels to off
  for ( int i = 0; i < 141; i++ ) {
    strip.setPixelColor( i , 0 );  
  }
  // turn pixels off
  strip.show();
}
