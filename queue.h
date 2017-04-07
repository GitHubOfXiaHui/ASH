#ifndef _QUEUE_H_
#define _QUEUE_H_

struct Queue
{
	double passtime;	// ͨ������ڵ�ʱ�� 
	int people;		// ��ǰ���� 
	int capacity;	// �������� 
};
typedef struct Queue Queue;

// ���ͨ�� 
typedef struct Queue Channel;

// ����� 
typedef struct Queue Checkpoint;

// �ж϶����Ƿ�Ϊ�� 
bool isEmpty(const Queue * queue);

// �ж϶����Ƿ����� 
bool isFull(const Queue * queue);

// ��� 
bool push(Queue * queue);
void pushAll(Queue * queue, const int num);

// ���� 
bool shift(Queue * queue);

#endif
