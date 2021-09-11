#ifndef SAVE_H
#define SAVE_H

#include "settings.h"

#define SAVE_FILE "SNKSAVE"

#define SCORE_OFFSET         0
#define SCORE_SIZE           2
#define SCORE_CHECK_OFFSET   2
#define SCORE_CHECK_SIZE     2
#define SETTINGS_DATA_OFFSET 4
#define SETTINGS_DATA_SIZE   3

void handleSaveFile(void);
void wipeSaveFile(void);

uint16_t getHighScore(void);
uint16_t writeHighScore(uint16_t highScore);
int getHighScoreVeracity(void);

void readSettings(struct settings *settings);
void writeSettings(struct settings *settings);

#endif