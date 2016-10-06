#include "BinaryMassenger.h"

// Internal. SLIP reserved codes.
#define BINARY_MASSENGER_SLIP_END     0xC0
#define BINARY_MASSENGER_SLIP_ESC     0xDB
#define BINARY_MASSENGER_SLIP_ESC_END 0xDC
#define BINARY_MASSENGER_SLIP_ESC_ESC 0xDD

BinaryMassenger::BinaryMassenger(Stream* stream)
  : Massenger(stream),
   _slipEscaping(false)
{}

void BinaryMassenger::flush()
{
  Massenger::flush();
  _nextIndex = 0;
  _slipEscaping = false;
}

int8_t BinaryMassenger::nextByte(bool* error) {
  int8_t v;
  _nextBlock((uint8_t*)&v, sizeof(int8_t), error);
  return v;
}

int16_t BinaryMassenger::nextInt(bool* error)
{
  int16_t v;
  _nextBlock((uint8_t*)&v, sizeof(int16_t), error);
  return v;
}

int32_t BinaryMassenger::nextLong(bool* error)
{
  int32_t v;
  _nextBlock((uint8_t*)&v, sizeof(int32_t), error);
  return v;
}

float BinaryMassenger::nextFloat(bool* error)
{
  float v;
  _nextBlock((uint8_t*)&v, sizeof(float), error);
  return v;
}

double BinaryMassenger::nextDouble(bool* error)
{
  double v;
  _nextBlock((uint8_t*)&v, sizeof(double), error);
  return v;
}

void BinaryMassenger::sendBegin(const char* address)
{
  _stream->print(address);
  _stream->write(uint8_t(0)); // null character (end of string)
}

void BinaryMassenger::sendByte(uint8_t value)
{
  _sendSlipData(&value, sizeof(uint8_t));
}

void BinaryMassenger::sendInt(int16_t value)
{
  _sendSlipData(&value, sizeof(uint16_t));
}

void BinaryMassenger::sendLong(int32_t value)
{
  _sendSlipData(&value, sizeof(uint32_t));
}

void BinaryMassenger::sendFloat(float value)
{
  _sendSlipData(&value, sizeof(float));
}

void BinaryMassenger::sendDouble(double value)
{
  _sendSlipData(&value, sizeof(double));
}

void BinaryMassenger::sendEnd()
{
  _stream->write(BINARY_MASSENGER_SLIP_END);
}

bool BinaryMassenger::_process(int streamByte)
{
  byte value = 0;

  // Check if we've reached the end of the buffer.
  if (_messageSize >= MASSENGER_BUFFERSIZE)
  {
    _messageSize = MASSENGER_BUFFERSIZE;
    return true;
  }

  // Process byte.
  _slipEscaping = false; // reset
  switch (streamByte) {
    case BINARY_MASSENGER_SLIP_END:
      if (_messageSize > 0) // only process this if we are *not* at beginning
      {
        // Position _nextIndex after command address string.
        _nextIndex = 0;
        _updateNextIndex();

        return true;
      }
      break;

    case BINARY_MASSENGER_SLIP_ESC:
      _slipEscaping = true;
      break;

    case BINARY_MASSENGER_SLIP_ESC_END:
      _write(_slipEscaping ? BINARY_MASSENGER_SLIP_END : BINARY_MASSENGER_SLIP_ESC_END);
      break;
    case BINARY_MASSENGER_SLIP_ESC_ESC:
      _write(_slipEscaping ? BINARY_MASSENGER_SLIP_ESC : BINARY_MASSENGER_SLIP_ESC_ESC);
      break;

    default:
      _write(streamByte);
  }

  return false;
}

bool BinaryMassenger::_hasNext() const {
  return (_nextIndex < _messageSize);
}

bool BinaryMassenger::_updateNextIndex()
{
  while (_buffer[_nextIndex] != 0)
    _nextIndex++;
  _nextIndex++;
  return (_nextIndex < _messageSize);
}

void BinaryMassenger::_nextBlock(uint8_t* value, size_t n, bool* error)
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

void BinaryMassenger::_sendSlipData(void* data, size_t n)
{
  uint8_t* ptr = (uint8_t*)data;
  while (n--)
  {
    uint8_t value = *ptr++;
    switch (value)
    {
      case BINARY_MASSENGER_SLIP_END:
        _stream->write(BINARY_MASSENGER_SLIP_ESC);
        _stream->write(BINARY_MASSENGER_SLIP_ESC_END);
        break;
      case BINARY_MASSENGER_SLIP_ESC:
        _stream->write(BINARY_MASSENGER_SLIP_ESC);
        _stream->write(BINARY_MASSENGER_SLIP_ESC_ESC);
        break;
      default:
        _stream->write(value);
    }
  }
}
