class SlipMassagePacker {

  final int SLIP_MASSAGE_END  =   0xC0;
  final int SLIP_MASSAGE_ESC  =   0xDB;
  final int SLIP_MASSAGE_ESC_END =  0xDC;
  final int SLIP_MASSAGE_ESC_ESC=  0xDD;

  int _messageSize;
  byte[] _buffer;
  boolean ended = false;
  
  final int MASSAGE_PACKER_BUFFERSIZE = 256;

  SlipMassagePacker() {
   _buffer = new byte[MASSAGE_PACKER_BUFFERSIZE];
  }

  /// Begins the sending of a message.
  void beginPacket(String address) {
    _messageSize = 0;
    ended = false;

    _write(SLIP_MASSAGE_END);

    byte[] b = address.getBytes();
    for ( int i =0; i < b.length; i++ ) {
      _dataToSlip(b[i]);
    }

    _write(0);
  }

  /// Adds a byte.
  void addByte(int value) {
    value = constrain(value,0,255);
    _dataToSlip(value & 0xFF);
  }

  /// Adds an int.
  void addInt(int value) {
    value = (short) value;
    _dataToSlip(value & 0xFF);
    _dataToSlip((value >> 8) & 0xFF);
  }

  /// Adds a long.
  void addLong(int value) {
    _dataToSlip(value & 0xFF);
    _dataToSlip((value >> 8) & 0xFF);
    _dataToSlip((value >> 16) & 0xFF);
    _dataToSlip((value >> 24) & 0xFF);
  }

  /// Adds a float.
  void addFloat(float value) {
    int bits = Float.floatToIntBits(value);
    _dataToSlip((bits & 0xff));
    _dataToSlip(((bits >> 8) & 0xff));
    _dataToSlip(((bits >> 16) & 0xff));
    _dataToSlip(((bits >> 24) & 0xff));
  }

  /// Ends the sending of a message.
  void endPacket() {
    _write(SLIP_MASSAGE_END);
    ended = true;
  }

  void _dataToSlip(int value)
  {

    switch (value)
    {
    case SLIP_MASSAGE_END:
      _write(SLIP_MASSAGE_ESC);
      _write(SLIP_MASSAGE_ESC_END);
      break;
    case SLIP_MASSAGE_ESC:
      _write(SLIP_MASSAGE_ESC);
      _write(SLIP_MASSAGE_ESC_ESC);
      break;
    default:
      _write(value);
    }
  }

  void _write(int data) {
    _buffer[_messageSize] = (byte)data;
    _messageSize++;
  }


  /// Returns size of buffer.
  int size() {
    return _messageSize;
  }

  // Returns a reference to the buffer.
  byte[] buffer() {
    if ( !ended ) println("Warning: getting the buffer of an unended packet!");
    return java.util.Arrays.copyOfRange(_buffer, 0, _messageSize);
  }


  void streamPacket(Serial serial) {
    endPacket();
    serial.write( buffer() );
    /*
    print("PACKER->buffer()");
    byte[] debugBuffer = buffer();
    for (int i =0; i < debugBuffer.length ; i++) {
      println("* "+debugBuffer[i]);
    }
    println();
    */
  }

  /// Create a packet with no arguments.
  void packEmpty(String address)
  {
    beginPacket(address);
    endPacket();
  }

  /// Create a packet with a single byte value.
  void packOneByte(String address, byte value)
  {
    beginPacket(address);
    addByte(value);
    endPacket();
  }

  /// Create a packet with a single int value.
  void packOneInt(String address, int value)
  {
    beginPacket(address);
    addInt(value);
    endPacket();
  }

  /// Create a packet with a single long value.
  void packOneLong(String address, int value)
  {
    beginPacket(address);
    addLong(value);
    endPacket();
  }

  /// Create a packet with a single float value.
  void packOneFloat(String address, float value)
  {
    beginPacket(address);
    addFloat(value);
    endPacket();
  }

  /// Create a streamet with no arguments.
  void streamEmpty(Serial serial, String address)
  {
    beginPacket(address);
    streamPacket(serial);
  }

  /// Create a streamet with a single byte value.
  void streamOneByte(Serial serial, String address, byte value)
  {
    beginPacket(address);
    addByte(value);
    streamPacket(serial);
  }

  /// Create a streamet with a single int value.
  void streamOneInt(Serial serial, String address, int value)
  {
    beginPacket(address);
    addInt(value);
    streamPacket(serial);
  }

  /// Create a streamet with a single long value.
  void streamOneLong(Serial serial, String address, int value)
  {
    beginPacket(address);
    addLong(value);
    streamPacket(serial);
  }

  /// Create a streamet with a single float value.
  void streamOneFloat(Serial serial, String address, float value)
  {
    beginPacket(address);
    addFloat(value);
    streamPacket(serial);
  }
}