
import java.lang.reflect.*;

class SlipMassageParser {

  final int SLIP_MASSAGE_END  =   0xC0;
  final int SLIP_MASSAGE_ESC  =   0xDB;
  final int SLIP_MASSAGE_ESC_END =  0xDC;
  final int SLIP_MASSAGE_ESC_ESC=  0xDD;

  java.lang.reflect.Method callback;
  Object instance;
  String callbackName = "";

  final int MASSAGE_PARSER_BUFFERSIZE = 256;

  byte[] _buffer = new byte[MASSAGE_PARSER_BUFFERSIZE];
  int _messageSize;
  int _nextIndex;

  String address;

  boolean ready = false;

  boolean _needToFlush = false;

  boolean _parsedAddress, _slipEscaping;


  SlipMassageParser( Object sketch ) {

    this.instance = sketch;
    flush();
  }


  private void flush() {
    _needToFlush = false;
    _messageSize = 0;
    _nextIndex = 0;
    ready = false;
    _parsedAddress = false;
    _slipEscaping = false;
  }


  int nextByte() {

    if ( _hasNext(1) ) {
      int data =  int((_buffer[_nextIndex]));
      _nextIndex++;
      return constrain(data, 0, 255);
    }
    return 0;
  }

  int nextInt() {

    if ( _hasNext(2) ) {
      short data =  (short)( ((_buffer[_nextIndex+1]&0xFF)<<8) | (_buffer[_nextIndex]&0xFF) );
      _nextIndex += 2; 
      return data;
    }
    return 0;
  }

  float nextFloat() {

    if ( _hasNext(4) ) {

      int asInt = (_buffer[_nextIndex] & 0xFF) 
        | ((_buffer[_nextIndex+1] & 0xFF) << 8) 
        | ((_buffer[_nextIndex+2] & 0xFF) << 16) 
        | ((_buffer[_nextIndex+3] & 0xFF) << 24);
      _nextIndex += 4; 
      float data = Float.intBitsToFloat(asInt);

      return data;
    }
    return 0;
  }

  int nextLong() {

    if ( _hasNext(4) ) {
      int data =   ((_buffer[_nextIndex+3]&0xFF)<<24) | ((_buffer[_nextIndex+2]&0xFF)<<16) | ((_buffer[_nextIndex+1]&0xFF)<<8) | (_buffer[_nextIndex]&0xFF) ;
      _nextIndex += 4; 
      return data;
    }
    return 0;
  }


  boolean parse( int streamByte ) {
    
    if ( _needToFlush ) {
      flush();
    }


    // Check if we've reached the end of the buffer.
    if (_messageSize >= MASSAGE_PARSER_BUFFERSIZE) {
      // NOT SURE ABOUT THIS!
      _messageSize = MASSAGE_PARSER_BUFFERSIZE;
      return false;
    }

    // Process byte.
    if ( streamByte == SLIP_MASSAGE_END ) {

      // only process this if we are *not* at beginning
      if (_messageSize > 0) {
        _parsedAddress = false;
        _slipEscaping = false;
        
        _needToFlush = true;

        return true;
      } 
      //flush();
    } else {
      if (  _slipEscaping  ) {
        switch (streamByte) {
        case SLIP_MASSAGE_ESC_END:
          _store(SLIP_MASSAGE_END);
          break;
        case SLIP_MASSAGE_ESC_ESC:
          _store(SLIP_MASSAGE_ESC);
          break;
        }
        _slipEscaping = false;
      } else {
        // NORMAL DATA
        if ( streamByte == SLIP_MASSAGE_ESC ) {
          _slipEscaping  = true;
        } else {
          // ADRRESS END
          if ( _parsedAddress == false && streamByte == 0 ) {
            _parsedAddress = true;
            _store(0);
            _nextIndex = _messageSize;

            byte[] stringBytes =  java.util.Arrays.copyOfRange(_buffer, 0, _messageSize-1);
            address = new String(stringBytes);
            
            //println("PARSER->address: "+address);

            // ANYTHING ELSE
          } else {
            _store(streamByte);
          }
        }
      }
    }

    return false;
  }

  boolean parse( int data, String callbackName) {

    boolean completed =  parse(data);
    if ( completed ) {

      if (this.callbackName.equals(callbackName) == false ) {
        this.callbackName = callbackName;
        callback = findCallback(callbackName);
      }

      try {
        callback.invoke(instance);
      } 
      catch (ReflectiveOperationException e) {
        print("Dropping massage, could not find callback called "+callbackName);
      }
    }

    return completed;
  }

  boolean parseStream(Serial serial) {
    while ( serial.available() > 0 ) {
      if ( parse( serial.read() ) ) {
        return true;
      }
    }
    return false;
  }

  boolean parseStream(Serial serial, String callbackName) {
    while ( serial.available() > 0 ) {
      if ( parse( serial.read(), callbackName ) ) {
        return true;
      }
    }
    return false;
  }

  private boolean _hasNext(int howMuch) {

    if ( _messageSize - _nextIndex  >= howMuch  ) {
      return true;
    } else {
      return false;
    }
  }

  private void _store(int data) {
    _buffer[_messageSize] = (byte)data;
    _messageSize++;
  }

  boolean fullMatch( String s) {
    //println( address +"=="+s + " " + address.equals(s) );
    if ( _hasNext(s.length()) &&  address.equals(s) ) {
      return true;
    }
    return false;
  }



  private Method findCallback(final String name) {
    try {
      return instance.getClass().getMethod(name);
    } 
    catch (Exception e) {
    }
    // Permit callback(Object) as alternative to callback(Serial).
    try {
      return instance.getClass().getMethod(name);
    } 
    catch (Exception e) {
    }
    return null;
  }
}