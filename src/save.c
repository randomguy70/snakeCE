#include <fileioc.h>
#include <stdbool.h>
#include <stdint.h>

#include "save.h"

void handleSaveFile(void) {
	ti_var_t file = ti_Open(SAVE_FILE, "r+");
	if(!file) {
		return;
	}
	ti_Resize(7, file);
	ti_SetArchiveStatus(true, file);
	ti_Close(file);
}

void wipeSaveFile(void) {
	ti_var_t file = ti_Open(SAVE_FILE, "w+");
	if(!file) {
		return;
	}
	ti_Seek(0, SEEK_SET, file);
	ti_Write((const void *)0xFF0000, 7, 1, file);
	ti_Resize(7, file);
	ti_SetArchiveStatus(true, file);
	ti_Close(file);
}

uint16_t getHighScore() {
	uint16_t highScore;
	ti_var_t file = ti_Open(SAVE_FILe, "r");
	if(!file) {
		return;
	}
	ti_Seek(2, SEEK_SET, file);
	ti_Read(&highScore, sizeof highScore, 1, file);
	ti_Close(file);
	
	return highScore;
}

uint16_t writeHighScore(uint16_t highScore) {
	ti_var_t file = ti_Open(SAVE_FILe, "r+");
	if(!file) {
		return;
	}
	ti_Seek(2, SEEK_SET, file);
	ti_Write(&highScore, sizeof highScore, 1, file);
	ti_SetArchiveStatus(true, file);
	ti_Close(file);
	
	return highScore;
}
