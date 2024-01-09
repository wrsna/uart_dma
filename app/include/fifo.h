#ifndef FIFO_H_INCLUDED
#define FIFO_H_INCLUDED


#include<asf.h>


#ifdef __cplusplus
extern "C" {
#endif


#define KEYS_FIFO_SIZE 100


/*********deklaracijja FIFO pomnilnika *********************/


struct _fifo{
    uint16_t write_idx;
    uint16_t read_idx;
    uint16_t fifo_size; // velikost fifo pomnilnika v bytih ?? ne v baytih
    uint8_t  write_protect;
    uint8_t  *buff;
};

typedef struct _fifo fifo_t;






uint16_t fifo_read (fifo_t *fifo, uint8_t *write_addr, uint16_t n); //data = naslov kam naj pišem
uint16_t fifo_write(fifo_t *fifo, const uint8_t *read_addr, uint16_t n); //fifio pomnilnik, naslov začetka podatkov, število bitov podatkov  NASLOV IZ KJE NAJ BEREM
uint16_t fifo_look(fifo_t  *fifo, uint8_t *write_addr);


#endif // FIFO_H_INCLUDED

#ifdef __cplusplus
}
#endif
