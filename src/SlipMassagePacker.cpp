#include "SlipMassagePacker.h"

// Internal. SLIP reserved codes.
#define BINARY_MASSENGER_SLIP_END     0xC0
#define BINARY_MASSENGER_SLIP_ESC     0xDB
#define BINARY_MASSENGER_SLIP_ESC_END 0xDC
#define BINARY_MASSENGER_SLIP_ESC_ESC 0xDD

SlipMassagePacker::SlipMassagePacker(){ 
  _slipEscaping = false; 
}

void SlipMassagePacker::beginPacket(const char* address)
{

  flush();

  _store(BINARY_MASSENGER_SLIP_END);


 const char* ptr = address;

  while (*ptr != '\0') {
    _store(*ptr);
    ptr++;
  }
      
  _store(0); // null character (end of string)


}

void SlipMassagePacker::addByte(uint8_t value)
{
  _dataToSlip(&value, sizeof(uint8_t));
}

void SlipMassagePacker::addInt(int16_t value)
{
  _dataToSlip(&value, sizeof(uint16_t));
}

void SlipMassagePacker::addLong(int32_t value)
{
  _dataToSlip(&value, sizeof(uint32_t));
}

void SlipMassagePacker::addFloat(float value)
{
  _dataToSlip(&value, sizeof(float));
}


void SlipMassagePacker::endPacket()
{
  _store(BINARY_MASSENGER_SLIP_END);
}


void SlipMassagePacker::_dataToSlip(void* data, size_t n)
{
  uint8_t* ptr = (uint8_t*)data;
  while (n--)
  {
    uint8_t value = *ptr++;
    switch (value)
    {
      case BINARY_MASSENGER_SLIP_END:
        _store(BINARY_MASSENGER_SLIP_ESC);
        _store(BINARY_MASSENGER_SLIP_ESC_END);
        break;
      case BINARY_MASSENGER_SLIP_ESC:
        _store(BINARY_MASSENGER_SLIP_ESC);
        _store(BINARY_MASSENGER_SLIP_ESC_ESC);
        break;
      default:
        _store(value);
    }
  }
}

