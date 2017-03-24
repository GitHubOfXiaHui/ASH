#include <stdio.h>
#include <graphics.h>
#include <windows.h>

#include "queue.h"
#include "global.h"
#include "action.h"
#include "layout.h"

int main() {
	init();
	initgraph(SCREEN_WIDTH, SCREEN_HEIGHT);
	mouse_msg msg = {0};
	for (; is_run(); delay_fps(60)) {
		msg = getmouse();
		if (msg.is_left() && msg.is_up()) {
			if (onPassengerButton(msg.x, msg.y)) {
				clickPassenger(); 
			} else if (onVipButton(msg.x, msg.y)) {
				clickVip();
			} else if (onCloseButton(msg.x, msg.y)) {
				extern bool closed;
				closed = true;
				break;
			}
			const int pause = onPauseButtons(msg.x, msg.y);
			if (pause) {
				clickPause(pause - 1);
			}
			const int reset = onResetButtons(msg.x, msg.y);
			if (reset) {
				clickReset(reset - 1);
			}
		}
		cleardevice();
		printScreen();
	}
	extern bool isLives[];
	bool flag = true;
	while (flag) {
		for (int i = 0; i < VIP_CHECKPOINT_NUMBER + CHECKPOINT_NUMBER; i++) {
			if (isLives[i]) {
				flag = true;
				break;
			}
			flag = false;
		}
	}
	closegraph();
	return 0;
}
