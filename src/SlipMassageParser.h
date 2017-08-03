#ifndef SlipMassageParser_h
#define SlipMassageParser_h

#include "Massage/MassageParser.h"

/// Main SlipMassageParser class.
class SlipMassageParser : public MassageParser
{
public:
  /// Constructor.
  SlipMassageParser();

  /// Flushes current message in buffer (if any).
  virtual void flush();

  /// Reads next byte.
  virtual int8_t nextByte(bool* error=0);

  /// Reads next int.
  virtual int16_t nextInt(bool* error=0);

  /// Reads next long.
  virtual int32_t nextLong(bool* error=0);

  /// Reads next float.
  virtual float nextFloat(bool* error=0);


protected:
  /// Processes a single value read from the serial stream.
  virtual bool _decode(int serialByte);

private:
  // Moves nextIndex to the next token.
 // bool _updateNextIndex();

  // Returns true iff it is still possible to call next*().
  bool _hasNext() const;

  // Helper function to read next block of data.
  void _nextBlock(uint8_t* value, size_t n, bool* error);

  // Index in the buffer of next argument to read.
  uint8_t _nextIndex;

  bool _slipEscaping;

  bool _parsedAddress;


};

#endif
