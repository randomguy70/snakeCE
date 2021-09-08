#include <fileioc.h>
#include <stdbool.h>
#include <stdint.h>

#include "save.h"

bool saveState(struct settings* settings, uint8_t score) {
	uint8_t file = ti_Open(SAVE_FILE, "w+");
	
	uint16_t checkSum = 0;
	unsigned int encryptedScore;
	uint8_t settingsData[3] = {settings->show_score, settings->size, settings->delay_time};
	
	if(!file) {
		ti_CloseAll();
		file = ti_Open(SAVE_FILE, "w+");
		if(!file) {
			ti_CloseAll();
			return false;
		}
	}
	
	encryptedScore = ((score*26)+15765)*3-4133; /* Yes, I know. So hard to crack. */
	
	settingsData[0] = settings->show_score;
	settingsData[1] = settings->size;
	settingsData[2] = settings->delay_time;
	
	ti_Seek(0, SEEK_SET, file);
	ti_Write(&checkSum, sizeof(checkSum), 1, file);
	ti_Write(&encryptedScore, sizeof(encryptedScore), 1, file);
	ti_Write(settingsData, 3, 1, file);
	ti_SetArchiveStatus(true, file);
	ti_Close(file);
	
	return true;
}

int loadSettings(struct settings *settings) {
	ti_var_t file;
	uint8_t data[3];
	
	file = ti_Open(SAVE_FILE, "r");
	
	if(!file) {
		return -1;
	}
	
	ti_Seek(5, SEEK_SET, file);
	ti_Read(data, 3*sizeof(uint8_t), 1, file);
	ti_SetArchiveStatus(true, file);
	ti_Close(file);
	
	settings->show_score = data[0];
	settings->size = data[1];
	settings->delay_time = data[2];
	
	if(settings->show_score != true && settings->show_score != false) {
		settings->show_score = true;
	}
	if(settings->size != SMALL_SIZE && settings->size != MEDIUM_SIZE  && settings->size != LARGE_SIZE) {
		settings->size = SMALL_SIZE;
	}
	if(settings->delay_time != SLOW_SPEED && settings->delay_time != MEDIUM_SPEED  && settings->delay_time != FAST_SPEED) {
		settings->delay_time = MEDIUM_SPEED;
	}
	
	return 1;
}

int getScore(uint8_t *score) {
	uint8_t file;
	unsigned int encryptedScore;
	
	file = ti_Open(SAVE_FILE, "r");
	if(!file) {
		return -1;
	}
	
	ti_Read(&encryptedScore, sizeof(encryptedScore), 1, file);
	ti_Close(file);
	
	// encrypted score = ((score*26)+15765)*3-4133
	*score = (((encryptedScore + 4133) /3 -15765) / 26);
	
	return 1;	
}

int checkSaveFileAuthenticity(void) {
	uint8_t file = 0;
	uint8_t fileBytes[5] = {0};
	uint16_t realCheckSum = 0;
	uint16_t fileCheckSum = 0;
	
	file = ti_Open(SAVE_FILE, "r");
	
	if(!file) {
		return -1;
	}
	
	ti_Seek(0, SEEK_SET, file);
	ti_Read(&fileCheckSum, 2, 1, file);
	ti_Read(fileBytes, 5, 1, file);
	ti_SetArchiveStatus(true, file);
	ti_Close(file);
	
	for(uint8_t i=0; i<5; i++) {
		realCheckSum += fileBytes[i];
	}
	
	if(realCheckSum == fileCheckSum) {
		return 1;
	}
	
	return 0;
}

void resetSaveFile(void) {
	uint8_t file = ti_Open(SAVE_FILE, "w+");
		
	if(!file) {
		return;
	}
	
	ti_Write((const void *)0xFF0000, 7, 1, file);
	ti_Close(file);
}