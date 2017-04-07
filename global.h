#ifndef _GLOBAL_H_
#define _GLOBAL_H_

// 候机通道
#define CHANNEL_CAPACITY 10
#define THRESHOLD (CHANNEL_CAPACITY / 5)
#define CHANNEL_MAX_NUM 3

// 安检口
#define CHECKPOINT_CAPACITY 5
#define CHECKPOINT_NUMBER 6
#define PASSTIME 10

// 初始化系统参数
void init();

// 调整安检口开放数量
void adjustCheckpoint(); 

#endif
