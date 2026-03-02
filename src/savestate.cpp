
#include "EEPROM.h"
#include "commonvars.h"
#include "savestate.h"

uint8_t calcCRC(void *data, size_t len) {
  uint8_t crc = 0;
  uint8_t *ptr = (uint8_t *)data;
  for (size_t i = 0; i < len; i++) {
    crc ^= ptr[i];
    for (uint8_t j = 0; j < 8; j++) {
      if (crc & 0x80)
        crc = (crc << 1) ^ 0x07;
      else
        crc <<= 1;
    }
  }
  return crc;
}

void loadSavedState(void)
{
    EEPROM.begin(sizeof(SaveData));
    EEPROM.get(0, saveData);
    //needs to be -uint8t size because of crc not included in calculation
    uint8_t crc = calcCRC(&saveData, sizeof(SaveData) - sizeof(uint8_t));
    if (saveData.magic != SAVE_MAGIC || saveData.crc != crc) 
    {
        memset(&saveData, 0, sizeof(SaveData));
        saveData.magic = SAVE_MAGIC;
    }
}

void saveSavedState(void)
{
    saveData.crc = calcCRC(&saveData, sizeof(SaveData) - sizeof(uint8_t));
    EEPROM.begin(sizeof(SaveData));
    EEPROM.put(0, saveData);
    bool ok = EEPROM.commit();
}
