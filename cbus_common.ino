#include <FlexCAN.h>
#include <kinetis_flexcan.h>

#include "project.h"

// Ensure newly programmed part is in virgin state
unsigned char defaultID = 0x7F;
unsigned char status = 0;
unsigned short nodeID = 0;

#ifdef HAS_EVENTS
	unsigned short ENindex = 0;
	event ENstart[EN_NUM];
	unsigned short EVstart[EN_NUM];
#endif


/*
 * Common CBUS CAN setup
 */
void cbus_setup(void) {
    CANbus.begin();
  
}

/*
 * ee_read_short() - read a short (16 bit) word from EEPROM
 *
 * Data is stored in little endian format
 */
uint16_t ee_read_short(unsigned char addr) {
  unsigned char byte_addr = addr;
  uint16_t ret = EEPROM.read(byte_addr++);
  ret = ret | ((uint16_t)EEPROM.read(byte_addr) << 8);
  return ret;
}

/*
 * ee_write_short() - write a short (16 bit) data to EEPROM
 *
 * Data is stored in little endian format
 */
void ee_write_short(unsigned char addr, uint16_t data) {
  unsigned char byte_addr = addr;
  EEPROM.update(byte_addr++, (unsigned char)data);
  EEPROM.update(byte_addr, (unsigned char)(data>>8));
}

//
// ecan_fifo_empty()
//
// Check if ECAN FIFO has a valid receive buffer available and
// preload the pointer to it
// 
unsigned char ecan_fifo_empty(void) {
    
    if (CANbus.available()) {
      CANbus.read(rx_ptr);
        // current buffer is not empty
        return 0;
    } else {
        return 1;
    }
}


