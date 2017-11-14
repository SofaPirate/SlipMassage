#include "SlipMassagePacker.h"

// Internal. SLIP reserved codes.
#define SLIP_MASSAGE_END     0xC0
#define SLIP_MASSAGE_ESC     0xDB
#define SLIP_MASSAGE_ESC_END 0xDC
#define SLIP_MASSAGE_ESC_ESC 0xDD

SlipMassagePacker::SlipMassagePacker(){ 
  _slipEscaping = false; 
}

void SlipMassagePacker::beginPacket(const char* address)
{

  flush();

  _store(SLIP_MASSAGE_END);


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
  _store(SLIP_MASSAGE_END);
}


void SlipMassagePacker::_dataToSlip(void* data, size_t n)
{
  uint8_t* ptr = (uint8_t*)data;
  while (n--)
  {
    uint8_t value = *ptr++;
    switch (value)
    {
      case SLIP_MASSAGE_END:
        _store(SLIP_MASSAGE_ESC);
        _store(SLIP_MASSAGE_ESC_END);
        break;
      case SLIP_MASSAGE_ESC:
        _store(SLIP_MASSAGE_ESC);
        _store(SLIP_MASSAGE_ESC_ESC);
        break;
      default:
        _store(value);
    }
  }
}

