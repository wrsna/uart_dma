#include <asf.h>
#include"lcd.h"
#include"bord.h"
// ali je že uključen v tasks ja #include<rtos.h>//jedro sistem definiri nastavitve in sproži
#include<rtos_task.h>//sprogramiran sistem
#include<fifo.h>
#include<uart_handler.h>

#ifdef __cplusplus
extern "C" {
#endif


//global variables
extern fifo_t keys_fifo;

int main (void){
    /* sets the processor clock according to conf_clock.h definitions */
    sysclk_init();

    /* disable wathcdog */
    WDT->WDT_MR = WDT_MR_WDDIS;

    /*********************   HW init   ***************************/
    bord_init();
    lcd_init();
    uart_start();


    NVIC_EnableIRQ(SysTick_IRQn);
    //a prioritetaje za exception avtomatsko pred interupt in jih gleda ločeno ker za SysTick mam nastavljeno na 15 ?



    /***********************RTOS**************************************/
    if(rtos_init(20000) != 0){
        while(1);
    }//napaka rezina predolga za števec števec se bo obrnu napaka da operacija uzame preveč čase je drugje
    rtos_enable();

/************************ test    *******************************/

    /********************* Main loop     ***************************/

    while(1){

    }
}




#ifdef __cplusplus
}
#endif
