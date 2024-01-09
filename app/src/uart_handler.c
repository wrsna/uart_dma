#include<uart_handler.h>

#ifdef __cplusplus
extern "C" {
#endif

extern fifo_t keys_fifo;

uint8_t memory0[MEMORY_SIZE];
uint8_t memory1[MEMORY_SIZE];



void uart_start(void){
    sam_uart_opt_t uart_setup = {
        .ul_mck = SystemCoreClock,
        .ul_baudrate = 9600,
        .ul_mode =UART_MR_CHMODE_NORMAL | UART_MR_PAR_NO
    };



    sysclk_enable_peripheral_clock(ID_PIOA);
    sysclk_enable_peripheral_clock(ID_UART);
    sysclk_enable_peripheral_clock(ID_DMAC);


    pio_set_peripheral(PIOA,PIO_PERIPH_A,PIO_PA8A_URXD | PIO_PA9A_UTXD);
    pio_pull_up(PIOA,PIO_PA8A_URXD | PIO_PA9A_UTXD,PIO_PULLUP);

    uart_init(UART, &uart_setup);

    /***********DMAC ****************************/


    PDC_UART -> PERIPH_PTCR = PERIPH_PTCR_RXTEN;
    PDC_UART -> PERIPH_PTCR = PERIPH_PTCR_TXTEN;

    PDC_UART -> PERIPH_RPR = (uint32_t)memory0;
    PDC_UART -> PERIPH_RCR = MEMORY_SIZE;
    PDC_UART -> PERIPH_RNCR = 0;

    PDC_UART -> PERIPH_TPR = (uint32_t)memory1;
    PDC_UART -> PERIPH_TCR = 0;
    PDC_UART -> PERIPH_TNCR = 0;






}
























#ifdef __cplusplus
}
#endif
