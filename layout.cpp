#include <stdio.h>
#include <graphics.h>

#include "layout.h"
#include "global.h"
#include "queue.h"

// 颜色值 
#define WHITE EGERGB(0xFF, 0xFF, 0xFF)
#define GREEN EGERGB(0x1A, 0xA2, 0x60)
#define BLUE EGERGB(0x33, 0x55, 0xD0)
#define RED EGERGB(0xD5, 0x5D, 0x5C)
#define BLACK EGERGB(0x00, 0x00, 0x00)
#define RANDOM EGERGB(random(256), random(256), random(256))

// 字体大小
#define CHANNEL_FONT_SIZE 18
#define BUTTON_FONT_SIZE 16
#define PAUSE_FONT_SIZE 14 

// 候机通道 
#define VIP_CHANNEL_LEFT 10
#define VIP_CHANNEL_TOP 60
#define VIP_CHANNEL_X_OFFSET 120
#define VIP_CHANNEL_Y_OFFSET 15

#define CHANNEL_LEFT VIP_CHANNEL_LEFT
#define CHANNEL_TOP 320
#define CHANNEL_X_OFFSET 145
#define CHANNEL_Y_OFFSET VIP_CHANNEL_Y_OFFSET

#define CHANNEL_WIDTH 400
#define CHANNEL_HEIGHT 50

// 乘客按钮 
#define SPAN 20

#define BUTTON_WIDTH 100
#define BUTTON_HEIGHT 30

#define VIP_BUTTON_LEFT 10
#define VIP_BUTTON_TOP (SCREEN_HEIGHT - BUTTON_HEIGHT - 10)
#define VIP_BUTTON_X_OFFSET 20
#define VIP_BUTTON_Y_OFFSET 7

#define BUTTON_LEFT (VIP_BUTTON_LEFT + BUTTON_WIDTH + SPAN)
#define BUTTON_TOP VIP_BUTTON_TOP
#define BUTTON_X_OFFSET 20
#define BUTTON_Y_OFFSET VIP_BUTTON_Y_OFFSET

#define CLOSE_BUTTON_LEFT (BUTTON_LEFT + BUTTON_WIDTH + SPAN)
#define CLOSE_BUTTON_TOP VIP_BUTTON_TOP
#define CLOSE_BUTTON_X_OFFSET 32
#define CLOSE_BUTTON_Y_OFFSET VIP_BUTTON_Y_OFFSET

// 暂停/回复按钮 
#define SPAN_H 20
#define SPAN_V 10

#define PAUSE_WIDTH 50
#define PAUSE_HEIGHT 20

#define CHECKPOINT_V 80

#define PAUSE_ORIGIN_X 500
#define PAUSE_ORIGIN_Y 10

#define PAUSE_X_OFFSET 12
#define PAUSE_Y_OFFSET 3

#define CHECKPOINT_ORIGIN_X PAUSE_ORIGIN_X
#define CHECKPOINT_ORIGIN_Y (PAUSE_ORIGIN_Y + PAUSE_HEIGHT + SPAN_V)

#define CHECKPOINT_WIDTH 135
#define CHECKPOINT_HEIGHT 30
#define CHECKPOINT_X_OFFSET 10

// 暂停标志
#define PAUSE_X (PAUSE_ORIGIN_X + CHECKPOINT_WIDTH + 20)
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

void printVipChannel();
void printChannel();
void printVipButton(); 
void printButton();
void printCloseButton();
void printCheckpoints();
void print(const int fontSize, const color_t foreColor, const color_t backColor, const Rect * rect, const char * title, const Point * point);
void printPauses();
void printPassengers();

void printScreen() {
	printVipChannel();
	printChannel();
	printVipButton();
	printButton();
	printCloseButton();
	printCheckpoints();
	printPauses();
	printPassengers();
}

void printVipChannel() {
	const Rect rect = {VIP_CHANNEL_LEFT, VIP_CHANNEL_TOP, VIP_CHANNEL_LEFT + CHANNEL_WIDTH, VIP_CHANNEL_TOP + CHANNEL_HEIGHT};
	const Point point = {VIP_CHANNEL_X_OFFSET, VIP_CHANNEL_Y_OFFSET};
	char title[20];
	extern Channel vipChannel;
	extern HANDLE vipChannelLock;
	WaitForSingleObject(vipChannelLock, INFINITE);
	sprintf(title, "VIP候机通道(%d/%d)", vipChannel.people, vipChannel.capacity);
	ReleaseMutex(vipChannelLock);
	print(CHANNEL_FONT_SIZE, GREEN, WHITE, &rect, title, &point);
}

void printChannel() {
	const Rect rect = {CHANNEL_LEFT, CHANNEL_TOP, CHANNEL_LEFT + CHANNEL_WIDTH, CHANNEL_TOP + CHANNEL_HEIGHT};
	const Point point = {CHANNEL_X_OFFSET, CHANNEL_Y_OFFSET};
	char title[20];
	extern Channel channel;
	extern HANDLE channelLock;
	WaitForSingleObject(channelLock, INFINITE);
	sprintf(title, "候机通道(%d/%d)", channel.people, channel.capacity);
	ReleaseMutex(channelLock);
	print(CHANNEL_FONT_SIZE, BLUE, WHITE, &rect, title, &point);
}

void printVipButton() {
	const Rect rect = {VIP_BUTTON_LEFT, VIP_BUTTON_TOP, VIP_BUTTON_LEFT + BUTTON_WIDTH, VIP_BUTTON_TOP + BUTTON_HEIGHT};
	const Point point = {VIP_BUTTON_X_OFFSET, VIP_BUTTON_Y_OFFSET};
	print(BUTTON_FONT_SIZE, WHITE, GREEN, &rect, "VIP乘客", &point);
}

