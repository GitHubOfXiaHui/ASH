#ifndef _GLOBAL_H_
#define _GLOBAL_H_

// 通道容量上限 
#define VIP_CHANNEL_CAPACITY 20
#define CHANNEL_CAPACITY 100

// 安检口容量上限 
#define CHECKPOINT_CAPACITY 5

// 安检口数量 
#define VIP_CHECKPOINT_NUMBER 2
#define CHECKPOINT_NUMBER 5

// 安检口处理时间
#define PASSTIME 10

// 初始化系统参数
void init();

#endif
