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
	}
	ReleaseMutex(channelLock);
}

bool hasVip(const char * str);

void clickVip() {
	HANDLE * lock;
	Channel * cl;
	char str[100];
	for (int i = 0; i < 3; i++) {
		inputbox_getline("提示输入", "请输入VIP身份号：", str, sizeof(str)/sizeof(*str));
		if (hasVip(str)) {
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

void clickClose() {
	printf("点击下班按钮\n");
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

DWORD WINAPI checkpoint0(PVOID pM) {
	while (true) {
		clock(0);
		extern bool closed;
		if (closed) {
			extern Channel vipChannel;
			extern Channel channel;
			extern Checkpoint checkpoints[];
			if (isEmpty(&vipChannel) && isEmpty(&channel) && isEmpty(&checkpoints[0])) {
				extern bool isLives[];
				isLives[0] = false;
				break;
			}
		}
	}
	return 0;
}

DWORD WINAPI checkpoint1(PVOID pM) {
	while (true) {
		clock(1);
		extern bool closed;
		if (closed) {
			extern Channel vipChannel;
			extern Channel channel;
			extern Checkpoint checkpoints[];
			if (isEmpty(&vipChannel) && isEmpty(&channel) && isEmpty(&checkpoints[1])) {
				extern bool isLives[];
				isLives[1] = false;
				break;
			}
		}
	}
	return 0;
}

DWORD WINAPI checkpoint2(PVOID pM) {
	while (true) {
		clock(2);
		extern bool closed;
		if (closed) {
			extern Channel vipChannel;
			extern Channel channel;
			extern Checkpoint checkpoints[];
			if (isEmpty(&vipChannel) && isEmpty(&channel) && isEmpty(&checkpoints[2])) {
				extern bool isLives[];
				isLives[2] = false;
				break;
			}
		}
	}
	return 0;
}

DWORD WINAPI checkpoint3(PVOID pM) {
	while (true) {
		clock(3);
		extern bool closed;
		if (closed) {
			extern Channel vipChannel;
			extern Channel channel;
			extern Checkpoint checkpoints[];
			if (isEmpty(&vipChannel) && isEmpty(&channel) && isEmpty(&checkpoints[3])) {
				extern bool isLives[];
				isLives[3] = false;
				break;
			}
		}
	}
	return 0;
}

DWORD WINAPI checkpoint4(PVOID pM) {
	while (true) {
		clock(4);
		extern bool closed;
		if (closed) {
			extern Channel vipChannel;
			extern Channel channel;
			extern Checkpoint checkpoints[];
			if (isEmpty(&vipChannel) && isEmpty(&channel) && isEmpty(&checkpoints[4])) {
				extern bool isLives[];
				isLives[4] = false;
				break;
			}
		}
	}
	return 0;
}

DWORD WINAPI checkpoint5(PVOID pM) {
	while (true) {
		clock(5);
		extern bool closed;
		if (closed) {
			extern Channel vipChannel;
			extern Channel channel;
			extern Checkpoint checkpoints[];
			if (isEmpty(&vipChannel) && isEmpty(&channel) && isEmpty(&checkpoints[5])) {
				extern bool isLives[];
				isLives[5] = false;
				break;
			}
		}
	}
	return 0;
}

DWORD WINAPI checkpoint6(PVOID pM) {
	while (true) {
		clock(6);
		extern bool closed;
		if (closed) {
			extern Channel vipChannel;
			extern Channel channel;
			extern Checkpoint checkpoints[];
			if (isEmpty(&vipChannel) && isEmpty(&channel) && isEmpty(&checkpoints[6])) {
				extern bool isLives[];
				isLives[6] = false;
				break;
			}
		}
	}
	return 0;
}

void clock(const int number) {
	while (true) {
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
		api_sleep(1000);
	}
}
