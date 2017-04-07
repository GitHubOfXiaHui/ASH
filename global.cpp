#include <stdio.h>
#include <windows.h>

#include "global.h"
#include "queue.h"
#include "action.h"

// 候机通道
int channelCurrentNum = 1;
Channel channel = {0.0, 0, channelCurrentNum * CHANNEL_CAPACITY};
HANDLE channelLock = CreateMutex(NULL, false, NULL);

// 安检口
Checkpoint checkpoints[CHECKPOINT_NUMBER];
int checkpointNum = 0;
HANDLE checkpointNumLock = CreateMutex(NULL, false, NULL);

// 暂停标志 
bool pauses[CHECKPOINT_NUMBER];

// 线程是否存活
bool isLives[CHECKPOINT_NUMBER];

// 关闭窗口
bool closed = false; 

void init() {
	// 暂停标志
	adjustCheckpoint();
	// 初始化安检口及存活标志 
	for (int i = 0; i < CHECKPOINT_NUMBER; i++) {
		const Checkpoint tmp = {0.0, 0, CHECKPOINT_CAPACITY};
		checkpoints[i] = tmp;
		
		// 开启安检口线程
		CloseHandle(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)checkpoint, NULL, 0, NULL));
		isLives[i] = true;
	}
	// 开启乘客线程
	CloseHandle(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)passenger, NULL, 0, NULL)); 
}

void adjustCheckpoint() {
	for (int i = 0; i < CHECKPOINT_NUMBER; i++) {
		if (i < channelCurrentNum * CHECKPOINT_NUMBER / CHANNEL_MAX_NUM) {
			pauses[i] = false;	
		} else {
			pauses[i] = true;
		}
	}
}
