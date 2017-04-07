#include <stdio.h>
#include <windows.h>

#include "global.h"
#include "queue.h"
#include "action.h"

#define VIP_LEN 6

// 候机通道
Channel vipChannel = {0.0, 0, VIP_CHANNEL_CAPACITY};
Channel channel = {0.0, 0, CHANNEL_CAPACITY};
// 候机通道互斥对象 
HANDLE vipChannelLock = CreateMutex(NULL, false, NULL);
HANDLE channelLock = CreateMutex(NULL, false, NULL);

// 安检口
Checkpoint checkpoints[VIP_CHECKPOINT_NUMBER + CHECKPOINT_NUMBER];
// 安检口编号 
int checkpointNum = 0;
// 安检口编号互斥对象 
HANDLE checkpointNumLock = CreateMutex(NULL, false, NULL);

// 暂停标志 
bool pauses[VIP_CHECKPOINT_NUMBER + CHECKPOINT_NUMBER];

// vip信息 
int vipInfoN = 0;
char ** vipInfos = NULL;

// 线程是否存活
bool isLives[VIP_CHECKPOINT_NUMBER + CHECKPOINT_NUMBER];

// 关闭窗口
bool closed = false; 

void init() {
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
	
	// 初始化安检口、暂停标志及存活标志 
	for (int i = 0; i < VIP_CHECKPOINT_NUMBER + CHECKPOINT_NUMBER; i++) {
		const Checkpoint tmp = {0.0, 0, CHECKPOINT_CAPACITY};
		checkpoints[i] = tmp;
		pauses[i] = false;
		
		// 开启线程
		CloseHandle(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)checkpoint, NULL, 0, NULL));
		isLives[i] = true;
	}
	
}
