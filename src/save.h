#ifndef SAVE_H
#define SAVE_H

#include "settings.h"

#define SAVE_FILE "SNKSAVE"

int saveSettings(struct settings* settings);
int loadSettings(struct settings *settings);

uint8_t getHighScore(void);
uint8_t writeHighScore(uint8_t highScore);

int checkSaveFileAuthenticity(void);

/* adds up the bytes of a file starting from an offset and returns the value */
uint16_t getCheckSum(const char *name, int seekOffset);
uint16_t writeCheckSum(const char *name, int seekOffset, uint16_t checkSum);

void resetSaveFile(void);

#endif