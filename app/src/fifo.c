#include<fifo.h>

#ifdef __cplusplus
extern "C" {
#endif

/**************************POMNILNIK*******************************************/
uint8_t keys_buff[KEYS_FIFO_SIZE];

fifo_t keys_fifo = {
    .write_idx = 0,
    .read_idx  = 0,
    .fifo_size = KEYS_FIFO_SIZE,
    .write_protect = 0,
    .buff = keys_buff
};


uint8_t sent_buff[KEYS_FIFO_SIZE];

fifo_t sent_fifo = {
    .write_idx = 0,
    .read_idx  = 0,
    .fifo_size = KEYS_FIFO_SIZE,
    .write_protect = 0,
    .buff = sent_buff
};






/*****************FUNKCIJE*******************************************/

uint16_t fifo_read (fifo_t *fifo, uint8_t *write_addr, uint16_t n){
    uint16_t i;

    for(i = 0; (i<n && fifo->read_idx != fifo->write_idx); i++){

        write_addr[i] = fifo->buff[fifo->read_idx];
        fifo->read_idx++;
        fifo->read_idx = fifo->read_idx%fifo->fifo_size;


    }
    return i;
}

/************************************************************************************************/

uint16_t fifo_write(fifo_t *fifo, const uint8_t *read_addr, uint16_t n){
    uint16_t i;

    if(fifo->write_protect == 0){
        fifo->write_protect = 1;
        for(i = 0; i<n; i++){
            if((fifo->write_idx+1)%fifo->fifo_size == fifo->read_idx){
                break;
            }
            else{
                fifo->buff[fifo->write_idx] = read_addr[i] ;
                fifo->write_idx++;
                fifo->write_idx = fifo->write_idx%fifo->fifo_size;

            }

        }
        fifo->write_protect = 0;
    }

    return i;
}

/********************************************************************************************************/

uint16_t fifo_look(fifo_t  *fifo, uint8_t *write_addr){
    uint16_t read, i;
    read = fifo->read_idx;



    for(i = 0; read != fifo->write_idx; i++){

        write_addr[i] = fifo->buff[read];
        read++;
        read = read%fifo->fifo_size;


    }
    return i;
}










#ifdef __cplusplus
}
#endif
