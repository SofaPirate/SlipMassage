// A PROCESSING EXAMPLE TO COMMUNICATE WITH THIS SKETCH CAN BE FOUND  INSIDE
// "extras/applications/Processing/SlipMassage_Processing/" OF THE FOLLOWING DOWNLOAD :
// https://github.com/SofaPirate/SlipMassage/archive/master.zip

// THIS EXAMPLE SIMPLY ECHOS BACK A MASSAGE CONTAINING THE FOUR DATATPYES.

// INCLUDE MASSAGE
#include <SlipMassagePacker.h>
#include <SlipMassageParser.h>

// PACKER(FOR SENDING) AND PARSER(FOR RECEIVING) INSTANCES.
SlipMassageParser inbound;
SlipMassagePacker outbound;


///////////
// SETUP //
///////////
void setup() {

  // INITIATE SERIAL COMMUNICATION.
  Serial.begin(57600);

}

//////////////////////
// SEND AND RECEIVE //
//////////////////////
// THE FOLLOWING FUNCTIONS ARE HELPER FUNCTIONS.
// sendPacket() SENDS OUT A PRE-PACKED MASSAGE.
// receivePacket() CHECK FOR A COMPLETED MASSAGE AND
// INDICATES WHAT TO DO WITH ITS CONTENTS.




//////////
// LOOP //
//////////
void loop() {

  if ( inbound.parseStream( &Serial ) ) {
    if ( inbound.fullMatch("address") ) {
      // GET THE DATA
      byte byteValue = inbound.nextByte();
      int intValue = inbound.nextInt();
      float floatValue = inbound.nextFloat();
      long longValue = inbound.nextLong();

      // ECHO THE DATA
         
      outbound.beginPacket("address");  /// Begins a packet.
      
      outbound.addByte(byteValue); /// Adds a byte.
      outbound.addInt(intValue); /// Adds an int.
      outbound.addFloat(floatValue); /// Adds a float.
      outbound.addLong(longValue); /// Adds a long.
      
      outbound.streamPacket(&Serial); /// Ends the massage and streams it.

    } else {
      // SEND "what?" WHEN A MASSAGE IS NOT RECOGNIZED.
      outbound.streamEmpty(&Serial, "what?");

    }
  }

}




