#include <Adafruit_NeoPixel.h>

#define PIXELS_DATA_PIN  6
#define NUMPIXELS        60
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIXELS_DATA_PIN, NEO_GRB + NEO_KHZ800);

#include <SlipMassagePacker.h>
#include <SlipMassageParser.h>

SlipMassagePacker outbound;
SlipMassageParser inbound;

void setup() {
  Serial.begin(115200); // Initiate serial communication.

  pixels.begin(); // This initializes the NeoPixel library.
}

void massageReceived() {

  if ( inbound.fullMatch("pixels")) {

    for ( int i = 0; i < NUMPIXELS ; i++ ) {
      byte red = inbound.nextByte(); // ...read the next element as a byte (0-255)...
      byte green = inbound.nextByte();
      byte blue = inbound.nextByte();

      pixels.setPixelColor(i, pixels.Color(red, green, blue));
    }
    pixels.show();
  }
}

void loop() {

  // PARSE INPUT AND EXECUTRE massageReceived IF A COMPLETE MASSAGE IS RECEIVED
  inbound.parseStream( &Serial , massageReceived );




}




