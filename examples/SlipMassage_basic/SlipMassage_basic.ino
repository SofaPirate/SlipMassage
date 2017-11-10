#include <SlipMassagePacker.h>
#include <SlipMassageParser.h>

SlipMassagePacker outbound;
SlipMassageParser inbound;

void setup() {
  Serial.begin(57600); // Initiate serial communication.
}

void massageReceived() {

  if ( inbound.fullMatch("address")) {
    byte b1 = inbound.nextByte(); // ...read the next element as a byte (0-255)...
    byte b2 = inbound.nextByte();
    byte b3 = inbound.nextByte();

    // ECHO THE DATA
    outbound.beginPacket("address"); /// Begins a packet.
    outbound.addByte(b1);
    outbound.addByte(b2);
    outbound.addByte(b3);
    outbound.streamPacket(&Serial);

    

  }

}

void loop() {

  // PARSE INPUT AND EXECUTRE massageReceived IF A COMPLETE MASSAGE IS RECEIVED
  inbound.parseStream( &Serial , massageReceived );
  

  

}




