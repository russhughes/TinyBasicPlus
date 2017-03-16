#ifndef M24FC1026_h
#define M24FC1026_h

#include <RamBaseDevice.h>
#include <extEEPROM.h>

#define CHIP_ADDRESS 0x50 // Address of EEPROM chip (24FC1026->0x50)

class M24FC1026 : public RamBaseDevice {
 public:
 
  /** Constructor */
  M24FC1026() {}
  bool begin();
  uint32_t sizeBlocks() {return 256;}
  bool read(uint32_t address, void *buf, size_t nbyte);
  bool write(uint32_t address, const void *buf, size_t nbyte);
  
  private:
	extEEPROM	*eep;
};

#endif // 24FC1026_h
