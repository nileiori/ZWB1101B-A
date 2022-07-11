#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "queue.h"


bool queue_init(queue_t *q, uint8_t *data, uint32_t item_cnt, uint32_t item_size)
{
    q->pbuff = data;
    q->item_cnt = item_cnt;
    q->item_size = item_size;
    q->front = 0;
    q->rear = 0;

    return true;
}

bool queue_en(queue_t *q, uint8_t *data)
{
    if (((q->rear+1)%q->item_cnt) == q->front)
        return false;
    
    memcpy(q->pbuff + (q->rear*q->item_size), data, q->item_size);
    q->rear = (q->rear+1)%q->item_cnt;

    return true;
}

bool queue_de(queue_t *q, uint8_t *data)
{
    if (q->front == q->rear)
        return false;
    
    if (data)
    {
        memcpy(data, q->pbuff + (q->front*q->item_size), q->item_size);
    }
    q->front = (q->front+1)%q->item_cnt;

    return true;
}


