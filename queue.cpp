#include <stdio.h>

#include "queue.h"

bool isEmpty(const Queue * queue) {
	if (NULL == queue) {
		return true;		
	}
	return queue->people == 0;
}

bool isFull(const Queue * queue) {
	if (NULL == queue) {
		return true;		
	}
	return queue->people == queue->capacity;
}

bool push(Queue * queue) {
	if (!isFull(queue)) {
		queue->people += 1;
		return true;
	}
	return false;
}

bool shift(Queue * queue) {
	if (!isEmpty(queue)) {
		queue->people -= 1;
		return true;
	}
	return false;
}
