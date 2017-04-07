#include <stdio.h>
#include <windows.h>

#include "global.h"
#include "queue.h"
#include "action.h"

// ���ͨ��
int channelCurrentNum = 1;
Channel channel = {0.0, 0, channelCurrentNum * CHANNEL_CAPACITY};
HANDLE channelLock = CreateMutex(NULL, false, NULL);

// �����
Checkpoint checkpoints[CHECKPOINT_NUMBER];
int checkpointNum = 0;
HANDLE checkpointNumLock = CreateMutex(NULL, false, NULL);

// ��ͣ��־ 
bool pauses[CHECKPOINT_NUMBER];

// �߳��Ƿ���
bool isLives[CHECKPOINT_NUMBER];

// �رմ���
bool closed = false; 

void init() {
	// ��ͣ��־
	adjustCheckpoint();
	// ��ʼ������ڼ�����־ 
	for (int i = 0; i < CHECKPOINT_NUMBER; i++) {
		const Checkpoint tmp = {0.0, 0, CHECKPOINT_CAPACITY};
		checkpoints[i] = tmp;
		
		// ����������߳�
		CloseHandle(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)checkpoint, NULL, 0, NULL));
		isLives[i] = true;
	}
	// �����˿��߳�
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
