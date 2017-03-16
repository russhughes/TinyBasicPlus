
#include <extEEPROM.h>
#include <M24FC1026.h>

 bool M24FC1026::begin() {
	eep = new extEEPROM(kbits_1024, 1, 256, CHIP_ADDRESS);
	if (eep) {
		eep->begin(twiClock400kHz);
		return true;
	}
	else 
		return false;
}
 
 bool M24FC1026::read(uint32_t address, void *buf, size_t nbyte){
	eep->read(address, (byte *)buf, nbyte); 
	return true;
 }

bool M24FC1026::write(uint32_t address, const void *buf, size_t nbyte) {
	eep->write(address, (byte *)buf, nbyte); 
	return true;
}