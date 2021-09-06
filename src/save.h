#ifndef SAVE_H
#define SAVE_H

#include "settings.h"

#define SAVE_FILE "SNKSAVE"

// saves settings and encrypts score
bool saveState(struct settings* settings, uint8_t score);

// load settings and score
int loadSettings(struct settings *settings);

// stores unencrypted score into given pointer
int getScore(uint8_t *score);

// checks the veracity of the save appvar
int checkSaveFileAuthenticity(void);

// wipes the save appvar (obv)
void resetSaveFile(void);

#endif