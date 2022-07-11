#ifndef __QUEUE_H__
#define __QUEUE_H__


typedef struct 
{
    uint8_t  *pbuff;
    uint32_t front;
    uint32_t rear;
    uint32_t item_cnt;
    uint32_t item_size;

} queue_t;

extern bool queue_init(queue_t *q, uint8_t *data, uint32_t item_cnt, uint32_t item_size);
extern bool queue_en(queue_t *q, uint8_t *data);
extern bool queue_de(queue_t *q, uint8_t *data);



#endif
