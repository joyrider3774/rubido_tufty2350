#ifndef SAVEDSTATE_H
#define SAVEDSTATE_H

#define SAVE_MAGIC 0xDCDC

#pragma pack(push, 1)
struct SaveData {
    uint16_t magic;  // always first
    int BestPegsLeft[4]; // array that holds the best amount of pegs left for each difficulty
    uint8_t crc;
};
#pragma pack(pop)

void loadSavedState(void);
void saveSavedState(void);

#endif