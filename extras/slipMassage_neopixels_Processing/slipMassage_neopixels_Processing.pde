import processing.serial.*;

Serial serial;
String serialPort = "COM7";

SlipMassagePacker outbound = new SlipMassagePacker();
SlipMassageParser inbound = new SlipMassageParser(this);


void setup() {
  size(400, 400);

  println("Available ports :");
  String[] availablePorts =  Serial.list();
  for ( int i = 0; i < availablePorts.length; i++) {
    println("* "+ availablePorts[i]);
  }

  println("Opening serial port "+ serialPort);

  serial = new Serial(this, serialPort, 115200);
  
  frameRate(30);
}

void draw() {

  background(0);


  // RECEIVE DATA
  while ( inbound.parseStream(serial) ) {

    if ( inbound.fullMatch("sensors") ) {
      // DO SOMETHING
    
    } else {
      println("Received unexpected massage");
    }
  }

  // DRAW AN ANIMATED RAINBOW OUT OF 60 RECTANGLES
  colorMode(HSB, 360, 100, 100);
  noStroke();
  float rectWidth = width / 60.0;
  for (int i = 0; i < 60; i++) {

    float hue = map( i, 0, 60, 0, 360);

    hue = (hue + frameCount * 5) % 360;

    fill(hue, 100, 100);
    rect( floor(i*rectWidth), 0, ceil(rectWidth), height);
  }


  // SEND A LINE OF PIXELS
  loadPixels();


  outbound.beginPacket("pixels");
  
  for ( int i =0; i < 60; i++) {
    int x = floor( map( i, 0, 59, 0, width) );
    int y = floor(height * 0.5);
    // https://processing.org/reference/get_.html
    // even though you may have drawn a shape with colorMode(HSB), the numbers returned by get() will be in RGB format.
    color c = get(x, y);
    // https://processing.org/reference/red_.html
    // float r1 = red(c);  // Simpler, but slower to calculate
    // int r2 = c >> 16 & 0xFF;  // Very fast to calculate
    outbound.addByte( c >> 16 & 0xFF );
    // https://processing.org/reference/green_.html
    // float r1 = green(c);  // Simpler, but slower to calculate
    // int r2 = c >> 8 & 0xFF;  // Very fast to calculate
    outbound.addByte( c >> 8 & 0xFF );
    // https://processing.org/reference/blue_.html
    // float b1 = blue(c);   // Simpler, but slower to calculate
    // float b2 = c & 0xFF;  // Very fast to calculate
    outbound.addByte( c & 0xFF );
    
    
  }
  
  outbound.streamPacket(serial);
}