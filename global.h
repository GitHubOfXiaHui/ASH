#ifndef _GLOBAL_H_
#define _GLOBAL_H_

struct WaitingChannel
{
	int passtime;	// 通过安检口的时刻 
	int people;		// 当前人数 
	int capacity;	// 容量上限 
};

typedef struct WaitingChannel WaitingChannel;
typedef struct WaitingChannel SecurityCheckpoint;

bool isEmpty(struct WaitingChannel * queue);

#endif
