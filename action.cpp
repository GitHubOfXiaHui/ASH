#include <stdio.h>
#include <windows.h>
#include <graphics.h>

#include "action.h"
#include "queue.h"
#include "global.h"

void enterChannel();

DWORD WINAPI passenger(PVOID pM) {
	while (true) {
		extern bool closed;
		if (closed) {
			break;
		}
		enterChannel();
		api_sleep(1000);
	}
}

void enterChannel() {
	extern HANDLE channelLock;
	WaitForSingleObject(channelLock, INFINITE);
	extern Channel channel;
	extern int channelCurrentNum;
	if (channelCurrentNum < CHANNEL_MAX_NUM) {
		int psg = random(100);
		if (channel.capacity - channel.people < psg) {
			channelCurrentNum++;
			channel.capacity = channelCurrentNum * CHANNEL_CAPACITY;
		}
		pushAll(&channel, psg);
		if (channel.people > (channelCurrentNum - 1) * CHANNEL_CAPACITY + THRESHOLD) {
			adjustCheckpoint(); 
		}
		printf("乘客进入候机通道\n");
	} else {
		printf("候机通道已满\n");
	}
	ReleaseMutex(channelLock);
}

void pass(const int number);
void enterCheckpoint(const int number);

DWORD WINAPI checkpoint(PVOID pM) {
	extern HANDLE checkpointNumLock;
	WaitForSingleObject(checkpointNumLock, INFINITE);
	extern int checkpointNum;
	int number = checkpointNum;
	checkpointNum++;
	ReleaseMutex(checkpointNumLock);
	while (true) {
		pass(number);
		enterCheckpoint(number);
		extern bool closed;
		if (closed) {
			extern Channel channel;
			extern Checkpoint checkpoints[];
			if (isEmpty(&channel) && isEmpty(&checkpoints[number])) {
				extern bool isLives[];
				isLives[number] = false;
				break;
			}
		}
		api_sleep(1000);
	}
	return 0;
}

void pass(const int number) {
	extern Checkpoint checkpoints[];
	if (!isEmpty(&checkpoints[number]) && checkpoints[number].passtime <= fclock()) {
		shift(&checkpoints[number]);
		printf("乘客通过安检口\n");
		checkpoints[number].passtime = fclock() + PASSTIME;
	}
}

void enterCheckpoint(const int number) {
	extern Checkpoint checkpoints[];
	extern bool pauses[];
	if (!pauses[number] && !isFull(&checkpoints[number])) {
		extern HANDLE channelLock;
		WaitForSingleObject(channelLock, INFINITE);
		extern Channel channel;
		if (!isEmpty(&channel)) {
			shift(&channel);
			extern int channelCurrentNum;
			if (channel.people < (channelCurrentNum - 1) * CHANNEL_CAPACITY - THRESHOLD) {
				channelCurrentNum--;
				channel.capacity = channelCurrentNum * CHANNEL_CAPACITY;
				adjustCheckpoint(); 
			}
			if (isEmpty(&checkpoints[number])) {
				checkpoints[number].passtime = fclock() + PASSTIME;
			}
			push(&checkpoints[number]);
			printf("乘客进入安检口\n");
		}
		ReleaseMutex(channelLock);
	}
}
