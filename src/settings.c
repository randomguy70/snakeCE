#include <fileioc.h>
#include <stdbool.h>

#include "settings.h"
#include "main.h"

int saveSettings(struct settings *settings) {
	uint8_t file;
	const uint8_t sizeOfData = 10;
	char data[sizeOfData] = {0};
	
	data[0] = settings->show_score;
	data[1] = settings->auto_save;
	data[2] = settings->auto_restore;
	data[3] = settings->size;
	data[4] = settings->speed;
	
	file = ti_Open(SAVE_APPVAR, "w+");
	if(!file) {
		ti_Close(file);
		return -1;
	}
	ti_Write(data, sizeOfData, 1, file);
	ti_SetArchiveStatus(true, file);
	ti_Close(file);
	return 1;
}

int readSettings(struct settings *settings) {
	uint8_t file;
	const uint8_t sizeOfData = 10;
	char data[sizeOfData] = {0};
	
	file = ti_Open(SAVE_APPVAR, "r");
	if(!file) {
		ti_Close(file);
		file = ti_Open(SAVE_APPVAR, "w+");
		ti_Close(file);
		return -1;
	}
	ti_Read(data, sizeOfData, 1, file);
	ti_SetArchiveStatus(true, file);
	ti_Close(file);
	return 1;
}
