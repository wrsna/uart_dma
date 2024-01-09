





#ifdef __cplusplus
extern "C" {
#endif


#include"bord.h"




void bord_init(void){
    delay_init();
    ioport_init();

    //ledice--------------------------------

    ioport_enable_pin(PIO_PC22_IDX);
    ioport_enable_pin(PIO_PC21_IDX);
    ioport_enable_pin(PIO_PC29_IDX);
    ioport_enable_pin(PIO_PD7_IDX);

    ioport_enable_pin(PIO_PB27_IDX);


    ioport_set_pin_dir(PIO_PC22_IDX, IOPORT_DIR_OUTPUT);
    ioport_set_pin_dir(PIO_PC21_IDX, IOPORT_DIR_OUTPUT);
    ioport_set_pin_dir(PIO_PC29_IDX, IOPORT_DIR_OUTPUT);
    ioport_set_pin_dir(PIO_PD7_IDX, IOPORT_DIR_OUTPUT);

    ioport_set_pin_dir(PIO_PB27_IDX, IOPORT_DIR_OUTPUT);

    ioport_set_pin_level(PIO_PC22_IDX, 0);
    ioport_set_pin_level(PIO_PC21_IDX, 0);
    ioport_set_pin_level(PIO_PC29_IDX, 0);
    ioport_set_pin_level(PIO_PD7_IDX, 0);

    pio_set_peripheral(PIOB, PIO_OUTPUT_0, PIO_ABSR_P27);


    //tipke-----------------------------------------------------------------------

    ioport_enable_pin(PIO_PC23_IDX);
    ioport_enable_pin(PIO_PC24_IDX);
    ioport_enable_pin(PIO_PC25_IDX);
    ioport_enable_pin(PIO_PC26_IDX);


    ioport_set_pin_dir(PIO_PC23_IDX, IOPORT_DIR_INPUT);
    ioport_set_pin_dir(PIO_PC24_IDX, IOPORT_DIR_INPUT);
    ioport_set_pin_dir(PIO_PC25_IDX, IOPORT_DIR_INPUT);
    ioport_set_pin_dir(PIO_PC26_IDX, IOPORT_DIR_INPUT);



    ioport_set_pin_mode(PIO_PC23_IDX, IOPORT_MODE_DEBOUNCE);
    ioport_set_pin_mode(PIO_PC24_IDX, IOPORT_MODE_DEBOUNCE);
    ioport_set_pin_mode(PIO_PC25_IDX, IOPORT_MODE_DEBOUNCE);
    ioport_set_pin_mode(PIO_PC26_IDX, IOPORT_MODE_DEBOUNCE);

    ioport_set_pin_mode(PIO_PC23_IDX, IOPORT_MODE_PULLUP);
    ioport_set_pin_mode(PIO_PC24_IDX, IOPORT_MODE_PULLUP);
    ioport_set_pin_mode(PIO_PC25_IDX, IOPORT_MODE_PULLUP);
    ioport_set_pin_mode(PIO_PC26_IDX, IOPORT_MODE_PULLUP);





}



int get_button_state(void){
    int stanje = 0;
    int pin1 = 0, pin2 = 0, pin3 = 0, pin4 = 0;
    int maska1 = 0, maska2 = 0, maska3 = 0, maska4 = 0;

    maska1 = 1<<0;
    maska2 = 1<<1;
    maska3 = 1<<2;
    maska4 = 1<<3;

    pin1 = ioport_get_pin_level(PIO_PC23_IDX)<<0;
    pin2 = ioport_get_pin_level(PIO_PC24_IDX)<<1;
    pin3 = ioport_get_pin_level(PIO_PC25_IDX)<<2;
    pin4 = ioport_get_pin_level(PIO_PC26_IDX)<<3;

    stanje = (pin1 & maska1) | stanje;
    stanje = (pin2 & maska2) | stanje;
    stanje = (pin3 & maska3) | stanje;
    stanje = (pin4 & maska4) | stanje;

    return stanje;


}


int get_button_press(void){

    static int old_state = 0;
    int state = 0;
    int state1 = 0, state2 = 0, state3 = 0, state4 = 0;
    int leds = 0, leds1 = 0, leds2 = 0, leds3 = 0, leds4 = 0;


    state1 = ioport_get_pin_level(PIO_PC23_IDX)<<0;
    state2 = ioport_get_pin_level(PIO_PC24_IDX)<<1;
    state3 = ioport_get_pin_level(PIO_PC25_IDX)<<2;
    state4 = ioport_get_pin_level(PIO_PC26_IDX)<<3;

    state = state1 | state2 | state3 | state4;

    if((old_state & 1<<0) && (state1 == 0))leds1 = 1<<0;
    if((old_state & 1<<1) && (state2 == 0))leds2 = 1<<1;
    if((old_state & 1<<2) && (state3 == 0))leds3 = 1<<2;
    if((old_state & 1<<3) && (state4 == 0))leds4 = 1<<3;

    leds = leds1 | leds2 | leds3 | leds4;
    old_state = state;

    return leds;

}



void adc_setup(void){
	sysclk_enable_peripheral_clock(ID_ADC);
	adc_init(ADC, SystemCoreClock, 6400000, ADC_STARTUP_TIME_4);
	adc_configure_timing(ADC, 0, ADC_SETTLING_TIME_3, 1);
	adc_set_resolution(ADC, ADC_12_BITS);
	adc_configure_trigger(ADC, ADC_TRIG_SW,0);
	adc_enable_channel(ADC, ADC_CHANNEL_7);


}



uint32_t adc_read(void){
    int status = 0;
    int mask = 1<<7;

    adc_start(ADC);

    while(1){

        status = adc_get_status(ADC) & mask;

        if(status){
            return adc_get_channel_value(ADC, ADC_CHANNEL_7);
        }

    }




}




void timer_init(void){

    tc_init(TC0,0, TC_CMR_TCCLKS_TIMER_CLOCK3 | TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC |TC_CMR_BCPC_CLEAR | TC_CMR_BCPB_SET );//

    tc_enable_interrupt(TC0, 0,  TC_IER_CPBS | TC_IER_CPCS);//ne vem zaki se RB compare ne proi??

    tc_write_rc(TC0, 0, 2625000);
    tc_write_rb(TC0, 0, 2625000/2);

    tc_start(TC0,0);



}

#ifdef __cplusplus
}
#endif
