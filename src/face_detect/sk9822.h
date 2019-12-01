#ifndef _SK9822_H
#define _SK9822_H
#include "stddef.h"
#include "stdbool.h"

#define SK9822_LED_NUMBER  12
#define SK9822_BUF_LEN  14

bool sk9822_clear(void);
bool sk9822_seton(void);
void sk9822_ledon(uint16_t lednum,uint8_t R, uint8_t G, uint8_t B);
void sk9822_init_gpiohs(void);
bool sk9822_send_data_gpiohs(uint32_t *leddata,uint8_t led_cnt);

extern uint32_t sk9822_ledbuf[SK9822_BUF_LEN];


#endif


