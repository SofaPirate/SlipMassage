# SlipMassage

*SlipMassage* is a microcontroller message packer and parser for the SLIP format in different protocols. A *SlipMassage* compatible message is called a *massage*.

A *massage* message always starts with an address string and is followed by a user defined number of bytes, ints, longs, or floats.  The address string is used to route the message as in OSC.

[This repository](https://github.com/SofaPirate/SlipMassage) is the SLIP implementation of the [Massage API](https://github.com/SofaPirate/Massenger).

*Massages* can be sent and received over different communication protocols such as serial, UDP, or web sockets.

Packing and sending a massage example
-------------------------------------

Add the library to the top of your code and instantiate an SlipMassagePacker called "outbound":
```
#include <SlipMassagePacker.h>
SlipMassagePacker outbound;
```

Pack an example massage:
```
outbound.beginPacket("value"); // Start a packet with the address called "value".
outbound.addLong( millis() ); // Add the milliseconds.
outbound.addInt( analogRead(0) ); // Add a reading of analog 0.
outbound.endPacket(); // End the packet.
```

Send the packed massage (through serial in this case):
```
Serial.write( outbound.buffer(), outbound.size() );
```

Receiving and parsing a massage example
---------------------------------------


Receiving and parsing a massage
-------------------------------
Add the library to the top of your code and instantiate an SlipMassageParser called "inbound":
```
#include <SlipMassageParser.h>
SlipMassageParser inbound;

```

Inside loop() receive the data (with serial in this case) and parse it with the SlipMassageParser. If parse() returns true, the massage is completed and ready to parse.
```
while ( Serial.available() ) {
    if ( inbound.parse( Serial.read() ) ) {
       // parse completed massage here.
    }
}
```

This example parses a massage that starts with the address "value" and that contains one long followed by one int:
```
// Does the massage's address match "value"?
if ( inbound.fullMatch ("value") ) {
    // Get the first long.
    long ms = inbound.nextLong();
    // Get the next int.
    int an0 = inbound.nextInt();
}
```

Receiving and parsing a massage with a callback example 
-------------------------------------------------------
Add the library to the top of your code and instantiate an SlipMassageParser called "inbound":
```
#include <SlipMassageParser.h>
SlipMassageParser inbound;
```

Inside loop() receive the data (with serial in this case), parse it with the SlipMassageParser and call the "massageReceived" function one a massage is completed:
```
while ( Serial.available() ) {
    inbound.parse( Serial.read() , massageReceived );
}
```

Add a function in your code called "massageReceived" to process the completed massage:
```
void massageReceived() {
	// parse completed massage here.
}
```

This example parses a massage that starts with the address "value" and that contains one long followed by one int:
```
// Does the massage's address match "value"?
if ( inbound.fullMatch ("value") ) {
    // Get the first long.
    long ms = inbound.nextLong();
    // Get the next int.
    int an0 = inbound.nextInt();
}
```


SlipMassageParser
-------------

`SlipMassageParser ()`: Constructor.
 
`parse (int data, callbackFunction callback)` : Reads one element of serial port, flushing previous message if needed. Returns true iff new message has arrived. Optional callback function will be called if new message has arrived. 
 
`fullMatch (const char *address)` : Return true if current message matches "address".
 
`dispatch (const char *address, callbackFunction callback)` : If current message matches "address", calls function "callback" and returns true.
 
`nextByte ()` : Returns next byte in the parsed massage.

`nextFloat()` : Returns next float in the parsed massage.

`nextInt ()` : Returns next int in the parsed massage.

`nextLong()` : Returns next long in the parsed massage.
 
SlipMassagePacker
-------------

`SlipMassagePacker()` : Constructor

`beginPacket(const char* address)` : Begins a packet (erasing any previous packet).

`addByte(uint8_t value)` : Adds a byte.

`addInt(int16_t value)` : Adds an int.

`addLong(int32_t value)` : Adds a long.

`addFloat(float value)` : Adds a float.

`endPacket()` : Ends a packet.

`packEmpty(const char *address)` : Create a packet with no arguments.
  
`packOneByte(const char *address, uint8_t value)` : Create a packet with a single byte value.

`packOneInt(const char *address, int16_t value)` : Create a packet with a single int value.

`packOneLong(const char *address, int32_t value)` :   Create a packet with a single long value.

`packOneFloat(const char *address, float value)` : Create a packet with a single float value.

`size()` : Returns the size of the packed massage.

`buffer()` : Returns a pointer to the buffer.



