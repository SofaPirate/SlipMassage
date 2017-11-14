import processing.serial.*;

Serial serial;
String serialPort = "COM18";

SlipMassagePacker outbound = new SlipMassagePacker();
SlipMassageParser inbound = new SlipMassageParser(this);

int byteValue; // Arduino byte = Processing int limited between 0 and 255
int intValue;
float floatValue;
int longValue; // Arduino long = Processing int

int receivedByte = 0;  // Arduino byte = Processing int limited between 0 and 255
int receivedInt = 0;
float receivedFloat = 0;
int receivedLong = 0; // Arduino long = Processing int

void setup() {
  size(400, 400);
  
  println("Available ports :");
  String[] availablePorts =  Serial.list();
  for ( int i = 0; i < availablePorts.length ; i++) {
    println("* "+ availablePorts[i]);
  }
  
  println("Opening serial port "+ serialPort);
  
  serial = new Serial(this, serialPort, 57600);
  
}

void draw() {

  background(0);

  while ( inbound.parseStream(serial) ) {

    if ( inbound.fullMatch("address") ) {
      // GET THE DATA
      receivedByte = inbound.nextByte();
      receivedInt = inbound.nextInt();
      receivedFloat = inbound.nextFloat();
      receivedLong = inbound.nextLong();
    } else {
      println("Received unexpected massage");
    }
  }

  text("Sent to Arduino", 10, 30 );
  text(byteValue, 10 , 50);
  text(intValue, 10 , 70);
  text(floatValue, 10 , 90);
  text(longValue, 10 , 110);

  text("Received from Arduino", width*0.5 , 30 );
  text(receivedByte, width*0.5 , 50);
  text(receivedInt, width*0.5 , 70);
  text(receivedFloat, width*0.5 , 90);
  text(receivedLong, width*0.5 , 110);
}




void mousePressed() {

  byteValue = (int) random(0, 255);
  intValue = (int) random(-32768, 32767);
  floatValue = random(-100, 100);
  longValue = (int) random(-2147483648, 2147483647);

  outbound.beginPacket("address");  /// Begins a packet.

  outbound.addByte(byteValue); /// Adds a byte.
  outbound.addInt(intValue); /// Adds an int.
  outbound.addFloat(floatValue); /// Adds a float.
  outbound.addLong(longValue); /// Adds a long.

  outbound.streamPacket(serial); /// Ends the massage and streams it.
}