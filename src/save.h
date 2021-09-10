#ifndef SAVE_H
#define SAVE_H

#include "settings.h"

#define SAVE_FILE "SNKSAVE"

// saves settings and encrypts score
bool saveState(struct settings* settings, uint8_t score);

// load settings and score
int loadSettings(struct settings *settings);

// stores unencrypted score into given pointer
uint8_t getHighScore(void);

// checks the veracity of the save appvar
int checkSaveFileAuthenticity(void);

// wipes the save appvar (obv)
void resetSaveFile(void);

// adds up the bytes of a file starting from an offset and returns the value
uint16_t getCheckSum(const char *name, int seekOffset);

#endif