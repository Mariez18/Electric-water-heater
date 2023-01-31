#define EEPROM_H_

void EEPROM_write(const short address,  char data);
  char EEPROM_read(const short address);