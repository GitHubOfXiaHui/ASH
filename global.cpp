#include <stdio.h>
#include <windows.h>

#include "global.h"
#include "queue.h"
#include "action.h"

#define VIP_LEN 6

// ���ͨ��
Channel vipChannel = {0.0, 0, VIP_CHANNEL_CAPACITY};
Channel channel = {0.0, 0, CHANNEL_CAPACITY};

// �����
Checkpoint checkpoints[VIP_CHECKPOINT_NUMBER + CHECKPOINT_NUMBER];

// ���ͨ��������� 
HANDLE vipChannelLock = CreateMutex(NULL, false, NULL);
HANDLE channelLock = CreateMutex(NULL, false, NULL);

// ��ͣ��־ 
bool pauses[VIP_CHECKPOINT_NUMBER + CHECKPOINT_NUMBER];

// vip��Ϣ 
int vipInfoN = 0;
char ** vipInfos = NULL;

// �߳��Ƿ���
bool isLives[] = {true, true, true, true, true, true, true};

// �رմ���
bool closed = false; 

void init() {
	// ��ʼ������ڼ���ͣ��־ 
	for (int i = 0; i < VIP_CHECKPOINT_NUMBER + CHECKPOINT_NUMBER; i++) {
		const Checkpoint tmp = {0.0, 0, CHECKPOINT_CAPACITY};
		checkpoints[i] = tmp;
		pauses[i] = false;
	}
	
	// ��ȡvip��Ϣ 
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
	
	// �����߳�
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
