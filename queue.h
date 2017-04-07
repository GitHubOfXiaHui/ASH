#ifndef _QUEUE_H_
#define _QUEUE_H_

struct Queue
{
	double passtime;	// 通过安检口的时刻 
	int people;		// 当前人数 
	int capacity;	// 容量上限 
};
typedef struct Queue Queue;

// 候机通道 
typedef struct Queue Channel;

// 安检口 
typedef struct Queue Checkpoint;

// 判断队列是否为空 
bool isEmpty(const Queue * queue);

// 判断队列是否已满 
bool isFull(const Queue * queue);

// 入队 
bool push(Queue * queue);
void pushAll(Queue * queue, const int num);

// 出队 
bool shift(Queue * queue);

#endif
