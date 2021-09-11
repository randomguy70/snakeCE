#include <fileioc.h>
#include <stdbool.h>
#include <stdint.h>

#include "save.h"

int saveSettings(struct settings* settings) {
	uint8_t settingsData[3] = {settings->show_score, settings->size, settings->delay_time};
	ti_var_t file = ti_Open(SAVE_FILE, "r+");
	
	if(!file) {
		return false;
	}
		
	// write data
	ti_Seek(5, SEEK_SET, file);
	ti_Write(settingsData, 3, 1, file);
	
	ti_Resize(7, file);
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

uint8_t getHighScore(void) {
	uint8_t file;
	uint8_t highScore;
	
	file = ti_Open(SAVE_FILE, "r");
	if(!file) {
		return -1;
	}
	// checker == score*57+7363
	ti_Seek(2, SEEK_SET, file);
	ti_Read(&highScore, sizeof highScore, 1, file);
	ti_SetArchiveStatus(true, file);
	ti_Close(file);
	
	return highScore;
}

uint8_t writeHighScore(uint8_t highScore) {
	uint16_t scoreBuffer = highScore;
	uint16_t scoreChecker = scoreBuffer*57+7363;
	
	ti_var_t file = ti_Open(SAVE_FILE, "r+");
	if(!file) {
		return 0;
	}
	ti_Seek(2, SEEK_SET, file);
	ti_Write(&highScore, sizeof highScore, 1, file);
	ti_Seek(3, SEEK_SET, file);
	ti_Write(&scoreChecker, sizeof scoreChecker, 1, file);
	ti_SetArchiveStatus(true, file);
	ti_Close(file);
	return highScore;
}

int checkSaveFileAuthenticity(void) {
	ti_var_t file;
	uint16_t checkSum = 0;
	
	file = ti_Open(SAVE_FILE, "r");
	
	if(!file) {
		return -1;
	}
	
	ti_Seek(0, SEEK_SET, file);
	ti_Read(&checkSum, sizeof checkSum, 1, file);
	ti_Close(file);
	if(getCheckSum(SAVE_FILE, sizeof checkSum) == checkSum) {
		return 1;
	}
	
	return 0;
}

void resetSaveFile(void) {
	uint8_t file = ti_Open(SAVE_FILE, "w+");
		
	if(!file) {
		return;
	}
	
	ti_Resize(7, file);
	ti_Write((const void *)0xFF0000, 7, 1, file);
	ti_Close(file);
}

uint16_t getCheckSum(const char *name, int seekOffset) {
	uint16_t checkSum = 0;
	uint8_t *dataPtr = NULL;
	int fileSize = 0;
	ti_var_t file = ti_Open(name, "r");
	if(!file)
		return 0;
	
	fileSize = ti_GetSize(file)-seekOffset;
	ti_Seek(seekOffset, SEEK_SET, file);
	dataPtr = ti_GetDataPtr(file);
	for(int i=0; i<fileSize; i++) {
		checkSum += dataPtr[i];
	}
	
	ti_Close(file);
	return checkSum;
}

uint16_t writeCheckSum(const char *name, int seekOffset, uint16_t checkSum) {
	ti_var_t file;
	
	file = ti_Open(name, "r+");
	if(!file) {
		return 0;
	}
	
	ti_Seek(0, SEEK_SET, file);
	ti_Write(&checkSum, sizeof checkSum, 1, file);
	ti_Close(file);
	return checkSum;
}
