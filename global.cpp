#include <stdio.h>
#include <windows.h>

#include "global.h"
#include "queue.h"
#include "action.h"

#define VIP_LEN 6

// ���ͨ��
Channel vipChannel = {0.0, 0, VIP_CHANNEL_CAPACITY};
Channel channel = {0.0, 0, CHANNEL_CAPACITY};
// ���ͨ��������� 
HANDLE vipChannelLock = CreateMutex(NULL, false, NULL);
HANDLE channelLock = CreateMutex(NULL, false, NULL);

// �����
Checkpoint checkpoints[VIP_CHECKPOINT_NUMBER + CHECKPOINT_NUMBER];
// ����ڱ�� 
int checkpointNum = 0;
// ����ڱ�Ż������ 
HANDLE checkpointNumLock = CreateMutex(NULL, false, NULL);

// ��ͣ��־ 
bool pauses[VIP_CHECKPOINT_NUMBER + CHECKPOINT_NUMBER];

// vip��Ϣ 
int vipInfoN = 0;
char ** vipInfos = NULL;

// �߳��Ƿ���
bool isLives[VIP_CHECKPOINT_NUMBER + CHECKPOINT_NUMBER];

// �رմ���
bool closed = false; 

void init() {
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
	
	// ��ʼ������ڡ���ͣ��־������־ 
	for (int i = 0; i < VIP_CHECKPOINT_NUMBER + CHECKPOINT_NUMBER; i++) {
		const Checkpoint tmp = {0.0, 0, CHECKPOINT_CAPACITY};
		checkpoints[i] = tmp;
		pauses[i] = false;
		
		// �����߳�
		CloseHandle(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)checkpoint, NULL, 0, NULL));
		isLives[i] = true;
	}
	
}
