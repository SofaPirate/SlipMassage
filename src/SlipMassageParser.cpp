#include "SlipMassageParser.h"

// Internal. SLIP reserved codes.
#define BINARY_MASSENGER_SLIP_END     0xC0
#define BINARY_MASSENGER_SLIP_ESC     0xDB
#define BINARY_MASSENGER_SLIP_ESC_END 0xDC
#define BINARY_MASSENGER_SLIP_ESC_ESC 0xDD

SlipMassageParser::SlipMassageParser() { 
  flush();
}

void SlipMassageParser::flush()
{
  MassageParser::flush();
  _parsedAddress = false;
   _slipEscaping = false;
}

int8_t SlipMassageParser::nextByte(bool* error) {
  int8_t v;
  _nextBlock((uint8_t*)&v, sizeof(int8_t), error);
  return v;
}

int16_t SlipMassageParser::nextInt(bool* error)
{
  int16_t v;
  _nextBlock((uint8_t*)&v, sizeof(int16_t), error);
  return v;
}

int32_t SlipMassageParser::nextLong(bool* error)
{
  int32_t v;
  _nextBlock((uint8_t*)&v, sizeof(int32_t), error);
  return v;
}

float SlipMassageParser::nextFloat(bool* error)
{
  float v;
  _nextBlock((uint8_t*)&v, sizeof(float), error);
  return v;
}


bool SlipMassageParser::_decode(int streamByte)
{
 

  // Check if we've reached the end of the buffer.
  if (_messageSize >= MASSAGE_PARSER_BUFFERSIZE) {
    // NOT SURE ABOUT THIS!
    _messageSize = MASSAGE_PARSER_BUFFERSIZE;
    return true;
  }

  // Process byte.
  if ( streamByte == BINARY_MASSENGER_SLIP_END ) {

    // only process this if we are *not* at beginning
     if (_messageSize > 0)   {
     	 _parsedAddress = false;
   		_slipEscaping = false;

      return true;
     } 
     //flush();
      
  } else {
     if (  _slipEscaping  ) {
       switch (streamByte) {
          case BINARY_MASSENGER_SLIP_ESC_END:
            _store(BINARY_MASSENGER_SLIP_END);
            break;
          case BINARY_MASSENGER_SLIP_ESC_ESC:
            _store(BINARY_MASSENGER_SLIP_ESC);
            break;
       }
        _slipEscaping = false;
    } else {
       // NORMAL DATA
        if ( streamByte == BINARY_MASSENGER_SLIP_ESC ) {
          _slipEscaping  = true;
        } else {
          // ADRRESS END
          if ( _parsedAddress == false && streamByte == 0 ) {
            _parsedAddress = true;
            _store(0);
            _nextIndex = _messageSize;
           
            // ANYTHING ELSE
          }  else {
             _store(streamByte);
          }
         // Serial.write(255);
          // Serial.write(streamByte);
          // Serial.write(_parsedAddress); 
          // Serial.write(_nextIndex);         
        }
    }
  }

  return false;
}

bool SlipMassageParser::_hasNext() const {
	
  return (_nextIndex < _messageSize);
}

/*
bool SlipMassageParser::_updateNextIndex()
{
  while (_buffer[_nextIndex] != 0)
    _nextIndex++;
  _nextIndex++;
  return (_nextIndex < _messageSize);
}
*/
void SlipMassageParser::_nextBlock(uint8_t* value, size_t n, bool* error)
{
  // Check for errors.
  bool err = !_hasNext();
  if (err)
    memset(value, 0, n); // set to zero (default)

  if (error)
    *error = err;

  // Process data.
  if (!err)
  {
    memcpy(value, &_buffer[_nextIndex], n);
    _nextIndex += n;
  }
}
