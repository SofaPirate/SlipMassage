
#include <SlipMassagePacker.h>
#include <SlipMassageParser.h>

SlipMassagePacker outbound;
SlipMassageParser inbound;

#include "LedBlinker.h"
LedBlinker blinker(13);

#include <OctoWS2811.h>

#define LEDS_PER_STRIP 100

DMAMEM int displayMemory[LEDS_PER_STRIP * 6];
int drawingMemory[LEDS_PER_STRIP * 6];

const int config = WS2811_GRB | WS2811_800kHz;

OctoWS2811 leds(LEDS_PER_STRIP, displayMemory, drawingMemory, config);

void setup() {

  blinker.begin();

  Serial.begin(115200); // Initiate serial communication.

  leds.begin(); // This initializes the NeoPixel library.



}

void massageReceived() {

  if ( inbound.fullMatch("pixels")) {

    // FIRST BYTE IS STRIP NUMBER (STARTS AT 1)

    int ledStrip =  inbound.nextByte();

    // IF STRIP IS 0, IT MEANS IT IS SPECIAL DATA
    if ( ledStrip == 0 ) {
      // SPECIAL DATA
    } else {
      // PIXEL DATA
      ledStrip = ledStrip - 1; // offset the led strip number

      // SECOND BYTE IS COUNT
      int ledCount = inbound.nextByte();

      int startIndex = ledStrip * LEDS_PER_STRIP;

      // IF COUNT IS 0, IT MEANS TO SET THE WHOLE CHANNEL TO THE VALUE
      if ( ledCount <= 0 ) {

        int stopIndex = startIndex + LEDS_PER_STRIP;

        byte red = inbound.nextByte(); // ...read the next element as a byte (0-255)...
        byte green = inbound.nextByte();
        byte blue = inbound.nextByte();
        for ( int i = startIndex ; i < stopIndex ; i++ ) {
          leds.setPixel(i, red, green, blue);
        }
        leds.show();

      } else {

        int stopIndex = startIndex + ledCount;

        for ( int i = startIndex ; i < stopIndex ; i++ ) {

          byte red = inbound.nextByte(); // ...read the next element as a byte (0-255)...
          byte green = inbound.nextByte();
          byte blue = inbound.nextByte();

          leds.setPixel(i, red, green, blue);
        }
        leds.show();
      }
    }
  }
  blinker.shortBlink();
}

void loop() {

   blinker.update();
 


  // PARSE INPUT AND EXECUTRE massageReceived IF A COMPLETE MASSAGE IS RECEIVED
  inbound.parseStream( &Serial , massageReceived );

}
