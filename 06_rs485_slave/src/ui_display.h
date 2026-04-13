#ifndef UI_DISPLAY_H
#define UI_DISPLAY_H

#include <TFT_eSPI.h>

void ui_init();
void ui_update(float temp, float press, bool linkActive);
void ui_drawStaticLayout();

#endif