#include <fileioc.h>

#include "state.h"
#include "settings.h"
#include "entities.h"
#include "graphics.h"

#define SAVE_APPVAR   "SNKSAVE"
#define SCORE_APPVAR  "SNKSCORE"

int saveState(struct settings *settings, struct snake *snake, struct point *apple, enum color *color) {
	const settings_size = 5;
	const snake_size    =  ;
	const apple_size       ;
	const color_size    = 1;
	
	uint8_t file = ti_Open(SAVE_APPVAR, "w+");
	if(!file) {
		ti_Close(file);
		return -1;
	}
	
	ti_Seek(0, SEEK_SET, file);
	
	ti_Write(settings, sizeof(struct settings), 1, file);
	ti_Write(snake, sizeof(struct snake), 1, file);
	ti_Write(apple, sizeof(struct point), 1, file);
	ti_Write(color, sizeof(enum color), 1, file);
	
	ti_SetArchiveStatus(true, file);
	ti_Close(file);
	return 1;
}

int loadState(struct settings *settings, struct snake *snake, struct point *apple, enum color *color) {
	uint8_t file = ti_Open(SAVE_APPVAR, "w+");
	if(!file) {
		ti_Close(file);
		return -1;
	}
	
	ti_Seek(0, SEEK_SET, file);
	
	ti_Read(settings, sizeof(struct settings), 1, file);
	ti_Read(snake, sizeof(struct snake), 1, file);
	ti_Read(apple, sizeof(struct point), 1, file);
	ti_Read(color, sizeof(enum color), 1, file);
	
	ti_SetArchiveStatus(true, file);
	ti_Close(file);
	return 1;
}
