#ifndef RTOS_TASK_H_INCLUDED
#define RTOS_TASK_H_INCLUDED



#ifdef __cplusplus
extern "C" {
#endif


#include<rtos.h>
#include<lcd.h>
#include<bord.h>
#include<fifo.h>
#include<uart_handler.h>

#define DAN 24*60*60*10
#define URA 1*60*60*10
#define MIN 1*60*10







void ura_driver(void);
void calc_driver(void);
void buttons_driver(void);

void led_driver(void);
void text_driver(void);
void sent_driver(void);

void dma_driver(void);


//void change(rtos_task_t *task_list00[10],rtos_task_t *priv);












#endif // RTOS_TASK_H_INCLUDED

#ifdef __cplusplus
}
#endif
