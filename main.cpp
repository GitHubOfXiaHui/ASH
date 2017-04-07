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
		if (msg.is_left() && msg.is_up() && onCloseButton(msg.x, msg.y)) {
			extern bool closed;
			closed = true;
			printf("µÈ´ýÏÂ°à...\n");
			break;
		}
		cleardevice();
		printScreen();
	}
	extern bool isLives[];
	bool flag = true;
	while (flag) {
		for (int i = 0; i < CHECKPOINT_NUMBER; i++) {
			if (isLives[i]) {
				flag = true;
				break;
			}
			flag = false;
		}
		delay_fps(60);
		cleardevice();
		printScreen();
	}
	closegraph();
	return 0;
}
