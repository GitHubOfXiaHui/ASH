#ifndef _LAYOUT_H_
#define _LAYOUT_H_

// ÆÁÄ»´óÐ¡ 
#define SCREEN_WIDTH 720
#define SCREEN_HEIGHT 560

void printScreen();

bool onVipButton(const int x, const int y);
bool onPassengerButton(const int x, const int y);
bool onCloseButton(const int x, const int y);

int onPauseButtons(const int x, const int y);
int onResetButtons(const int x, const int y);

#endif
