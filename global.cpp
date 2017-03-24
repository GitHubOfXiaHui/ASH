#include <stdio.h>
#include <windows.h>

#include "global.h"
#include "queue.h"
#include "action.h"

#define VIP_LEN 6

// 候机通道
Channel vipChannel = {0.0, 0, VIP_CHANNEL_CAPACITY};
Channel channel = {0.0, 0, CHANNEL_CAPACITY};

// 安检口
Checkpoint checkpoints[VIP_CHECKPOINT_NUMBER + CHECKPOINT_NUMBER];

// 候机通道互斥对象 
HANDLE vipChannelLock = CreateMutex(NULL, false, NULL);
HANDLE channelLock = CreateMutex(NULL, false, NULL);

// 暂停标志 
bool pauses[VIP_CHECKPOINT_NUMBER + CHECKPOINT_NUMBER];

// vip信息 
int vipInfoN = 0;
char ** vipInfos = NULL;

// 线程是否存活
bool isLives[] = {true, true, true, true, true, true, true};

// 关闭窗口
bool closed = false; 

void init() {
	// 初始化安检口及暂停标志 
	for (int i = 0; i < VIP_CHECKPOINT_NUMBER + CHECKPOINT_NUMBER; i++) {
		const Checkpoint tmp = {0.0, 0, CHECKPOINT_CAPACITY};
		checkpoints[i] = tmp;
		pauses[i] = false;
	}
	
	// 读取vip信息 
	FILE *fp = fopen("..\\vipInfos.dat", "r");
	if (NULL != fp) {
		fscanf(fp, "%d", &vipInfoN);
		vipInfos = (char **)malloc(vipInfoN * sizeof(char *));
		for (int i = 0; i < vipInfoN; i++) {
			vipInfos[i] = (char *)malloc(VIP_LEN * sizeof(char));
			fscanf(fp, "%s", vipInfos[i]);
		}
	}
	fclose(fp);
	
	// 开启线程
 	CloseHandle(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)checkpoint0, NULL, 0, NULL));
 	CloseHandle(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)checkpoint1, NULL, 0, NULL));
 	CloseHandle(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)checkpoint2, NULL, 0, NULL));
 	CloseHandle(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)checkpoint3, NULL, 0, NULL));
 	CloseHandle(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)checkpoint4, NULL, 0, NULL));
 	CloseHandle(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)checkpoint5, NULL, 0, NULL));
 	CloseHandle(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)checkpoint6, NULL, 0, NULL));
 	
 	//for (int i = 0; i < VIP_CHECKPOINT_NUMBER + CHECKPOINT_NUMBER; i++) {
		 	
 	//}
}
