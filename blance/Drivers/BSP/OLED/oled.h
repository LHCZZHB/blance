#ifndef __OLED_H
#define __OLED_H

#include "../../SYSTEM/sys/sys.h"

void oled_init(void);

void oled_clear(void);
void oled_showchar(uint8_t x,uint8_t y,char byte, uint8_t Fontsize);

void oled_showstring(uint8_t x,uint8_t y,char *string, uint8_t Fontsize);

#endif
