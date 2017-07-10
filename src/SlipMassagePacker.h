#ifndef SlipMassagePacker_h
#define SlipMassagePacker_h

#include "Massage/MassagePacker.h"

/// Main SlipMassagePacker class.
class SlipMassagePacker : public MassagePacker {

public:
  /// Constructor.
  SlipMassagePacker();


  /// Begins the sending of a message.
  virtual void beginPacket(const char* address);

  /// Sends a byte.
  virtual void addByte(uint8_t value);


  /// Sends an int.
  virtual void addInt(int16_t value);


  /// Sends a long.
  virtual void addLong(int32_t value);


  /// Sends a float.
  virtual void addFloat(float value);


  /// Ends the sending of a message.
  virtual void endPacket();




private:

  bool _slipEscaping;


  void _dataToSlip(void* data, size_t n);


};

#endif
