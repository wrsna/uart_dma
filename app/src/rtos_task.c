#include<rtos_task.h>

#ifdef __cplusplus
extern "C" {
#endif

extern fifo_t keys_fifo;
extern fifo_t sent_fifo;
extern uint8_t memory0[];
extern uint8_t memory1;
/***************OPRAVILA**************************/

rtos_task_t task_lcd = {
    .last_tick = 0,
    .name = "LCD",
    .active = 1,
    .driver = lcd_driver
};


rtos_task_t task_ura = {
    .last_tick = 0,
    .name = "URA",
    .active = 1,
    .driver = ura_driver
};


rtos_task_t task_text = {
    .last_tick = 0,
    .name = "CALC",
    .active = 1,
    .driver = text_driver
};


rtos_task_t task_tipke = {
    .last_tick = 0,
    .name = "TIPKE",
    .active = 1,
    .driver = buttons_driver
};


rtos_task_t task_led = {
    .last_tick = 0,
    .name = "LED",
    .active = 0,
    .driver = led_driver
};


rtos_task_t task_sent = {
    .last_tick = 0,
    .name = "SENT",
    .active = 0,
    .driver = sent_driver,
};


rtos_task_t task_dma = {
    .last_tick = 0,
    .name = "DMA",
    .active = 1,
    .driver = dma_driver,
};




















/*****************FUNKCIJE**************************************************/


void ura_driver(void){
    static uint32_t clk = 0;

    clk++;

    if(!(clk%25)){
        ioport_toggle_pin_level(D1_IDX);
    }
}


void text_driver(void){
    static uint8_t position = 0;
    int i,n = 0;
    uint8_t key;
    /// z isklapljanjem procesov


    n = fifo_read(&keys_fifo,&key,1);

    if((key == 27) && n){
        task_text.active = 0;
        task_sent.active = 1;
    }
    else if(n){

        switch(key){

            case 127:  //backspace
                position--;
                lcd_string[16+position] = ' ';
                break;
            case  13:  // line fead
                for(i=0; i<16;i++){
                    lcd_string[i] = lcd_string[i+16];
                    lcd_string[i+16] = ' ';

                }
                break;
            case 10:  //carage return
                position = 0;
                break;
            default:
                sprintf(lcd_string+16+position,"%c",(char)key);
                position++;
                lcd_string[16+position] = ' ';

                if(position == 16){
                    position = 0;

                    for(i=0; i<16;i++){
                        lcd_string[i] = lcd_string[i+16];
                        lcd_string[i+16] = ' ';

                    }
                }
        }
    }
}


void buttons_driver(void){
    int butons = 0;

    butons = get_button_press();

    if(butons & Mask_T1){
        fifo_write(&keys_fifo,(const unsigned char *)"T1",2);
    }
    if(butons & Mask_T2){
        fifo_write(&keys_fifo,(const unsigned char *)"T2",2);
    }
    if(butons & Mask_T3){
        fifo_write(&keys_fifo,(const unsigned char *)"T3",2);
    }
    if(butons & Mask_T4){
        fifo_write(&keys_fifo,(const unsigned char *)"T4",2);   //write protect še nared
    }
}


void led_driver(void){
    uint8_t  check, data_read;
    static uint8_t  count=0;


    if(count>0){
        if((count%5 == 0)  && (count>50)){
            ioport_set_pin_level(D4_IDX, HIGH);
            ioport_set_pin_level(D2_IDX, HIGH);

        }
        else if((count%5 == 3)  && (count>50)){
            ioport_set_pin_level(D4_IDX, LOW);
            ioport_set_pin_level(D2_IDX, LOW);

        }
        else{
        }

        count--;
    }
    else{
        check = fifo_read (&keys_fifo, &data_read, 1);
        if(check == 1){
            count = data_read * 5 +50;

            /*** test neki nouga  in dela bolš šibajo poodatki ***/
            ioport_set_pin_level(D4_IDX, HIGH);
            ioport_set_pin_level(D2_IDX, HIGH);

            count--;

            /**********************************************************/
        }
        else{
        //error reading or buffer empty
        }
    }

}

void sent_driver(void){
    int n;
    uint8_t key;

    n = fifo_read(&keys_fifo,&key,1);

    if((key == 27) && n){
        task_text.active = 1;
        task_sent.active = 0;
    }
    else if(n){
        PDC_UART -> PERIPH_TPR = (uint32_t)&key;
        PDC_UART -> PERIPH_TCR = 1;
    }
}


void dma_driver(void){
    uint8_t *check_sent;
    static uint8_t overflow = 0;

    check_sent = (uint8_t*)PDC_UART->PERIPH_RPR-1;

    if(*check_sent == 13 && overflow){
        overflow = 0;
        fifo_write(&keys_fifo,(uint8_t *)"ok\r",3);

        PDC_UART -> PERIPH_RPR = (uint32_t)memory0;
        PDC_UART -> PERIPH_RCR = MEMORY_SIZE;
        PDC_UART -> PERIPH_RNCR = 0;
    }
    else if(*check_sent == 13){

        fifo_write(&keys_fifo,(uint8_t*)PDC_UART->PERIPH_RPR - (MEMORY_SIZE - PDC_UART -> PERIPH_RCR),MEMORY_SIZE - PDC_UART -> PERIPH_RCR);
        fifo_write(&keys_fifo,(uint8_t*)"\n",1);

        PDC_UART -> PERIPH_RPR = (uint32_t)memory0;
        PDC_UART -> PERIPH_RCR = MEMORY_SIZE;
        PDC_UART -> PERIPH_RNCR = 0;
    }
    else if(!(uint8_t*)PDC_UART->PERIPH_RCR){
        fifo_write(&keys_fifo,(uint8_t*)"\n\roverfllow\n\r",13);


        PDC_UART -> PERIPH_RPR = (uint32_t)memory0;
        PDC_UART -> PERIPH_RCR = MEMORY_SIZE;
        PDC_UART -> PERIPH_RNCR = 0;

        overflow = 1;
    }
}


/****************************************IZBIRA PROGRAMA  za drug program******************************************************************

rtos_task_t *task_list[10];
rtos_task_t *task_priv = &task_ura;

void change(rtos_task_t *task_list00[10],rtos_task_t *priv){
    int i=0;
    while(task_list00[i]){
        task_list[i] = task_list00[i];
        i++;
    }
    task_priv = priv;
}

****************************************************************************************************************************************************/

rtos_task_t *task_list[] = {&task_tipke, &task_text, &task_lcd, &task_sent, &task_led, &task_dma, 0/*null__pointer*/}; //array od tega tipa sprem samo da so naslovi -> konstantna velikost
rtos_task_t *task_priv = &task_ura;








#ifdef __cplusplus
}
#endif





/**********

Za prašat asistenta kako rešm tc problem za TIOB
                    kako rešem ta problem z dma s pointerji
****************/
