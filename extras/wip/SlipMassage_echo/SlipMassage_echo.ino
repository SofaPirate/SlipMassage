#include <SlipMassagePacker.h>
#include <SlipMassageParser.h>

SlipMassagePacker outbound;
SlipMassageParser inbound;

void setup() {
  Serial.begin(57600); // Initiate serial communication.
}

void loop() {
/*
  outbound.beginPacket("test");
  outbound.addByte(millis()%256);
  outbound.endPacket();
  Serial.write(outbound.buffer(), outbound.size() );
  delay(500);
*/
// PARSE INPUT AND EXECUTRE massageReceived IF A COMPLETE MASSAGE IS RECEIVED 
   while ( Serial.available() ) inbound.parse( Serial.read() , massageReceived );
  
}


void massageReceived() {

  if ( inbound.fullMatch("echo")) {
    byte b1 = inbound.nextByte(); // ...read the next element as an int...
    byte b2 = inbound.nextByte();
    byte b3 = inbound.nextByte();

    outbound.beginPacket("echo");
    outbound.addByte(b1);
    outbound.addByte(b2);
    outbound.addByte(b3);
    outbound.endPacket();
    
    Serial.write(outbound.buffer(),outbound.size());
    
  }
 
}

