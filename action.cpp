#include <stdio.h>
#include <windows.h>
#include <graphics.h>

#include "action.h"
#include "queue.h"
#include "global.h"

void clickPassenger() {
	extern HANDLE channelLock;
	WaitForSingleObject(channelLock, INFINITE);
	extern Channel channel;
	if (!isFull(&channel)) {
		push(&channel);
		printf("普通乘客进入候机通道\n");
	} else {
		printf("普通候机通道已满\n");
	}
	ReleaseMutex(channelLock);
}

bool hasVip(const char * str);

void clickVip() {
	HANDLE * lock;
	Channel * cl;
	char str[100];
	bool isVip;
	for (int i = 0; i < 3; i++) {
		inputbox_getline("提示输入", "请输入VIP身份号：", str, sizeof(str)/sizeof(*str));
		isVip = hasVip(str);
		if (isVip) {
			extern HANDLE vipChannelLock;
			lock = &vipChannelLock;
			extern Channel vipChannel;
			cl = &vipChannel;
			break;
		} else {
			extern HANDLE channelLock;
			lock = &channelLock;
			extern Channel channel;
			cl = &channel;
		}	
	}
	WaitForSingleObject(*lock, INFINITE);
	extern Channel vipChannel;
	if (!isFull(cl)) {
		push(cl);
		if (isVip) {
			printf("VIP乘客进入候机通道\n");
		} else {
			printf("普通乘客进入候机通道\n");
		}
	} else {
		if (isVip) {
			printf("VIP候机通道已满\n");
		} else {
			printf("普通候机通道已满\n");
		}
	}
	ReleaseMutex(*lock);
}

bool hasVip(const char * str) {
	extern int vipInfoN;
	extern char ** vipInfos;
	if (NULL == vipInfos) {
		return false;
	}
	for (int i = 0; i < vipInfoN; i++) {
		if (strcmp(str, vipInfos[i]) == 0) {
			return true;
		}
	}
	return false;
}

bool hasOtherNotPause(const int number, const int s, const int e);

void clickPause(const int number) {
	extern bool pauses[];
	if (number < VIP_CHECKPOINT_NUMBER) {
		pauses[number] = hasOtherNotPause(number, 0, VIP_CHECKPOINT_NUMBER);
	} else {
		pauses[number] = hasOtherNotPause(number, VIP_CHECKPOINT_NUMBER, VIP_CHECKPOINT_NUMBER + CHECKPOINT_NUMBER);
	}
} 

bool hasOtherNotPause(const int number, const int s, const int e) {
	extern bool pauses[];
	for (int i = s; i < e; i++) {
		if (i != number && !pauses[i]) {
			return true;
		}
	}
	return false;
}

void clickReset(const int number) {
	extern bool pauses[];
	pauses[number] = false;
}

void clock(const int number);

DWORD WINAPI checkpoint(PVOID pM) {
	extern HANDLE checkpointNumLock;
	WaitForSingleObject(checkpointNumLock, INFINITE);
	extern int checkpointNum;
	int number = checkpointNum;
	checkpointNum++;
	ReleaseMutex(checkpointNumLock);
	printf("number: %d\n", number);
	while (true) {
		clock(number);
		extern bool closed;
		if (closed) {
			extern Channel vipChannel;
			extern Channel channel;
			extern Checkpoint checkpoints[];
			if (isEmpty(&vipChannel) && isEmpty(&channel) && isEmpty(&checkpoints[number])) {
				extern bool isLives[];
				isLives[number] = false;
				break;
			}
		}
		api_sleep(1000);
	}
	return 0;
}

void clock(const int number) {
	extern Checkpoint checkpoints[];
	if (!isEmpty(&checkpoints[number]) && checkpoints[number].passtime <= fclock()) {
		shift(&checkpoints[number]);
		checkpoints[number].passtime = fclock() + PASSTIME;
	}
	extern bool pauses[];
	if (!pauses[number] && !isFull(&checkpoints[number])) {
		extern HANDLE vipChannelLock;
		extern HANDLE channelLock;
		const HANDLE * lock = number < VIP_CHECKPOINT_NUMBER ? &vipChannelLock : &channelLock;
		WaitForSingleObject(*lock, INFINITE);
		extern Channel vipChannel;
		extern Channel channel;
		Channel * cl = number < VIP_CHECKPOINT_NUMBER ? &vipChannel : &channel;
		if (!isEmpty(cl)) {
			shift(cl);
			push(&checkpoints[number]);
		}
		ReleaseMutex(*lock);
	}
}
