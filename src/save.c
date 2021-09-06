#include <fileioc.h>
#include <stdbool.h>
#include <stdint.h>

#include "save.h"

bool saveState(struct settings* settings, uint8_t score) {
	uint8_t file = ti_Open(SAVE_APPVAR, "w+");
	
	uint16_t checkSum = 0;
	unsigned int encryptedScore;
	uint8_t settingsData[3] = {0};
	
	if(!file) {
		ti_CloseAll();
		file = ti_Open(SAVE_APPVAR, "w+");
		if(!file) {
			ti_CloseAll();
			return false;
		}
	}
	
	settingsData[0] = settings->show_score;
	settingsData[1] = settings->size;
	settingsData[2] = settings->speed;
	
	ti_Seek(0, SEEK_SET, file);
	
	ti_Close(file);
}