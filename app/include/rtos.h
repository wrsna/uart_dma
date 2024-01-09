#ifndef RTOS_H_INCLUDED
#define RTOS_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif


#include<asf.h>
#include<bord.h>
#define MICRO_SEC 1000000



/**** definicija struktur opravil podatkovnih tipov ****/

typedef void (*ptr_function)(void); // kazavc na funkcijo definiru tip je void je pointer je funkcija


struct task{
    uint32_t last_tick;
    char name[15];
    uint8_t active;
    ptr_function driver;
};

typedef struct task rtos_task_t; //da ne pišm struct task pa nov tip sprem define






///////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////



/**********drklaracija funkcij***********/
uint8_t rtos_init(uint32_t slice_us);
void rtos_enable(void);
void rtos_disable(void);



/** ker je to jedro in neodvisno od taskov zato zdel to jedro in gremo nou file štimat za custom op sistem**/

#endif // RTOS_H_INCLUDED


#ifdef __cplusplus
}
#endif
