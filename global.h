#ifndef _GLOBAL_H_
#define _GLOBAL_H_

struct WaitingChannel
{
	int passtime;	// ͨ������ڵ�ʱ�� 
	int people;		// ��ǰ���� 
	int capacity;	// �������� 
};

typedef struct WaitingChannel WaitingChannel;
typedef struct WaitingChannel SecurityCheckpoint;

bool isEmpty(struct WaitingChannel * queue);

#endif