void printButton() {
	const Rect rect = {BUTTON_LEFT, BUTTON_TOP, BUTTON_LEFT + BUTTON_WIDTH, BUTTON_TOP + BUTTON_HEIGHT};
	const Point point = {BUTTON_X_OFFSET, BUTTON_Y_OFFSET};
	print(BUTTON_FONT_SIZE, WHITE, BLUE, &rect, "普通乘客", &point);	
}

void printCloseButton() {
	const Rect rect = {CLOSE_BUTTON_LEFT, CLOSE_BUTTON_TOP, CLOSE_BUTTON_LEFT + BUTTON_WIDTH, CLOSE_BUTTON_TOP + BUTTON_HEIGHT};
	const Point point = {CLOSE_BUTTON_X_OFFSET, CLOSE_BUTTON_Y_OFFSET};
	print(BUTTON_FONT_SIZE, WHITE, RED, &rect, "下班", &point);
}

void printCheckpoint(const int number);

void printCheckpoints() {
	for (int i = 0; i < VIP_CHECKPOINT_NUMBER + CHECKPOINT_NUMBER; i++) {
		printCheckpoint(i);
	}
}

void printCheckpoint(const int number) {
	const int top = PAUSE_ORIGIN_Y + number * CHECKPOINT_V;
	Rect rect = {PAUSE_ORIGIN_X, top, PAUSE_ORIGIN_X + PAUSE_WIDTH, top + PAUSE_HEIGHT};
	const Point point = {PAUSE_X_OFFSET, PAUSE_Y_OFFSET};
	const color_t foreColor = number < VIP_CHECKPOINT_NUMBER ? GREEN : BLUE;
	print(PAUSE_FONT_SIZE, foreColor, WHITE, &rect, "暂停", &point);
	
	rect.left += PAUSE_WIDTH + SPAN_H;
	rect.right += PAUSE_WIDTH + SPAN_H;
	print(PAUSE_FONT_SIZE, foreColor, WHITE, &rect, "恢复", &point);
	
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
	for (int i = 0; i < VIP_CHECKPOINT_NUMBER + CHECKPOINT_NUMBER; i++) {
		if (pauses[i]) {
			printPause(i);
		}
	}
}

void printPause(const int number) {
	setfont(PAUSE_FONT_SIZE, 0, "宋体");
	const color_t foreColor = number < VIP_CHECKPOINT_NUMBER ? GREEN : BLUE;
	setcolor(foreColor);
	setfontbkcolor(BLACK);
	int y = PAUSE_Y + number * CHECKPOINT_V;
	xyprintf(PAUSE_X, y, "pause");
}

void printPassengers() {
	extern Checkpoint checkpoints[];
	setcolor(WHITE);
	setfillcolor(getcolor());
	for (int i = 0; i < VIP_CHECKPOINT_NUMBER + CHECKPOINT_NUMBER; i++) {
		for (int j = 0; j < checkpoints[i].people; j++) {
			fillellipsef(PASSENGER_X - j * 2 * PASSENGER_R, PASSENGER_Y + i * CHECKPOINT_V, PASSENGER_R, PASSENGER_R);
		}
	}
}

bool onButton(const int x, const int y, const Rect * rect);

bool onVipButton(const int x, const int y) {
	const Rect rect = {VIP_BUTTON_LEFT, VIP_BUTTON_TOP, VIP_BUTTON_LEFT + BUTTON_WIDTH, VIP_BUTTON_TOP + BUTTON_HEIGHT};
	return onButton(x, y, &rect);
} 

bool onPassengerButton(const int x, const int y) {
	const Rect rect = {BUTTON_LEFT, BUTTON_TOP, BUTTON_LEFT + BUTTON_WIDTH, BUTTON_TOP + BUTTON_HEIGHT};
	return onButton(x, y, &rect);
}

bool onCloseButton(const int x, const int y) {
	const Rect rect = {CLOSE_BUTTON_LEFT, CLOSE_BUTTON_TOP, CLOSE_BUTTON_LEFT + BUTTON_WIDTH, CLOSE_BUTTON_TOP + BUTTON_HEIGHT};
	return onButton(x, y, &rect);
}

int onPauseButtons(const int x, const int y) {
	for (int i = 0; i < VIP_CHECKPOINT_NUMBER + CHECKPOINT_NUMBER; i++) {
		const int top = PAUSE_ORIGIN_Y + i * CHECKPOINT_V;
		const Rect rect = {PAUSE_ORIGIN_X, top, PAUSE_ORIGIN_X + PAUSE_WIDTH, top + PAUSE_HEIGHT};
		if (onButton(x, y, &rect)) {
			return i + 1;
		}
	}
	return 0;
}

int onResetButtons(const int x, const int y) {
	for (int i = 0; i < VIP_CHECKPOINT_NUMBER + CHECKPOINT_NUMBER; i++) {
		const int left = PAUSE_ORIGIN_X + PAUSE_WIDTH + SPAN_H;
		const int top = PAUSE_ORIGIN_Y + i * CHECKPOINT_V;
		const Rect rect = {left, top, left + PAUSE_WIDTH, top + PAUSE_HEIGHT};
		if (onButton(x, y, &rect)) {
			return i + 1;
		}
	}
	return 0;
}

bool onButton(const int x, const int y, const Rect * rect) {
	return (rect->left < x && x < rect->right) && (rect->top < y && y <rect->bottom);
}
