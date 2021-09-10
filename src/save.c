#include <fileioc.h>
#include <stdbool.h>
#include <stdint.h>

#include "save.h"

bool saveState(struct settings* settings, uint8_t score) {
	uint16_t checkSum = 0;
	uint16_t scoreChecker;
	uint8_t settingsData[3] = {settings->show_score, settings->size, settings->delay_time};
	uint8_t file = ti_Open(SAVE_FILE, "w+");
	
	if(!file) {
		return false;
	}
	
	scoreChecker = score*25+7367;
	
	// write data
	ti_Seek(sizeof checkSum, SEEK_SET, file);
	ti_Write(&score, sizeof score, 1, file);
	ti_Write(&scoreChecker, sizeof scoreChecker, 1, file);
	ti_Write(settingsData, 3, 1, file);
	
	// calculate and write checksum
	checkSum = getCheckSum(SAVE_FILE, sizeof checkSum);
	ti_Seek(0, SEEK_SET, file);
	ti_Write(&checkSum, sizeof checkSum, 1, file);
	
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
	unsigned int encryptedScore;
	
	file = ti_Open(SAVE_FILE, "r");
	if(!file) {
		return -1;
	}
	
	ti_Read(&encryptedScore, sizeof encryptedScore, 1, file);
	ti_Close(file);
	
	// encrypted score = ((score*26)+15765)*3-4133
	highScore = (uint8_t) (((encryptedScore + 4133) /3 -15765) / 26);
	
	return highScore;	
}

int checkSaveFileAuthenticity(void) {
	uint8_t file = 0;
	uint8_t fileBytes[5] = {0};
	uint16_t checkSum = 0;
	
	file = ti_Open(SAVE_FILE, "r");
	
	if(!file) {
		return -1;
	}
	
	ti_Seek(0, SEEK_SET, file);
	ti_Read(&checkSum, sizeof(checkSum), 1, file);
	ti_Close(file);
	if(getCheckSum(SAVE_FILE, sizeof(checkSum)) == checkSum) {
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
