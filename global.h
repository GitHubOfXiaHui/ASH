#ifndef _GLOBAL_H_
#define _GLOBAL_H_

// ���ͨ��
#define CHANNEL_CAPACITY 10
#define THRESHOLD (CHANNEL_CAPACITY / 5)
#define CHANNEL_MAX_NUM 3

// �����
#define CHECKPOINT_CAPACITY 5
#define CHECKPOINT_NUMBER 6
#define PASSTIME 10

// ��ʼ��ϵͳ����
void init();

// ��������ڿ�������
void adjustCheckpoint(); 

#endif
