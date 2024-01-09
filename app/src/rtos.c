#include <rtos.h>


#ifdef __cplusplus
extern "C" {
#endif


extern rtos_task_t *task_list[];  //definiramo neko zbirko tega tipa z exteren pa povemo da to nekje obstaja in poglej kaj je to drugje
extern rtos_task_t *task_priv;  //leddriver





uint8_t rtos_init(uint32_t slices_us){
    /*pazi na zaokraževanje/deljenje in na obseg števil*/
    uint32_t n = 0;
    uint64_t buff = 0;

    buff = SystemCoreClock/8;
    buff = buff*slices_us;
    n = buff/MICRO_SEC;

    if((n-1) > SysTick_LOAD_RELOAD_Msk) return 1;  //counter value too big

    SysTick->LOAD  = n - 1;
    NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);
    SysTick->VAL   = 0;
    SysTick-> CTRL = 0<<SysTick_CTRL_CLKSOURCE_Pos | SysTick_CTRL_TICKINT_Msk;




    return 0;
}

/************************************************************************/

void rtos_enable(void){
    SysTick-> CTRL |= SysTick_CTRL_ENABLE_Msk;
}

/****************************************************************************/

void rtos_disable(void){
    SysTick-> CTRL |= !SysTick_CTRL_ENABLE_Msk;
}

/*************************************************************************/


void SysTick_Handler(void){
/******jedro programa exeption prekinitve za taske****/

    static uint8_t idx = 0;
    static uint32_t tick = 0;
    uint32_t pending_proces = 0;
    tick++;



/** priviligiran task izvedba vsako rezino**/
    if((task_priv != 0) && task_priv->active){
        task_priv->driver();
        task_priv->last_tick = tick;

    }
/** izvajaj taske **/

    if(task_list[idx]->active){
        task_list[idx]->driver(); //uzemi objekt driver iz te strukture delaš s pointeri
        task_list[idx]->last_tick = tick;
    }


/** skrbi za rollover  **/
    idx++;
    if(task_list[idx] == 0){
        idx = 0;
    }





    pending_proces = SCB->ICSR;

    if(pending_proces & SCB_ICSR_PENDSTSET_Msk){
        while(1); //operacija predolga za časovno rezino

    }
}

/***********************************************************************************************/






#ifdef __cplusplus
}
#endif
