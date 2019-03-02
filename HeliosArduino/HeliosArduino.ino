#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#define PIN 6
#define POT_PIN A0

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

int directionInput;
int modeValue;
int count;

uint32_t off      = strip.Color(0, 0, 0);
uint32_t chill    = strip.Color(70, 50, 0);
uint32_t work     = strip.Color(0, 70, 70);
uint32_t ambient  = strip.Color(50, 50, 50);
uint32_t romantic = strip.Color(70, 10, 0);
uint32_t party    = strip.Color(0, 50, 0);;

uint32_t mode[6] = {off, work, romantic, party, ambient, chill};

int previousPosition;
int currentPosition;

void setup() {  
  pinMode(POT_PIN, INPUT); // initialize the pushbutton pin as an input
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  Serial.begin(9600); // Listen for input (9600, 19200 or 115200)
}

void loop() {  
  directionInput = Serial.read();
  modeValue = analogRead(POT_PIN);
  delay(5); // delay in between reads for stability
  modeValue = map(modeValue, 0, 1023, 0, 5);   // scale it to use it with the modes (value between 0 and 1023) 
  
  Serial.println(modeValue);
     
  if (modeValue != 0) {
    counter();
    switch (modeValue) {
      case 1: // Chill
        chosenDirection(30);
      break;
      case 2: // Work
        chosenDirection(30);
      break;
      case 3: // Ambient
        chosenDirection(30);
      break;
      case 4: // Romantic
        chosenDirection(30);
      break;
      case 5: // Party
        partyMode(party, 50);
      break;
    }
  }
  else { // Off/reset
    //allDirections(off, 30);
    count = 0; // Reset counter
    
    // if our position has changed
    if(directionInput != currentPosition) {  
      // take note of where we were    
      previousPosition = currentPosition;  
      // take note of where we are now 
      currentPosition = directionInput;  
      
      strip.setPixelColor(currentPosition, work); 
      strip.show();
      
      // make the previous LED turn off
      strip.setPixelColor(previousPosition, off);
      strip.show();
      delay(10);      
    }
  }
}

void counter() {
  if (count < 2) {
    count++;
    onebyOne();
  }
}

// Turn on the pixels one by one
void onebyOne() {
  for (int i = 1; i < strip.numPixels(); i++) {
    strip.setPixelColor((i-1), off);
    strip.setPixelColor(i, chill);
    delay(50);
    strip.show();
  }
}

// Fill the dots one after the other with a color
void allDirections(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < 10; i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

void chosenDirection(uint8_t wait) {
  switch (directionInput) {
    case 97: // Right
      for (uint16_t i = 18; i < 27; i++) {
        strip.setPixelColor(i, mode[modeValue]);
        strip.show();
        delay(wait);
      }
    break;
    case 98: // Left
      for (uint16_t i = 4; i < 13; i++) {
        strip.setPixelColor(i, mode[modeValue]);
        strip.show();
        delay(wait);
      }
    break;
    case 99: // Up
      for (uint16_t i = 14; i < 18; i++) {
        strip.setPixelColor(i, mode[modeValue]);
        strip.show();
        delay(wait);
      }
    break;
    case 100: // Down
      for (uint16_t i = 0; i < 4; i++) {
        strip.setPixelColor(i, mode[modeValue]);
        strip.show();
        delay(wait);
        for (uint16_t i = 28; i < 32; i++) {
          strip.setPixelColor(i, mode[modeValue]);
          strip.show();
          delay(wait);
        }
      }
    break;
    case 101: // Clockwise: All directions
      for (uint16_t i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, mode[modeValue]);
        strip.show();
        delay(wait);
      }
    break;
  }
}

//Theatre-style crawling lights.
void partyMode(uint32_t c, uint8_t wait) {
  for (int j = 0; j < 10; j++) {  //do 10 cycles of chasing
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, c);    //turn every third pixel on
      }
      strip.show();
     
      delay(wait);
     
      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } 
  else if(WheelPos < 170) {
    WheelPos -= 85;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } 
  else {
   WheelPos -= 170;
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}
