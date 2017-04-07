#include <stdio.h>
#include <graphics.h>

#include "layout.h"
#include "global.h"
#include "queue.h"

// 颜色值 
#define WHITE EGERGB(0xFF, 0xFF, 0xFF)
#define BLUE EGERGB(0x33, 0x55, 0xD0)
#define RED EGERGB(0xD5, 0x5D, 0x5C)
#define GRAY EGERGB(0xC0, 0xC0, 0xC4)
#define BLACK EGERGB(0x00, 0x00, 0x00)

// 字体大小
#define CHANNEL_FONT_SIZE 18
#define BUTTON_FONT_SIZE 16
#define PAUSE_FONT_SIZE 14

// 候机通道 
#define CHANNEL_LEFT 10
#define CHANNEL_TOP 10
#define CHANNEL_X_OFFSET 145
#define CHANNEL_Y_OFFSET 15
#define CHANNEL_WIDTH 400
#define CHANNEL_HEIGHT 50
#define CHANNEL_V 10

// 下班按钮 
#define CLOSE_BUTTON_WIDTH 100
#define CLOSE_BUTTON_HEIGHT 30
#define CLOSE_BUTTON_LEFT 10
#define CLOSE_BUTTON_TOP (SCREEN_HEIGHT - CLOSE_BUTTON_HEIGHT - 10)
#define CLOSE_BUTTON_X_OFFSET 32
#define CLOSE_BUTTON_Y_OFFSET 7

// 安检口 
#define CHECKPOINT_V 60
#define CHECKPOINT_ORIGIN_X 500
#define CHECKPOINT_ORIGIN_Y 10
#define CHECKPOINT_WIDTH 135
#define CHECKPOINT_HEIGHT 30
#define CHECKPOINT_X_OFFSET 10

// 暂停标志
#define PAUSE_X (CHECKPOINT_ORIGIN_X + CHECKPOINT_WIDTH + 20)
#define PAUSE_Y (CHECKPOINT_ORIGIN_Y + 5)

// 乘客
#define PASSENGER_R 12.5
#define PASSENGER_X (CHECKPOINT_ORIGIN_X + CHECKPOINT_WIDTH - CHECKPOINT_X_OFFSET - PASSENGER_R)
#define PASSENGER_Y (CHECKPOINT_ORIGIN_Y + 15)

struct Rect
{
	int left;
	int top;
	int right;
	int bottom;
};
typedef struct Rect Rect;

struct Point
{
	int x;
	int y;
};
typedef struct Point Point;

void printChannel();
void printCloseButton();
void printCheckpoints();
void print(const int fontSize, const color_t foreColor, const color_t backColor, const Rect * rect, const char * title, const Point * point);
void printPauses();
void printPassengers();

void printScreen() {
	printChannel();
	printCloseButton();
	printCheckpoints();
	printPauses();
	printPassengers();
}

void printChannel() {
	Rect rect = {CHANNEL_LEFT, CHANNEL_TOP, CHANNEL_LEFT + CHANNEL_WIDTH, CHANNEL_TOP + CHANNEL_HEIGHT};
	const Point point = {CHANNEL_X_OFFSET, CHANNEL_Y_OFFSET};
	char title[20];
	extern Channel channel;
	extern HANDLE channelLock;
	WaitForSingleObject(channelLock, INFINITE);
	sprintf(title, "候机通道(%d/%d)", channel.people, channel.capacity);
	print(CHANNEL_FONT_SIZE, BLUE, WHITE, &rect, title, &point);
	extern int channelCurrentNum;
	for (int i = 1; i < channelCurrentNum; i++) {
		rect.top += CHANNEL_V + CHANNEL_HEIGHT;
		rect.bottom += CHANNEL_V + CHANNEL_HEIGHT;
		print(CHANNEL_FONT_SIZE, BLUE, WHITE, &rect, "候机通道", &point);
	}
	ReleaseMutex(channelLock);
}

void printCloseButton() {
	const Rect rect = {CLOSE_BUTTON_LEFT, CLOSE_BUTTON_TOP, CLOSE_BUTTON_LEFT + CLOSE_BUTTON_WIDTH, CLOSE_BUTTON_TOP + CLOSE_BUTTON_HEIGHT};
	const Point point = {CLOSE_BUTTON_X_OFFSET, CLOSE_BUTTON_Y_OFFSET};
	extern bool closed;
	const color_t bgcolor = closed ? GRAY : RED;
	print(BUTTON_FONT_SIZE, WHITE, bgcolor, &rect, "下班", &point);
}

void printCheckpoint(const int number);

void printCheckpoints() {
	setcolor(BLUE);
	for (int i = 0; i < CHECKPOINT_NUMBER; i++) {
		printCheckpoint(i);
	}
}

void printCheckpoint(const int number) {
	int y = CHECKPOINT_ORIGIN_Y + number * CHECKPOINT_V;
	moveto(CHECKPOINT_ORIGIN_X, y);
	linerel(CHECKPOINT_WIDTH, 0);
	moveto(CHECKPOINT_ORIGIN_X, y + CHECKPOINT_HEIGHT);
	linerel(CHECKPOINT_WIDTH, 0);
	moveto(CHECKPOINT_ORIGIN_X + CHECKPOINT_WIDTH - CHECKPOINT_X_OFFSET, y);
	linerel(0, CHECKPOINT_HEIGHT);
}

void print(const int fontSize, const color_t foreColor, const color_t backColor, const Rect * rect, const char * title, const Point * point) {
	setfont(fontSize, 0, "宋体");
	setcolor(foreColor);
	setfillcolor(backColor);
	setfontbkcolor(getfillcolor());
	bar(rect->left, rect->top, rect->right, rect->bottom);
	rectangle(rect->left, rect->top, rect->right, rect->bottom);
	xyprintf(rect->left + point->x, rect->top + point->y, title);
}

void printPause(const int number);

void printPauses() {
	extern bool pauses[];
	for (int i = 0; i < CHECKPOINT_NUMBER; i++) {
		if (pauses[i]) {
			printPause(i);
		}
	}
}

void printPause(const int number) {
	setfont(PAUSE_FONT_SIZE, 0, "宋体");
	setcolor(BLUE);
	setfontbkcolor(BLACK);
	int y = PAUSE_Y + number * CHECKPOINT_V;
	xyprintf(PAUSE_X, y, "pause");
}

void printPassengers() {
	extern Checkpoint checkpoints[];
	setcolor(WHITE);
	setfillcolor(getcolor());
	for (int i = 0; i < CHECKPOINT_NUMBER; i++) {
		for (int j = 0; j < checkpoints[i].people; j++) {
			fillellipsef(PASSENGER_X - j * 2 * PASSENGER_R, PASSENGER_Y + i * CHECKPOINT_V, PASSENGER_R, PASSENGER_R);
		}
	}
}

bool onCloseButton(const int x, const int y) {
	return (CLOSE_BUTTON_LEFT < x && x < CLOSE_BUTTON_LEFT + CLOSE_BUTTON_WIDTH) && (CLOSE_BUTTON_TOP < y && y < CLOSE_BUTTON_TOP + CLOSE_BUTTON_HEIGHT);
}
